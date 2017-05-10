/*
 * ELoggerManager.cpp
 *
 *  Created on: 2014-6-23
 *      Author: cxxjava@163.com
 */

#include "ELoggerManager.hh"
#include "ELoggerManagerImp.hh"
#include "ELoggerImp.hh"
#include "EConfigurator.hh"

namespace efc {
namespace log {

ELoggerManagerImp* getLoggerManagerImp() {
	static ELoggerManagerImp imp;
	return &imp;
}

ELoggerManagerImp::~ELoggerManagerImp() {
	delete configFile;
	delete loggers;
}

ELoggerManagerImp::ELoggerManagerImp() : configFile(null), lastModified(0L) {
	loggers = new EConcurrentHashMap<EString, ELogger>();
	rootLogger = getLogger(ROOT_LOGGER_NAME);
}

void ELoggerManagerImp::reset(const char* config) {
	delete configFile;
	configFile = new EFile(config);

	flushConfig();
}

void ELoggerManagerImp::flushConfig() {
	SYNCBLOCK(&flushLock) {
		if (configFile->exists()) {
			// if properties file is exist then use it first.
			llong lm = configFile->lastModified();
			if (lastModified == lm) {
				return;
			}
			lastModified = lm;

			EConfig properties;
			properties.load(configFile);

			EConfigurator configurator;
			sp<EConfiguration> conf = configurator.doConfigure(&properties);
			if (conf != null) {
				atomic_store(&this->configuration, conf);
			}
		} else {
			// if not set properties file then output to stdout for default.
			const char* default_props = "log4j.rootLogger=TRACE, stdout\r\n"
					"log4j.appender.stdout = org.apache.log4j.ConsoleAppender\r\n"
					"log4j.appender.stdout.Target = System.out\r\n"
					"log4j.appender.stdout.layout = org.apache.log4j.PatternLayout\r\n"
					"log4j.appender.stdout.layout.ConversionPattern = %d{ABSOLUTE} %5p %c{1}:%L - %m%n";
			EByteArrayInputStream bais((void*)default_props, strlen(default_props));
			EConfig properties;
			properties.load(&bais);
			EConfigurator configurator;
			sp<EConfiguration> conf = configurator.doConfigure(&properties);
			if (conf != null) {
				atomic_store(&this->configuration, conf);
			}

			fprintf(stdout, "Not found log4j.properties file, logger default output to stdout.\n");
		}
    }}
}

sp<ELogger> ELoggerManagerImp::getLogger(const char* name) {
	sp<EString> name_(new EString(name));
	sp<ELogger> logger = loggers->get(name_.get());
	if (logger == null) {
		logger = new ELoggerImp(name);
		sp<ELogger> oldLogger = loggers->putIfAbsent(name_, logger);
		if (oldLogger != null)
			logger = oldLogger;
	}
	return logger;
}

sp<EConcurrentCollection<ELogger> > ELoggerManagerImp::getLoggers() {
	return loggers->values();
}

void ELoggerManagerImp::remove(const char* name) {
	EString s(name);
	loggers->remove(&s);
}

sp<EConfiguration> ELoggerManagerImp::getCurrentConfigure() {
	return atomic_load(&this->configuration);
}

} /* namespace log */
} /* namespace efc */
