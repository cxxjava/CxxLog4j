/*
 * EFileLogWriter.hh
 *
 *  Created on: 2015-8-23
 *      Author: cxxjava@163.com
 */

#ifndef EFILELOGWRITER_HH_
#define EFILELOGWRITER_HH_

#include "Efc.hh"
#include "EFileAppender.hh"

namespace efc {
namespace log {

namespace flw {
	class FileLogWriterImp;
}

class EFileLogWriter: public ESynchronizeable {
public:
	virtual ~EFileLogWriter();

	EFileLogWriter(EFile* file, EFileAppender* fa);

	void reset(EFileAppender* fa);

	void write(const void* data, int len);

protected:
	enum {
		FILE_APPENDER,
		ROLLING_FILE_APPENDER,
		DAILY_ROLLING_FILE_APPENDER
	} emType;

	EFile* ofile;
	flw::FileLogWriterImp* imp;
};

} /* namespace log */
} /* namespace efc */
#endif /* EFILELOGWRITER_HH_ */
