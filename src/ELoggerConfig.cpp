/*
 * ELoggerConfig.cpp
 *
 *  Created on: 2015-7-27
 *      Author: cxxjava@163.com
 */

#include "./ELoggerConfig.hh"
#include "./ELoggerManagerImp.hh"
#include "./EConfiguration.hh"

namespace efc {
namespace log {

ELoggerConfig::ELoggerConfig(EConfiguration* conf, const char* name_,
		ELogger::Level level, boolean additive_) :
		name(name_), configuration(conf), currentLogLevel(level), minThreshold(
				ELogger::LEVEL_OFF), additive(additive_) {
}

const char* ELoggerConfig::getName() {
	return name.c_str();
}

ELogger::Level ELoggerConfig::getLevel() {
	return (ELogger::Level)ES_MAX(currentLogLevel.get(), minThreshold);
}

void ELoggerConfig::setLevel(ELogger::Level level) {
	currentLogLevel.set(level);
}

boolean ELoggerConfig::getAdditivity() {
	return additive;
}

boolean ELoggerConfig::isRootConfig() {
	return name.equals(ROOT_LOGGER_NAME);
}

void ELoggerConfig::addAppender(sp<EAppender>& appender) {
	appenders.add(appender);
	minThreshold = ES_MIN(minThreshold, appender->getThreshold());
}

boolean ELoggerConfig::isLevelEnabled(int logLevel) {
	return (logLevel >= getLevel());
}

ELogger::Level ELoggerConfig::log(ELogEvent& event) {
	if (isLevelEnabled(event.getLevel())) {
		for (int i = 0; i < appenders.size(); i++) {
			appenders.getAt(i)->append(event);
		}
	}

	if (getAdditivity()) {
		ELoggerConfig* parent = configuration->getParentLoggerConfig(getName());
		if (parent != null) {
			parent->log(event);
		}
	}

	return getLevel();
}

} /* namespace log */
} /* namespace efc */
