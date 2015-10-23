/*
 * EConfigurator.cpp
 *
 *  Created on: 2015-7-27
 *      Author: cxxjava@163.com
 */

#include "EConfigurator.hh"
#include "EOptionConverter.hh"
#include "ELoggerManager.hh"
#include "ELoggerImp.hh"
#include "ELayoutFactory.hh"
#include "EAppenderFactory.hh"
#include "ESimpleLayout.hh"
#include "EFileAppender.hh"

namespace efc {
namespace log {

static const char* CATEGORY_PREFIX = "log4j.category.";
static const char* LOGGER_PREFIX   = "log4j.logger.";
static const char* FACTORY_PREFIX = "log4j.factory";
static const char* ADDITIVITY_PREFIX = "log4j.additivity.";
static const char* ROOT_CATEGORY_PREFIX = "log4j.rootCategory";
static const char* ROOT_LOGGER_PREFIX   = "log4j.rootLogger";
static const char* APPENDER_PREFIX = "log4j.appender.";
static const char* RENDERER_PREFIX = "log4j.renderer.";
static const char* THRESHOLD_PREFIX = "log4j.threshold";

/**
 Special level value signifying inherited behaviour. The current
 value of this string constant is <b>inherited</b>. {@link #NULL}
 is a synonym.  */
static const char* LEVEL_INHERITED = "inherited";

/**
 Special level signifying inherited behaviour, same as {@link
 #INHERITED}. The current value of this string constant is
 <b>null</b>. */
static const char* LEVEL_NULL = "null";

EConfigurator::~EConfigurator() {
}

EConfigurator::EConfigurator() : threshold(ELogger::LEVEL_TRACE) {
}

sp<EConfiguration> EConfigurator::doConfigure(EConfig* properties) {
	EString thresholdStr = EOptionConverter::findAndSubst(THRESHOLD_PREFIX, properties);
	if (thresholdStr.length() > 0) {
		threshold = EOptionConverter::toLevel(thresholdStr.c_str());
	}

	/**
	 * New configure.
	 */
	EArray<EString*> loggerNames;
	findAllLoggers(loggerNames, properties);
	sp<EConfiguration> configure = new EConfiguration(loggerNames, threshold);
	parseRootLogger(configure, properties);
	parseSubLoggers(configure, properties);
	return configure;
}

void EConfigurator::findAllLoggers(EArray<EString*>& loggerNames, EConfig* properties) {
	EArray<EString*> keys = properties->keyNames();
	for (int i = 0; i < keys.size(); i++) {
		EString* key = keys[i];
		if (key->startsWith(CATEGORY_PREFIX)
				|| key->startsWith(LOGGER_PREFIX)) {
			EString loggerName;
			if (key->startsWith(CATEGORY_PREFIX)) {
				loggerName = key->substring(eso_strlen(CATEGORY_PREFIX));
			} else if (key->startsWith(LOGGER_PREFIX)) {
				loggerName = key->substring(eso_strlen(LOGGER_PREFIX));
			}
			//printf("loggerName=%s\n", loggerName.c_str());
			loggerNames.add(new EString(loggerName));
		}
	}
}

void EConfigurator::parseRootLogger(sp<EConfiguration>& configure, EConfig* props) {
	EString value = EOptionConverter::findAndSubst(ROOT_LOGGER_PREFIX, props);
	if (value.length() == 0) {
		value = EOptionConverter::findAndSubst(ROOT_CATEGORY_PREFIX, props);
	}

	if (value.length() > 0) {
		ELoggerConfig* newConfig = parseCategory(configure, props, ROOT_LOGGER_NAME, value.c_str());
		configure->addLoggerConfig(newConfig);
	}
	else {
		configure->addLoggerConfig(new ELoggerConfig(configure.get(), ROOT_LOGGER_NAME, ELogger::LEVEL_TRACE, true));
	}
}

void EConfigurator::parseSubLoggers(sp<EConfiguration>& configure, EConfig* props) {
	EA<EString*> sortedNames = configure->getSortedLoggerNames();

	for (int i = 0; i < sortedNames.length(); i++) {
		const char* loggerName = sortedNames[i]->c_str();
		//printf("loggerName=%s\n", loggerName);
		EString prefix(LOGGER_PREFIX);
		EString value = EOptionConverter::findAndSubst(prefix.concat(loggerName).c_str(), props);
		if (value.length() == 0) {
			prefix = CATEGORY_PREFIX;
			value = EOptionConverter::findAndSubst(prefix.concat(loggerName).c_str(), props);
		}

		if (value.length() > 0) {
			ELoggerConfig* newConfig = parseCategory(configure, props, loggerName, value.c_str());
			configure->addLoggerConfig(newConfig);
		}
		else {
			ELoggerConfig* parent = configure->getLoggerConfig(loggerName);
			configure->addLoggerConfig(new ELoggerConfig(configure.get(), loggerName, parent->getLevel(), parent->getAdditivity()));
		}
	}
}

ELoggerConfig* EConfigurator::parseCategory(sp<EConfiguration>& configure, EConfig* props,
		const char* loggerName, const char* value) {
	//value = [level|INHERITED|NULL], appenderName, appenderName, ...

	EArray<EString*> vs = EPattern::split(",", value);

	ELogger::Level level = ELogger::LEVEL_TRACE;
	boolean additive = true;

	// parse additivity
	EString additivity(ADDITIVITY_PREFIX);
	additivity.concat(loggerName); //ADDITIVITY_PREFIX + loggerName
	EString v = EOptionConverter::findAndSubst(additivity.c_str(), props);
	// touch additivity only if necessary
	if (v.trim().length() > 0) {
		additive = EBoolean::parseBoolean(v.c_str());
	}

	// parse level
	if (vs.size() > 0) {
		if (vs[0]->equalsIgnoreCase(LEVEL_INHERITED) || vs[0]->equalsIgnoreCase(LEVEL_NULL)) {
			level = configure->getLoggerConfig(loggerName)->getLevel();
		}
		else {
			level = EOptionConverter::toLevel(vs[0]->c_str());
		}
	}

	// new logger config.
	ELoggerConfig* lc = new ELoggerConfig(configure.get(), loggerName, level, additive);

	// parse all appender
	for (int i = 1; i < vs.length(); i++) {
		if (vs[i]->trim().length() == 0)
			continue;

		const char* appenderName = vs[i]->c_str();
		sp<EAppender> appender = parseAppender(configure, props, appenderName);
		if (appender != null) {
			lc->addAppender(appender);
		}
	}

	return lc;
}

sp<EAppender> EConfigurator::parseAppender(sp<EConfiguration>& configure, EConfig* props, const char* an) {
	sp<EAppender>* appender = null;
	EString appenderName(an);
	if ((appender = (sp<EAppender>*)appenderCache.get(&appenderName)) != null) {
		return *appender;
	}

	// Appender was not previously initialized.
	EString prefix(APPENDER_PREFIX); //APPENDER_PREFIX + appenderName;
	prefix.concat(appenderName);

	EAppender* newAppender = EAppenderFactory::newInstance(configure.get(), props, prefix, configure);
	if (newAppender) {
		EString layoutPrefix = prefix + ".layout";
		ELayout* newLayout = ELayoutFactory::newInstance(props, layoutPrefix);
		if (!newLayout && newAppender->requiresLayout()) {
			newLayout = new ESimpleLayout(props, layoutPrefix);
		}
		newAppender->setLayout(newLayout);

		EFileAppender* fa = dynamic_cast<EFileAppender*>(newAppender);
		if (fa) {
			configure->addLoggerFile(fa->getFile(), fa);
		}

		appender = new sp<EAppender>(newAppender);
		appenderCache.put(new EString(an), appender);
		return *appender;
	}

	return null;
}

} /* namespace log */
} /* namespace efc */
