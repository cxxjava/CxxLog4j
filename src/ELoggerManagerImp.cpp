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
	}
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
