/*
 * EConsoleAppender.hh
 *
 *  Created on: 2015-8-2
 *      Author: cxxjava@163.com
 */

#ifndef ECONSOLEAPPENDER_HH_
#define ECONSOLEAPPENDER_HH_

#include "EAbstractAppender.hh"

namespace efc {
namespace log {

class EConsoleAppender: public EAbstractAppender {
public:
	EConsoleAppender(EConfiguration* conf, EConfig* props, EString& prefix, ELogger::Level threshold);

	virtual void close();
	virtual void append(ELogEvent& event);
	virtual void setLayout(ELayout* layout);
	virtual bool requiresLayout();

protected:
	boolean logToStdErr;
};

} /* namespace log */
} /* namespace efc */
#endif /* ECONSOLEAPPENDER_HH_ */
