/*
 * ENDC.cpp
 *
 *  Created on: 2015-8-9
 *      Author: cxxjava@163.com
 */

#include "ENDC.hh"
#include "ELoggerManager.hh"

namespace efc {
namespace log {

namespace ndc {

class StackSizable : public ELinkedList<EString*> {
public:
    StackSizable() : maxSize(0) {
    }

    virtual void push(EString* elt) {
    	ELinkedList<EString*>::push(elt);
    	if (maxSize > 0) {
			while (size() > maxSize) {
				delete removeLast();
			}
    	}
    }

    void setMaxDepth(int maxDepth) {
    	this->maxSize = maxDepth;
    	if (maxSize > 0) {
			while (size() > maxSize) {
				delete removeLast();
			}
		}
    }

private:
    int maxSize;
};

class ThreadLocal : public EThreadLocal {
public:
	virtual EObject* initialValue() {
		return new StackSizable();
	}
};
} //

EThreadLocalVariable<ndc::ThreadLocal, ndc::StackSizable>* ENDC::localObj;

DEFINE_STATIC_INITZZ_BEGIN(ENDC)
	ESystem::_initzz_();
	printf("ENDC::initzz()\n");
	localObj = getLocalObj();
DEFINE_STATIC_INITZZ_END

EThreadLocalVariable<ndc::ThreadLocal, ndc::StackSizable>* ENDC::getLocalObj() {
	static EThreadLocalVariable<ndc::ThreadLocal, ndc::StackSizable> gLocalObj;
	localObj = &gLocalObj;
	return localObj;
}

ENDC::ENDC(const char* message) {
	push(new EString(message));
}

ENDC::~ENDC() {
	delete pop();
}

void ENDC::clear() {
	_initzz_();
	localObj->get()->clear();
}

EString ENDC::get() {
	_initzz_();

	EString output;

	ndc::StackSizable* ss = localObj->get();
	EIterator<EString*> *iter = localObj->get()->iterator();
	while (iter->hasNext()) {
		EString* s = iter->next();
		if (s) {
			if (output.length() > 0) {
				output.append(' ');
			}
			output.append(s->c_str());
		}
	}
	delete iter;
	return output;
}

int ENDC::getDepth() {
	_initzz_();
	return localObj->get()->size();
}

EString* ENDC::pop() {
	_initzz_();
	return localObj->get()->pop();
}

EString* ENDC::peek() {
	_initzz_();
	return localObj->get()->peek();
}

void ENDC::push(EString* message) {
	_initzz_();
	localObj->get()->push(message);
}

void ENDC::remove() {
	_initzz_();
	delete localObj->set(null);
}

void ENDC::setMaxDepth(int maxDepth) {
	_initzz_();
	localObj->get()->setMaxDepth(maxDepth);
}

} /* namespace log */
} /* namespace efc */
