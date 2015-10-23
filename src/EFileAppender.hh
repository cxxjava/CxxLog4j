/*
 * EFileAppender.hh
 *
 *  Created on: 2015-8-15
 *      Author: cxxjava@163.com
 */

#ifndef EFILEAPPENDER_HH_
#define EFILEAPPENDER_HH_

#include "EAbstractAppender.hh"

namespace efc {
namespace log {

/**
 *  FileAppender appends log events to a file.
 *
 *  <p>Support for <code>java.io.Writer</code> and console appending
 *  has been deprecated and then removed. See the replacement
 *  solutions: {@link WriterAppender} and {@link ConsoleAppender}.
 *
 */

class EFileAppender: public EAbstractAppender, public ESynchronizeable {
public:
	virtual ~EFileAppender();
	EFileAppender(EConfiguration* conf, EConfig* props, EString& prefix, ELogger::Level threshold);

	virtual void close();
	virtual void append(ELogEvent& event);
	virtual void setLayout(ELayout* layout);
	virtual bool requiresLayout();

	EFile* getFile();
	boolean fileIsAppend();
	boolean fileIsBufferedIO();
	int getFileBufferSize();

protected:
	EFile *file_;
	boolean append_;
	boolean bufferedIO_;
	int bufferSize_;
};

} /* namespace log */
} /* namespace efc */
#endif /* EFILEAPPENDER_HH_ */
