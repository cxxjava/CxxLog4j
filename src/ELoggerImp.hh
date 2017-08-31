/*
 * ELoggerImp.hh
 *
 *  Created on: 2014-6-23
 *      Author: cxxjava@163.com
 */

#ifndef ELOGGERIMP_HH_
#define ELOGGERIMP_HH_

#include "ESharedPtr.hh"
#include "./ELoggerConfig.hh"
#include "../inc/ELogger.hh"

namespace efc {
namespace log {

class ELoggerImp: public ELogger {
public:
	virtual ~ELoggerImp();

	ELoggerImp(const char* name);

	/**
	 *
	 */
	void close();

	/**
	 * Get the logger name.
	 */
	virtual const char* getName();

	/**
	 * Gets the Level associated with the Logger.
	 *
	 * @return the Level associate with the Logger.
	 */
	virtual ELogger::Level getLevel();

	/**
	 * Sets the Level associated with the Logger.
	 *
	 * @param level  the logger Level
	 * @return the old Level associate with the Logger.
	 */
	virtual ELogger::Level setLevel(ELogger::Level level);

	/**
	 * Is the logger instance enabled for the TRACE level?
	 *
	 * @return True if this Logger is enabled for the TRACE level,
	 *         false otherwise.
	 * @since 1.4
	 */
	boolean isTraceEnabled();

	/**
	 * Log a message at the TRACE level according to the specified format
	 * and arguments.
	 *
	 * @param format    the format string
	 * @param arguments a list of 3 or more arguments
	 * @since 1.4
	 */
	void trace(EString& message);
	void trace(const char* _file_, int _line_, EString& message);
	void trace(const char* message);
	void trace(const char* _file_, int _line_, const char* message);

	/**
	 * Log an exception (throwable) at the ERROR level with an
	 * accompanying message.
	 *
	 * @param msg the message accompanying the exception
	 * @param t   the exception (throwable) to log
	 */
	void trace(EThrowable& t);
	void trace(const char* msg, EThrowable& t);
	void trace(const char* _file_, int _line_, const char* msg, EThrowable& t);

	/**
	 * Log a message at the DEBUG level according to the specified format
	 * and arguments.
	 * <p/>
	 * <p>This form avoids superfluous string concatenation when the logger
	 * is disabled for the TRACE level. However, this variant incurs the hidden
	 * (and relatively small) cost of creating an <code>Object[]</code> before invoking the method,
	 * even if this logger is disabled for TRACE. The variants taking {@link #trace(String, Object) one} and
	 * {@link #trace(String, Object, Object) two} arguments exist solely in order to avoid this hidden cost.</p>
	 *
	 * @param format    the format string
	 * @param arguments a list of 3 or more arguments
	 */
	void trace_(const char* format, ...);
	void trace__(const char* _file_, int _line_, const char* format, ...);

	/**
	 * Is the logger instance enabled for the DEBUG level?
	 *
	 * @return True if this Logger is enabled for the DEBUG level,
	 *         false otherwise.
	 */
	boolean isDebugEnabled();

	/**
	 * Log a message at the DEBUG level.
	 *
	 * @param message    the logging string
	 */
	void debug(EString& message);
	void debug(const char* _file_, int _line_, EString& message);
	void debug(const char* message);
	void debug(const char* _file_, int _line_, const char* message);

	/**
	 * Log an exception (throwable) at the ERROR level with an
	 * accompanying message.
	 *
	 * @param msg the message accompanying the exception
	 * @param t   the exception (throwable) to log
	 */
	void debug(EThrowable& t);
	void debug(const char* msg, EThrowable& t);
	void debug(const char* _file_, int _line_, const char* msg, EThrowable& t);

	/**
	 * Log a message at the DEBUG level according to the specified format
	 * and arguments.
	 * <p/>
	 * <p>This form avoids superfluous string concatenation when the logger
	 * is disabled for the ERROR level. However, this variant incurs the hidden
	 * (and relatively small) cost of creating an <code>Object[]</code> before invoking the method,
	 * even if this logger is disabled for DEBUG. The variants taking
	 * {@link #debug(String, Object) one} and {@link #debug(String, Object, Object) two}
	 * arguments exist solely in order to avoid this hidden cost.</p>
	 *
	 * @param format    the format string
	 * @param arguments a list of 3 or more arguments
	 */
	void debug_(const char* format, ...);
	void debug__(const char* _file_, int _line_, const char* format, ...);

	/**
	 * Is the logger instance enabled for the INFO level?
	 *
	 * @return True if this Logger is enabled for the INFO level,
	 *         false otherwise.
	 */
	boolean isInfoEnabled();

	/**
	 * Log a message at the INFO level according to the specified format
	 * and arguments.
	 *
	 * @param format    the format string
	 * @param arguments a list of 3 or more arguments
	 */
	void info(EString& message);
	void info(const char* _file_, int _line_, EString& message);
	void info(const char* message);
	void info(const char* _file_, int _line_, const char* message);

	/**
	 * Log an exception (throwable) at the ERROR level with an
	 * accompanying message.
	 *
	 * @param msg the message accompanying the exception
	 * @param t   the exception (throwable) to log
	 */
	void info(EThrowable& t);
	void info(const char* msg, EThrowable& t);
	void info(const char* _file_, int _line_, const char* msg, EThrowable& t);

	/**
	 * Log a message at the ERROR level according to the specified format
	 * and arguments.
	 * <p/>
	 * <p>This form avoids superfluous string concatenation when the logger
	 * is disabled for the INFO level. However, this variant incurs the hidden
	 * (and relatively small) cost of creating an <code>Object[]</code> before invoking the method,
	 * even if this logger is disabled for INFO. The variants taking
	 * {@link #info(String, Object) one} and {@link #info(String, Object, Object) two}
	 * arguments exist solely in order to avoid this hidden cost.</p>
	 *
	 * @param format    the format string
	 * @param arguments a list of 3 or more arguments
	 */
	void info_(const char* format, ...);
	void info__(const char* _file_, int _line_, const char* format, ...);

	/**
	 * Is the logger instance enabled for the WARN level?
	 *
	 * @return True if this Logger is enabled for the WARN level,
	 *         false otherwise.
	 */
	boolean isWarnEnabled();

	/**
	 * Log a message at the WARN level.
	 *
	 * @param message    the logging string
	 */
	void warn(EString& message);
	void warn(const char* _file_, int _line_, EString& message);
	void warn(const char* message);
	void warn(const char* _file_, int _line_, const char* message);

	/**
	 * Log an exception (throwable) at the WARN level with an
	 * accompanying message.
	 *
	 * @param msg the message accompanying the exception
	 * @param t   the exception (throwable) to log
	 */
	void warn(EThrowable& t);
	void warn(const char* msg, EThrowable& t);
	void warn(const char* _file_, int _line_, const char* msg, EThrowable& t);

	/**
	 * Log a message at the WARN level according to the specified format
	 * and arguments.
	 * <p/>
	 * <p>This form avoids superfluous string concatenation when the logger
	 * is disabled for the WARN level. However, this variant incurs the hidden
	 * (and relatively small) cost of creating an <code>Object[]</code> before invoking the method,
	 * even if this logger is disabled for WARN. The variants taking
	 * {@link #warn(String, Object) one} and {@link #warn(String, Object, Object) two}
	 * arguments exist solely in order to avoid this hidden cost.</p>
	 *
	 * @param format    the format string
	 * @param arguments a list of 3 or more arguments
	 */
	void warn_(const char* format, ...);
	void warn__(const char* _file_, int _line_, const char* format, ...);

	/**
	 * Is the logger instance enabled for the ERROR level?
	 *
	 * @return True if this Logger is enabled for the ERROR level,
	 *         false otherwise.
	 */
	boolean isErrorEnabled();

	/**
	 * Log a message at the ERROR level.
	 *
	 * @param message    the logging string
	 */
	void error(EString& message);
	void error(const char* _file_, int _line_, EString& message);
	void error(const char* message);
	void error(const char* _file_, int _line_, const char* message);

	/**
	 * Log an exception (throwable) at the ERROR level with an
	 * accompanying message.
	 *
	 * @param msg the message accompanying the exception
	 * @param t   the exception (throwable) to log
	 */
	void error(EThrowable& t);
	void error(const char* msg, EThrowable& t);
	void error(const char* _file_, int _line_, const char* msg, EThrowable& t);

	/**
	 * Log a message at the ERROR level according to the specified format
	 * and arguments.
	 * <p/>
	 * <p>This form avoids superfluous string concatenation when the logger
	 * is disabled for the ERROR level. However, this variant incurs the hidden
	 * (and relatively small) cost of creating an <code>Object[]</code> before invoking the method,
	 * even if this logger is disabled for ERROR. The variants taking
	 * {@link #error(String, Object) one} and {@link #error(String, Object, Object) two}
	 * arguments exist solely in order to avoid this hidden cost.</p>
	 *
	 * @param format    the format string
	 * @param arguments a list of 3 or more arguments
	 */
	void error_(const char* format, ...);
	void error__(const char* _file_, int _line_, const char* format, ...);

	/**
	 *
	 */
	virtual boolean equals(EObject* obj);
	virtual int hashCode();

protected:
	EString name;

	/** The mirror of log level to config */
	ELogger::Level recentLevel;
	llong recentLevelUpdatedTimestamp;
	static const int recentLevelUpdateInterval = 3000; //ms

	/**
	 * Is the given log level currently enabled?
	 *
	 * @param logLevel is this level enabled?
	 */
	boolean isLevelEnabled(int logLevel);

	/**
	 * This is our internal implementation for logging regular (non-parameterized)
	 * log messages.
	 *
	 * @param level   One of the LOG_LEVEL_XXX constants defining the log level
	 * @param message The message itself
	 * @param t       The exception whose stack trace should be logged
	 */
	void log(const char* _file_, int _line_, ELogger::Level level, const char* message, EThrowable* t);
};

} /* namespace log */
} /* namespace efc */
#endif /* ELOGGERIMP_HH_ */
