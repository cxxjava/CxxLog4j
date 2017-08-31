/*
 * ELoggerManager.cpp
 *
 *  Created on: 2014-6-23
 *      Author: cxxjava@163.com
 */

#include "./ELoggerManagerImp.hh"
#include "./ELoggerImp.hh"
#include "../inc/ELoggerManager.hh"

namespace efc {
namespace log {

void* ELoggerManager::imp;
boolean ELoggerManager::initialized;
llong ELoggerManager::startTime;

DEFINE_STATIC_INITZZ_BEGIN(ELoggerManager)
	ESystem::_initzz_();
	imp = getLoggerManagerImp();
	initialized = false;
	startTime = ESystem::currentTimeMillis();
DEFINE_STATIC_INITZZ_END

#define IMP static_cast<ELoggerManagerImp*>(imp)

void ELoggerManager::init(const char* config) {
	if (!initialized) {
		IMP->reset(config);
	}
	initialized = true;
}

void ELoggerManager::flushConfig() {
	IMP->flushConfig();
}

sp<ELogger> ELoggerManager::getRootLogger() {
	return getLogger(ROOT_LOGGER_NAME);
}

sp<ELogger> ELoggerManager::getLogger(const char* name) {
	ELoggerManager::_initzz_(); //!

	return IMP->getLogger(name);
}

sp<EConcurrentCollection<ELogger> > ELoggerManager::getLoggers() {
	return IMP->getLoggers();
}

void ELoggerManager::remove(const char* name) {
	IMP->remove(name);
}

llong ELoggerManager::getStartTime() {
	return startTime;
}

} /* namespace log */
} /* namespace efc */
