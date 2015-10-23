/*
 * EAppender.hh
 *
 *  Created on: 2015-7-27
 *      Author: cxxjava@163.com
 */

#ifndef EAPPENDER_HH_
#define EAPPENDER_HH_

#include "Efc.hh"
#include "ELogEvent.hh"
#include "ELayout.hh"

namespace efc {
namespace log {

interface EAppender : virtual public EObject {
	virtual ~EAppender() {}

	/**
	 Release any resources allocated within the appender such as file
	 handles, network connections, etc.
	 <p>It is a programming error to append to a closed appender.
	 */
	virtual void close() = 0;

	/**
	 Log in <code>Appender</code> specific way. When appropriate,
	 Loggers will call the <code>doAppend</code> method of appender
	 implementations in order to log.
	 */
	virtual void append(ELogEvent& event) = 0;

	/**
	 Get the name of this appender. The name uniquely identifies the
	 appender.
	 */
	virtual const char* getName() = 0;

	/**
	 Set the Layout for this appender.
	 */
	virtual void setLayout(ELayout* layout) = 0;

	/**
	 Returns this appenders layout.
	 */
	virtual ELayout* getLayout() = 0;

	/**
	 * Returns this appenders threshold level. See the {@link
	 * #setThreshold} method for the meaning of this option.
	 */
	virtual ELogger::Level getThreshold() = 0;

	/**
	 Configurators call this method to determine if the appender
	 requires a layout. If this method returns <code>true</code>,
	 meaning that layout is required, then the configurator will
	 configure an layout using the configuration information at its
	 disposal.  If this method returns <code>false</code>, meaning that
	 a layout is not required, then layout configuration will be
	 skipped even if there is available layout configuration
	 information at the disposal of the configurator..

	 <p>In the rather exceptional case, where the appender
	 implementation admits a layout but can also work without it, then
	 the appender should return <code>true</code>.
	 */
	virtual bool requiresLayout() = 0;
};

} /* namespace log */
} /* namespace efc */
#endif /* EAPPENDER_HH_ */
