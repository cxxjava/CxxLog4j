/*
 * EAbstractLayout.cpp
 *
 *  Created on: 2015-8-4
 *      Author: cxxjava@163.com
 */

#include "EAbstractLayout.hh"

namespace efc {
namespace log {

EAbstractLayout::EAbstractLayout(EConfig* props, EString& prefix) {
	name = prefix;
}

const char* EAbstractLayout::getName() {
	return name.c_str();
}

EString EAbstractLayout::getContentType() {
	return "text/plain";
}

EString EAbstractLayout::getHeader() {
	return header.c_str();
}

EString EAbstractLayout::getFooter() {
	return footer.c_str();
}

} /* namespace log */
} /* namespace efc */
