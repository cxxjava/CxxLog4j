/*
 * EFileAppender.cpp
 *
 *  Created on: 2015-8-15
 *      Author: cxxjava@163.com
 */

#include "./EFileAppender.hh"
#include "./ELoggerManagerImp.hh"
#include "./EOptionConverter.hh"

namespace efc {
namespace log {

EFileAppender::~EFileAppender() {
	delete file_;
}

EFileAppender::EFileAppender(EConfiguration* conf, EConfig* props, EString& prefix, ELogger::Level threshold) :
		EAbstractAppender(conf, props, prefix, threshold), file_(null) {
	EString filePrefix = prefix + ".File";
	EString fileName = EOptionConverter::findAndSubst(filePrefix.c_str(), props);
	if (!fileName.isEmpty()) {
		file_ = new EFile(fileName.c_str());
	}

	EString appendPrefix = prefix + ".Append";
	append_ = props->getBoolean(appendPrefix.c_str(), true);
	EString bufferedIOPrefix = prefix + ".BufferedIO";
	bufferedIO_ = props->getBoolean(bufferedIOPrefix.c_str(), false);
	EString bufferSizePrefix = prefix + ".BufferSize";
	bufferSize_ = props->getInt(bufferSizePrefix.c_str(), 8192); //8KB
}

void EFileAppender::close() {
	SYNCHRONIZED(this) {
		delete file_;
		file_ = null;
    }}
}

void EFileAppender::append(ELogEvent& event) {
	if (event.getLevel() >= getThreshold()) {
		EString s = layout->format(event);

		sp<EFileLogWriter> os = configuration->getLoggerFile(file_);
		if (os != null) {
			os->write(s.c_str(), s.length());
		}
	}
}

void EFileAppender::setLayout(ELayout* layout) {
	this->layout = layout;
}

boolean EFileAppender::requiresLayout() {
	return true;
}

EFile* EFileAppender::getFile() {
	return file_;
}

boolean EFileAppender::fileIsAppend() {
	return append_;
}

boolean EFileAppender::fileIsBufferedIO() {
	return bufferedIO_;
}

int EFileAppender::getFileBufferSize() {
	return bufferSize_;
}

} /* namespace log */
} /* namespace efc */
