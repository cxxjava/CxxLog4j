/*
 * ESimpleLayout.cpp
 *
 *  Created on: 2015-8-5
 *      Author: cxxjava@163.com
 */

#include "ESimpleLayout.hh"
#include "ETransform.hh"

namespace efc {
namespace log {

ESimpleLayout::ESimpleLayout(EConfig* props, EString& prefix) :
		EAbstractLayout(props, prefix) {
}

EString ESimpleLayout::format(ELogEvent& event) {
	EString sbuf;
	sbuf.append(ETransform::toLevelStr(event.getLevel()));
	sbuf.append(" - ");
	sbuf.append(event.getMessage());
	sbuf.append(ES_LINE_SEPARATOR);
	return sbuf;
}

} /* namespace log */
} /* namespace efc */
