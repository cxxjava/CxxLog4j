/*
 * EDailyRollingFileAppender.cpp
 *
 *  Created on: 2015-8-23
 *      Author: cxxjava@163.com
 */

#include "./EDailyRollingFileAppender.hh"

namespace efc {
namespace log {

EDailyRollingFileAppender::EDailyRollingFileAppender(EConfiguration* conf, EConfig* props, EString& prefix, ELogger::Level threshold) :
	ERollingFileAppender(conf, props, prefix, threshold) {
	EString datePatternPrefix = prefix + ".DatePattern";
	datePattern_ = props->getString(datePatternPrefix.c_str(), "'.'yyyy-MM-dd");
}

EString& EDailyRollingFileAppender::getDatePattern() {
	return datePattern_;
}

} /* namespace log */
} /* namespace efc */
