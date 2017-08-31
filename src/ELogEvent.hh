/*
 * ELogEvent.hh
 *
 *  Created on: 2015-7-23
 *      Author: cxxjava@163.com
 */

#ifndef ELOGEVENT_HH_
#define ELOGEVENT_HH_

#include "../inc/ELogger.hh"

namespace efc {
namespace log {

class ELogEvent : public EObject {
public:
	virtual ~ELogEvent();

	ELogEvent(const char* logger, ELogger::Level level, const char* message, EThrowable* t, const char* _file_, int _line_);

	/**
     * Gets the level.
     *
     * @return level.
     */
	ELogger::Level getLevel();

	/**
	 * Gets the logger name.
	 *
	 * @return logger name, may be {@code null}.
	 */
	const char* getLoggerName();

	/**
	 * Gets the message associated with the event.
	 *
	 * @return message.
	 */
	const char* getMessage();

	/**
	 * This method returns the NDC for this event. It will return the
	 * correct content even if the event was generated in a different
	 * thread or even on a different machine. The {@link NDC#get} method
	 * should <em>never</em> be called directly.  */
	EString getNDC();

	/**
	  Returns the the context corresponding to the <code>key</code>
	  parameter. If there is a local MDC copy, possibly because we are
	  in a logging server or running inside AsyncAppender, then we
	  search for the key in MDC copy, if a value is found it is
	  returned. Otherwise, if the search in MDC copy returns a null
	  result, then the current thread's <code>MDC</code> is used.

	  <p>Note that <em>both</em> the local MDC copy and the current
	  thread's MDC are searched.
    */
	const char* getMDC(const char* key);
	EHashMap<EString*, EString*>* getMDC();

	/**
	 * Gets event time in milliseconds since midnight, January 1, 1970 UTC.
	 *
	 * @return milliseconds since midnight, January 1, 1970 UTC.
	 * @see java.lang.System#currentTimeMillis()
	 */
	llong getTimeMillis();
	int getTimeZone();

	/**
	 * Gets thread name.
	 *
	 * @return thread name, may be null.
	 */
	const char* getThreadName();

	const char* getFileName();
	const int getLineNumber();

	/**
	 * Return this event's throwable's string.
	 */
	EString getThrowableStr();

private:
	/** the logger name. */
	const char* logger;
	/** level of logging event. */
	ELogger::Level level;
	/** message of logging event. */
	const char* message;
	/** throwable of logging event. */
	EThrowable* throwable;

	/** The number of milliseconds elapsed from 1/1/1970 until logging event was created. */
	llong timeStamp;
	int timeZone;

	/** The identifier of thread in which this logging event was generated. */
	const char* threadName;

	const char* _file_;
	int   _line_;
};

} /* namespace log */
} /* namespace efc */
#endif /* ELOGEVENT_HH_ */
