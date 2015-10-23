/*
 * EConsoleAppender.cpp
 *
 *  Created on: 2015-8-2
 *      Author: cxxjava@163.com
 */

#include "EConsoleAppender.hh"

namespace efc {
namespace log {

EConsoleAppender::EConsoleAppender(EConfiguration* conf, EConfig* props, EString& prefix, ELogger::Level threshold) :
		EAbstractAppender(conf, props, prefix, threshold), logToStdErr(false) {
	EString targetPrefix = prefix + ".Target";
	const char* targetStr = props->getString(targetPrefix.c_str());
	if (targetStr
			&& (eso_strcasecmp(targetStr, "System.err") == 0
					|| eso_strcasecmp(targetStr, "stderr") == 0)) {
		logToStdErr = true;
	}
}

void EConsoleAppender::close() {
}

void EConsoleAppender::append(ELogEvent& event) {
	if (event.getLevel() >= getThreshold()) {
		EString s = layout->format(event);
		if (logToStdErr) {
			ESystem::err->print(s.c_str());
		}
		else {
			ESystem::out->print(s.c_str());
		}
	}
}

void EConsoleAppender::setLayout(ELayout* layout) {
	this->layout = layout;
}

bool EConsoleAppender::requiresLayout() {
	return true;
}

} /* namespace log */
} /* namespace efc */
