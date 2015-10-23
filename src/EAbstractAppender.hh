/*
 * EAbstractAppender.hh
 *
 *  Created on: 2015-7-28
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTAPPENDER_HH_
#define EABSTRACTAPPENDER_HH_

#include "Efc.hh"
#include "EAppender.hh"
#include "ELayout.hh"

namespace efc {
namespace log {

class EConfiguration;

class EAbstractAppender: public EAppender {
public:
	virtual ~EAbstractAppender();

	/**
	 * Returns the name of the Appender.
	 * @return The name of the Appender.
	 */
	const char* getName();

	/**
	 * Returns the Layout for the appender.
	 * @return The Layout used to format the event.
	 */
	ELayout* getLayout();

	/**
	 * Returns this appenders threshold level. See the {@link
	 * #setThreshold} method for the meaning of this option.
	 */
	ELogger::Level getThreshold();

protected:
	EConfig* props;
	EString name;
	ELayout* layout;
	ELogger::Level threshold;
	EConfiguration* configuration;

	EAbstractAppender(EConfiguration* conf, EConfig* props, EString& prefix, ELogger::Level threshold);
};

} /* namespace log */
} /* namespace efc */
#endif /* EABSTRACTAPPENDER_HH_ */
