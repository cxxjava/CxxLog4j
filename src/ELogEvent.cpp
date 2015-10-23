/*
 * ELogEvent.cpp
 *
 *  Created on: 2015-7-27
 *      Author: cxxjava@163.com
 */

#include "ELogEvent.hh"
#include "EMDC.hh"
#include "ENDC.hh"

namespace efc {
namespace log {

ELogEvent::~ELogEvent() {
}

ELogEvent::ELogEvent(const char* logger_, ELogger::Level level_,
		const char* message_, EThrowable* t_, const char* file_, int line_) :
		logger(logger_),
		level(level_),
		message(message_),
		throwable(t_),
		_file_(file_),
		_line_(line_) {
	timeStamp = ESystem::currentTimeMillis();
	timeZone = ESystem::localTimeZone();
	threadName = EThread::currentThread()->getName();
}

ELogger::Level ELogEvent::getLevel() {
	return level;
}

const char* ELogEvent::getLoggerName() {
	return logger;
}

const char* ELogEvent::getMessage() {
	return message;
}

EString ELogEvent::getNDC() {
	return ENDC::get();
}

const char* ELogEvent::getMDC(const char* key) {
	return EMDC::get(key);
}

EHashMap<EString*, EString*>* ELogEvent::getMDC() {
	return EMDC::get();
}

llong ELogEvent::getTimeMillis() {
	return timeStamp;
}

int ELogEvent::getTimeZone() {
	return timeZone;
}

const char* ELogEvent::getThreadName() {
	return threadName;
}

const char* ELogEvent::getFileName() {
	return _file_ ? eso_filepath_name_get(_file_) : "";
}

const int ELogEvent::getLineNumber() {
	return _line_;
}

EString ELogEvent::getThrowableStr() {
	return throwable ? throwable->toString() : null;
}

} /* namespace log */
} /* namespace efc */
