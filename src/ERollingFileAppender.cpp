/*
 * ERollingFileAppender.cpp
 *
 *  Created on: 2015-8-23
 *      Author: cxxjava@163.com
 */

#include "ERollingFileAppender.hh"
#include "EOptionConverter.hh"

namespace efc {
namespace log {

ERollingFileAppender::ERollingFileAppender(EConfiguration* conf, EConfig* props, EString& prefix, ELogger::Level threshold) :
	EFileAppender(conf, props, prefix, threshold) {
	EString maxFileSizePrefix = prefix + ".MaxFileSize";
	maxFileSize_ = EOptionConverter::toFileSize(props->getString(maxFileSizePrefix.c_str()), 10*1024*1024L);

	EString maxBackupIndexPrefix = prefix + ".MaxBackupIndex";
	maxBackupIndex_ = props->getInteger(maxBackupIndexPrefix.c_str(), 1);
}

llong ERollingFileAppender::getMaxFileSize() {
	return maxFileSize_;
}

int ERollingFileAppender::getMaxBackupIndex() {
	return maxBackupIndex_;
}

} /* namespace log */
} /* namespace efc */
