/*
 * EAbstractAppender.cpp
 *
 *  Created on: 2015-7-28
 *      Author: cxxjava@163.com
 */

#include "EAbstractAppender.hh"
#include "EConfiguration.hh"

namespace efc {
namespace log {

EAbstractAppender::~EAbstractAppender() {
	delete layout;
}

const char* EAbstractAppender::getName() {
	return name.c_str();
}

ELayout* EAbstractAppender::getLayout() {
	return layout;
}

ELogger::Level EAbstractAppender::getThreshold() {
	return threshold;
}

EAbstractAppender::EAbstractAppender(EConfiguration* conf, EConfig* props, EString& prefix,
		ELogger::Level threshold) {
	this->props = props;
	this->name = prefix;
	this->layout = null;
	this->threshold = threshold;
	this->configuration = conf;
}

} /* namespace log */
} /* namespace efc */
