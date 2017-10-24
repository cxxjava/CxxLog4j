/*
 * ELoggerImp.cpp
 *
 *  Created on: 2014-6-23
 *      Author: cxxjava@163.com
 */

#include "./ELoggerImp.hh"
#include "./ELoggerManagerImp.hh"

namespace efc {
namespace log {

#define MYLOG(_file_, _line_, level, format) if (!isLevelEnabled(level)) {return;} \
	es_string_t *str = NULL; \
	va_list args; \
	va_start(args, format); \
	eso_mvsprintf(&str, format, args); \
	va_end(args); \
	log(_file_, _line_, level, str, null); \
	eso_mfree(str);

ELoggerImp::~ELoggerImp() {
}

ELoggerImp::ELoggerImp(const char* name) {
	this->name = name;

	recentLevel = LEVEL_TRACE;
	recentLevelUpdatedTimestamp = 0L;
}

void ELoggerImp::close() {
	ELoggerManager::remove(name.c_str());
}

const char* ELoggerImp::getName() {
	return name.c_str();
}

boolean ELoggerImp::isTraceEnabled() {
	return isLevelEnabled(LEVEL_TRACE);
}

void ELoggerImp::trace(EString& message) {
	log(null, -1, LEVEL_TRACE, message.c_str(), null);
}

void ELoggerImp::trace(EThrowable& t) {
	log(null, -1, LEVEL_TRACE, null, &t);
}

void ELoggerImp::trace(const char* msg, EThrowable& t) {
	log(null, -1, LEVEL_TRACE, msg, &t);
}

boolean ELoggerImp::isDebugEnabled() {
	return isLevelEnabled(LEVEL_DEBUG);
}

void ELoggerImp::debug(EString& message) {
	log(null, -1, LEVEL_DEBUG, message.c_str(), null);
}

void ELoggerImp::debug(EThrowable& t) {
	log(null, -1, LEVEL_DEBUG, null, &t);
}

void ELoggerImp::debug(const char* msg, EThrowable& t) {
	log(null, -1, LEVEL_DEBUG, msg, &t);
}

boolean ELoggerImp::isInfoEnabled() {
	return isLevelEnabled(LEVEL_INFO);
}

void ELoggerImp::info(EString& message) {
	log(null, -1, LEVEL_INFO, message.c_str(), null);
}

void ELoggerImp::info(EThrowable& t) {
	log(null, -1, LEVEL_INFO, null, &t);
}

void ELoggerImp::info(const char* msg, EThrowable& t) {
	log(null, -1, LEVEL_INFO, msg, &t);
}

boolean ELoggerImp::isWarnEnabled() {
	return isLevelEnabled(LEVEL_WARN);
}

void ELoggerImp::warn(EString& message) {
	log(null, -1, LEVEL_WARN, message.c_str(), null);
}

void ELoggerImp::warn(EThrowable& t) {
	log(null, -1, LEVEL_WARN, null, &t);
}

void ELoggerImp::warn(const char* msg, EThrowable& t) {
	log(null, -1, LEVEL_WARN, msg, &t);
}

boolean ELoggerImp::isErrorEnabled() {
	return isLevelEnabled(LEVEL_ERROR);
}

void ELoggerImp::error(EString& message) {
	log(null, -1, LEVEL_ERROR, message.c_str(), null);
}

void ELoggerImp::error(EThrowable& t) {
	log(null, -1, LEVEL_ERROR, null, &t);
}

void ELoggerImp::error(const char* msg, EThrowable& t) {
	log(null, -1, LEVEL_ERROR, msg, &t);
}

void ELoggerImp::trace(const char* _file_, int _line_,
		EString& message) {
	log(_file_, _line_, LEVEL_TRACE, message.c_str(), null);
}

void ELoggerImp::trace(const char* message) {
	log(null, -1, LEVEL_TRACE, message, null);
}

void ELoggerImp::trace(const char* _file_, int _line_,
		const char* message) {
	log(_file_, _line_, LEVEL_TRACE, message, null);
}

void ELoggerImp::trace(const char* _file_, int _line_,
		const char* msg, EThrowable& t) {
	log(_file_, _line_, LEVEL_TRACE, msg, &t);
}

void ELoggerImp::trace_(const char* format, ...) {
	MYLOG(null, -1, LEVEL_TRACE, format);
}

void ELoggerImp::trace__(const char* _file_, int _line_,
		const char* format, ...) {
	MYLOG(_file_, _line_, LEVEL_TRACE, format);
}

void ELoggerImp::debug(const char* _file_, int _line_,
		EString& message) {
	log(_file_, _line_, LEVEL_DEBUG, message.c_str(), null);
}

void ELoggerImp::debug(const char* message) {
	log(null, -1, LEVEL_DEBUG, message, null);
}

void ELoggerImp::debug(const char* _file_, int _line_,
		const char* message) {
	log(_file_, _line_, LEVEL_DEBUG, message, null);
}

void ELoggerImp::debug(const char* _file_, int _line_,
		const char* msg, EThrowable& t) {
	log(_file_, _line_, LEVEL_DEBUG, msg, &t);
}

void ELoggerImp::debug_(const char* format, ...) {
	MYLOG(null, -1, LEVEL_DEBUG, format);
}

void ELoggerImp::debug__(const char* _file_, int _line_,
		const char* format, ...) {
	MYLOG(_file_, _line_, LEVEL_DEBUG, format);
}

void ELoggerImp::info(const char* _file_, int _line_,
		EString& message) {
	log(_file_, _line_, LEVEL_INFO, message.c_str(), null);
}

void ELoggerImp::info(const char* message) {
	log(null, -1, LEVEL_INFO, message, null);
}

void ELoggerImp::info(const char* _file_, int _line_,
		const char* message) {
	log(_file_, _line_, LEVEL_INFO, message, null);
}

void ELoggerImp::info(const char* _file_, int _line_, const char* msg,
		EThrowable& t) {
	log(_file_, _line_, LEVEL_INFO, msg, &t);
}

void ELoggerImp::info_(const char* format, ...) {
	MYLOG(null, -1, LEVEL_INFO, format);
}

void ELoggerImp::info__(const char* _file_, int _line_,
		const char* format, ...) {
	MYLOG(_file_, _line_, LEVEL_INFO, format);
}

void ELoggerImp::warn(const char* _file_, int _line_,
		EString& message) {
	log(_file_, _line_, LEVEL_WARN, message.c_str(), null);
}

void ELoggerImp::warn(const char* message) {
	log(null, -1, LEVEL_WARN, message, null);
}

void ELoggerImp::warn(const char* _file_, int _line_,
		const char* message) {
	log(_file_, _line_, LEVEL_WARN, message, null);
}

void ELoggerImp::warn(const char* _file_, int _line_, const char* msg,
		EThrowable& t) {
	log(_file_, _line_, LEVEL_WARN, msg, &t);
}

void ELoggerImp::warn_(const char* format, ...) {
	MYLOG(null, -1, LEVEL_WARN, format);
}

void ELoggerImp::warn__(const char* _file_, int _line_,
		const char* format, ...) {
	MYLOG(_file_, _line_, LEVEL_WARN, format);
}

void ELoggerImp::error(const char* _file_, int _line_,
		EString& message) {
	log(_file_, _line_, LEVEL_ERROR, message.c_str(), null);
}

void ELoggerImp::error(const char* message) {
	log(null, -1, LEVEL_ERROR, message, null);
}

void ELoggerImp::error(const char* _file_, int _line_,
		const char* message) {
	log(_file_, _line_, LEVEL_ERROR, message, null);
}

void ELoggerImp::error(const char* _file_, int _line_,
		const char* msg, EThrowable& t) {
	log(_file_, _line_, LEVEL_ERROR, msg, &t);
}

void ELoggerImp::error_(const char* format, ...) {
	MYLOG(null, -1, LEVEL_ERROR, format);
}

void ELoggerImp::error__(const char* _file_, int _line_,
		const char* format, ...) {
	MYLOG(_file_, _line_, LEVEL_ERROR, format);
}

boolean ELoggerImp::isLevelEnabled(int logLevel) {
	// log level are numerically ordered so can use simple numeric
	// comparison
	return (logLevel >= getLevel());
}

ELogger::Level ELoggerImp::getLevel() {
	llong ct = ESystem::currentTimeMillis();
	if (ct - recentLevelUpdatedTimestamp > recentLevelUpdateInterval) {
		recentLevelUpdatedTimestamp = ct;

		sp<EConfiguration> configuration = getLoggerManagerImp()->getCurrentConfigure(); //keep it alive!
		ELoggerConfig* config = configuration->getLoggerConfig(name.c_str());
		if (config) {
			recentLevel = config->getLevel();
		}
	}
	return recentLevel;
}

ELogger::Level ELoggerImp::setLevel(ELogger::Level level) {
	ELogger::Level oldLevel = ELogger::LEVEL_OFF;
	sp<EConfiguration> configuration = getLoggerManagerImp()->getCurrentConfigure(); //keep it alive!
	ELoggerConfig* config = configuration->getLoggerConfig(name.c_str());
	if (config) {
		oldLevel = config->getLevel();
		config->setLevel(level);
	}
	return oldLevel;
}

boolean ELoggerImp::equals(EObject* o) {
	ELoggerImp* obj = dynamic_cast<ELoggerImp*>(obj);
	return obj ?
			(((void*) this == (void*) obj) ?
					true : (this->name.equals(obj->name) ? true : false )) :
			false ;
}

int ELoggerImp::hashCode()
{
	return name.hashCode();
}

void ELoggerImp::log(const char* _file_, int _line_, ELogger::Level level, const char* message, EThrowable* t) {
	ELogEvent event(name.c_str(), level, message, t, _file_, _line_);

	sp<EConfiguration> configuration = getLoggerManagerImp()->getCurrentConfigure(); //keep it alive!
	ELoggerConfig* config = configuration->getLoggerConfig(name.c_str());
	if (config) {
		recentLevel = config->log(event);
		recentLevelUpdatedTimestamp = ESystem::currentTimeMillis();
	} else {
		fprintf(stderr, "Can't get ELoggerConfig Object!\n");
	}
}

} /* namespace log */
} /* namespace efc */
