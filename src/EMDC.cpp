/*
 * EMDC.cpp
 *
 *  Created on: 2015-8-9
 *      Author: cxxjava@163.com
 */

#include "../inc/EMDC.hh"
#include "../inc/ELoggerManager.hh"

namespace efc {
namespace log {

namespace mdc {

class ThreadLocal : public EThreadLocal {
public:
	virtual EObject* initialValue() {
		return new EHashMap<EString*, EString*>();
	}
};
} //

EThreadLocalVariable<mdc::ThreadLocal, EHashMap<EString*, EString*> >* EMDC::localObj;

DEFINE_STATIC_INITZZ_BEGIN(EMDC)
	ESystem::_initzz_();
	localObj = getLocalObj();
DEFINE_STATIC_INITZZ_END

EThreadLocalVariable<mdc::ThreadLocal, EHashMap<EString*, EString*> >* EMDC::getLocalObj() {
	static EThreadLocalVariable<mdc::ThreadLocal, EHashMap<EString*, EString*> > gLocalObj;
	localObj = &gLocalObj;
	return localObj;
}

EMDC::EMDC(const char* key, const char* value) {
	this->key = key;
	put(key, value);
}

EMDC::~EMDC() {
	remove(this->key.c_str());
}

void EMDC::clear() {
	_initzz_();
	localObj->get()->clear();
}

const char* EMDC::get(const char* key) {
	_initzz_();
	EString k(key);
	EString* v = localObj->get()->get(&k);
	if (v) {
		return v->c_str();
	}
	return NULL;
}

EHashMap<EString*, EString*>* EMDC::get() {
	return localObj->get();
}

void EMDC::put(const char* key, const char* value) {
	_initzz_();
	delete localObj->get()->put(new EString(key), new EString(value));
}

void EMDC::remove(const char* key) {
	_initzz_();
	EString k(key);
	delete localObj->get()->remove(&k);
}

} /* namespace log */
} /* namespace efc */
