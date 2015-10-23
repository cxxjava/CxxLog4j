/*
 * EAppenderFactory.cpp
 *
 *  Created on: 2015-8-2
 *      Author: cxxjava@163.com
 */

#include "EAppenderFactory.hh"
#include "EOptionConverter.hh"
#include "EConsoleAppender.hh"
#include "EFileAppender.hh"
#include "ERollingFileAppender.hh"
#include "EDailyRollingFileAppender.hh"

namespace efc {
namespace log {

static const char* CLASSNAME_CONSOLE_APPENDER = "org.apache.log4j.ConsoleAppender";
static const char* CLASSNAME_FILE_APPENDER = "org.apache.log4j.FileAppender";
static const char* CLASSNAME_ROLLING_FILE_APPENDER = "org.apache.log4j.RollingFileAppender";
static const char* CLASSNAME_DAILROLLING_FILE_APPENDER = "org.apache.log4j.DailyRollingFileAppender";
static const char* CLASSNAME_ASYNC_APPENDER = "org.apache.log4j.AsyncAppender";
static const char* CLASSNAME_SOCKET_APPENDER = "org.apache.log4j.SocketAppender";

EAppender* EAppenderFactory::newInstance(EConfiguration* conf, EConfig* props, EString& prefix, sp<EConfiguration>& configure) {
	// Get appender threshold : log4j.appender.XXX.Threshold = [level]
	ELogger::Level threshold = configure->getGlobalThreshold();
	EString thresholdPrefix = prefix + ".Threshold";
	EString thresholdStr = EOptionConverter::findAndSubst(thresholdPrefix.c_str(), props);
	if (thresholdStr.length() > 0) {
		threshold = ES_MAX(threshold, EOptionConverter::toLevel(thresholdStr.c_str()));
	}

	// Get the value of the property in string form
	EString className = EOptionConverter::findAndSubst(prefix.c_str(), props);
	if (className.length() > 0) {
		if (className.equalsIgnoreCase(CLASSNAME_CONSOLE_APPENDER)) {
			return new EConsoleAppender(conf, props, prefix, threshold);
		}
		else if (className.equalsIgnoreCase(CLASSNAME_FILE_APPENDER)) {
			return new EFileAppender(conf, props, prefix, threshold);
		}
		else if (className.equalsIgnoreCase(CLASSNAME_ROLLING_FILE_APPENDER)) {
			return new ERollingFileAppender(conf, props, prefix, threshold);
		}
		else if (className.equalsIgnoreCase(CLASSNAME_DAILROLLING_FILE_APPENDER)) {
			return new EDailyRollingFileAppender(conf, props, prefix, threshold);
		}
		else {
			//TODO...
			return new EConsoleAppender(conf, props, prefix, threshold);
		}
	}
	return null;
}

} /* namespace log */
} /* namespace efc */
