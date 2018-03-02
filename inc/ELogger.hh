/*
 * ELogger.hh
 *
 *  Created on: 2014-6-23
 *      Author: cxxjava@163.com
 */

#ifndef ELOGGER_HH_
#define ELOGGER_HH_

#include "Efc.hh"

namespace efc {
namespace log {

/**
 * The org.slf4j.Logger interface is the main user entry point of SLF4J API.
 * It is expected that logging takes place through concrete implementations
 * of this interface.
 * <p/>
 * <h3>Typical usage pattern:</h3>
 * <pre>
 * import org.slf4j.Logger;
 * import org.slf4j.LoggerFactory;
 *
 * public class Wombat {
 *
 *   <span style="color:green">final static Logger logger = LoggerFactory.getLogger(Wombat.class);</span>
 *   Integer t;
 *   Integer oldT;
 *
 *   public void setTemperature(Integer temperature) {
 *     oldT = t;
 *     t = temperature;
 *     <span style="color:green">logger.debug("Temperature set to {}. Old temperature was {}.", t, oldT);</span>
 *     if(temperature.intValue() > 50) {
 *       <span style="color:green">logger.info("Temperature has risen above 50 degrees.");</span>
 *     }
 *   }
 * }
 * </pre>
 *
 * Be sure to read the FAQ entry relating to <a href="../../../faq.html#logging_performance">parameterized
 * logging</a>. Note that logging statements can be parameterized in
 * <a href="../../../faq.html#paramException">presence of an exception/throwable</a>.
 *
 * <p>Once you are comfortable using loggers, i.e. instances of this interface, consider using
 * <a href="MDC.html">MDC</a> as well as <a href="Marker.html">Markers</a>.</p>
 */

interface ELogger : virtual public EObject {
public:
	enum Level {
		LEVEL_TRACE = 00,
		LEVEL_DEBUG = 10,
		LEVEL_INFO = 20,
		LEVEL_WARN = 30,
		LEVEL_ERROR = 40,
		LEVEL_OFF = 50 //!
	};

	#define ROOT_LOGGER_NAME "ROOT"

public:
	virtual ~ELogger() {}

	/**
	 *
	 */
	virtual void close() = 0;

	/**
	 * Return the name of this <code>Logger</code> instance.
	 * @return name of this logger instance
	 */
	virtual const char* getName() = 0;

	/**
	 * Gets the Level associated with the Logger.
	 *
	 * @return the Level associate with the Logger.
	 */
	virtual Level getLevel() = 0;

	/**
	 * Sets the Level associated with the Logger.
	 *
	 * @param level  the logger Level
	 * @return the old Level associate with the Logger.
	 */
	virtual ELogger::Level setLevel(Level level) = 0;

	/**
	 * Log a message for regular (non-parameterized) log messages.
	 *
	 * @param level   One of the LOG_LEVEL_XXX constants defining the log level
	 * @param message The message itself
	 * @param t       The exception whose stack trace should be logged
	 */
	virtual void log(const char* _file_, int _line_, ELogger::Level level, const char* message, EThrowable* t) = 0;

	/**
	 * Is the logger instance enabled for the TRACE level?
	 *
	 * @return True if this Logger is enabled for the TRACE level,
	 *         false otherwise.
	 * @since 1.4
	 */
	virtual boolean isTraceEnabled() = 0;

	/**
	 * Log a message at the TRACE level according to the specified format
	 * and arguments.
	 *
	 * @param format    the format string
	 * @param arguments a list of 3 or more arguments
	 * @since 1.4
	 */
	virtual void trace(EString message) = 0;
	virtual void trace(const char* _file_, int _line_, EString message) = 0;
	virtual void trace(const char* message) = 0;
	virtual void trace(const char* _file_, int _line_, const char* message) = 0;

	/**
	 * Log an exception (throwable) at the ERROR level with an
	 * accompanying message.
	 *
	 * @param msg the message accompanying the exception
	 * @param t   the exception (throwable) to log
	 */
	virtual void trace(EThrowable& t) = 0;
	virtual void trace(const char* msg, EThrowable& t) = 0;
	virtual void trace(const char* _file_, int _line_, const char* msg, EThrowable& t) = 0;
	virtual void trace(EString msg, EThrowable& t) = 0;
	virtual void trace(const char* _file_, int _line_, EString msg, EThrowable& t) = 0;

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
	virtual void trace_(const char* format, ...) = 0;
	virtual void trace__(const char* _file_, int _line_, const char* format, ...) = 0;

	/**
	 * Is the logger instance enabled for the DEBUG level?
	 *
	 * @return True if this Logger is enabled for the DEBUG level,
	 *         false otherwise.
	 */
	virtual boolean isDebugEnabled() = 0;

	/**
	 * Log a message at the DEBUG level.
	 *
	 * @param message    the logging string
	 */
	virtual void debug(EString message) = 0;
	virtual void debug(const char* _file_, int _line_, EString message) = 0;
	virtual void debug(const char* message) = 0;
	virtual void debug(const char* _file_, int _line_, const char* message) = 0;

	/**
	 * Log an exception (throwable) at the ERROR level with an
	 * accompanying message.
	 *
	 * @param msg the message accompanying the exception
	 * @param t   the exception (throwable) to log
	 */
	virtual void debug(EThrowable& t) = 0;
	virtual void debug(const char* msg, EThrowable& t) = 0;
	virtual void debug(const char* _file_, int _line_, const char* msg, EThrowable& t) = 0;
	virtual void debug(EString msg, EThrowable& t) = 0;
	virtual void debug(const char* _file_, int _line_, EString msg, EThrowable& t) = 0;

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
	virtual void debug_(const char* format, ...) = 0;
	virtual void debug__(const char* _file_, int _line_, const char* format, ...) = 0;

	/**
	 * Is the logger instance enabled for the INFO level?
	 *
	 * @return True if this Logger is enabled for the INFO level,
	 *         false otherwise.
	 */
	virtual boolean isInfoEnabled() = 0;

	/**
	 * Log a message at the INFO level according to the specified format
	 * and arguments.
	 *
	 * @param format    the format string
	 * @param arguments a list of 3 or more arguments
	 */
	virtual void info(EString message) = 0;
	virtual void info(const char* _file_, int _line_, EString message) = 0;
	virtual void info(const char* message) = 0;
	virtual void info(const char* _file_, int _line_, const char* message) = 0;

	/**
	 * Log an exception (throwable) at the ERROR level with an
	 * accompanying message.
	 *
	 * @param msg the message accompanying the exception
	 * @param t   the exception (throwable) to log
	 */
	virtual void info(EThrowable& t) = 0;
	virtual void info(const char* msg, EThrowable& t) = 0;
	virtual void info(const char* _file_, int _line_, const char* msg, EThrowable& t) = 0;
	virtual void info(EString msg, EThrowable& t) = 0;
	virtual void info(const char* _file_, int _line_, EString msg, EThrowable& t) = 0;

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
	virtual void info_(const char* format, ...) = 0;
	virtual void info__(const char* _file_, int _line_, const char* format, ...) = 0;

	/**
	 * Is the logger instance enabled for the WARN level?
	 *
	 * @return True if this Logger is enabled for the WARN level,
	 *         false otherwise.
	 */
	virtual boolean isWarnEnabled() = 0;

	/**
	 * Log a message at the WARN level.
	 *
	 * @param message    the logging string
	 */
	virtual void warn(EString message) = 0;
	virtual void warn(const char* _file_, int _line_, EString message) = 0;
	virtual void warn(const char* message) = 0;
	virtual void warn(const char* _file_, int _line_, const char* message) = 0;

	/**
	 * Log an exception (throwable) at the WARN level with an
	 * accompanying message.
	 *
	 * @param msg the message accompanying the exception
	 * @param t   the exception (throwable) to log
	 */
	virtual void warn(EThrowable& t) = 0;
	virtual void warn(const char* msg, EThrowable& t) = 0;
	virtual void warn(const char* _file_, int _line_, const char* msg, EThrowable& t) = 0;
	virtual void warn(EString msg, EThrowable& t) = 0;
	virtual void warn(const char* _file_, int _line_, EString msg, EThrowable& t) = 0;

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
	virtual void warn_(const char* format, ...) = 0;
	virtual void warn__(const char* _file_, int _line_, const char* format, ...) = 0;

	/**
	 * Is the logger instance enabled for the ERROR level?
	 *
	 * @return True if this Logger is enabled for the ERROR level,
	 *         false otherwise.
	 */
	virtual boolean isErrorEnabled() = 0;

	/**
	 * Log a message at the ERROR level.
	 *
	 * @param message    the logging string
	 */
	virtual void error(EString message) = 0;
	virtual void error(const char* _file_, int _line_, EString message) = 0;
	virtual void error(const char* message) = 0;
	virtual void error(const char* _file_, int _line_, const char* message) = 0;

	/**
	 * Log an exception (throwable) at the ERROR level with an
	 * accompanying message.
	 *
	 * @param msg the message accompanying the exception
	 * @param t   the exception (throwable) to log
	 */
	virtual void error(EThrowable& t) = 0;
	virtual void error(const char* msg, EThrowable& t) = 0;
	virtual void error(const char* _file_, int _line_, const char* msg, EThrowable& t) = 0;
	virtual void error(EString msg, EThrowable& t) = 0;
	virtual void error(const char* _file_, int _line_, EString msg, EThrowable& t) = 0;

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
	virtual void error_(const char* format, ...) = 0;
	virtual void error__(const char* _file_, int _line_, const char* format, ...) = 0;
};

//trace
#define ELOGT(logger, message) { \
	logger->trace(__FILE__, __LINE__, message);}
#define ELOG_T(logger, format, ...) { \
	logger->trace__(__FILE__, __LINE__, format, ##__VA_ARGS__);}
#define ELOG__T(logger, message, throwable) { \
	logger->trace(__FILE__, __LINE__, message, throwable);}

//debug
#define ELOGD(logger, message) { \
	logger->debug(__FILE__, __LINE__, message);}
#define ELOG_D(logger, format, ...) { \
	logger->debug__(__FILE__, __LINE__, format, ##__VA_ARGS__);}
#define ELOG__D(logger, message, throwable) { \
	logger->debug(__FILE__, __LINE__, message, throwable);}

//info
#define ELOGI(logger, message) { \
	logger->info(__FILE__, __LINE__, message);}
#define ELOG_I(logger, format, ...) { \
	logger->info__(__FILE__, __LINE__, format, ##__VA_ARGS__);}
#define ELOG__I(logger, message, throwable) { \
	logger->info(__FILE__, __LINE__, message, throwable);}

//warn
#define ELOGW(logger, message) { \
	logger->warn(__FILE__, __LINE__, message);}
#define ELOG_W(logger, format, ...) { \
	logger->warn__(__FILE__, __LINE__, format, ##__VA_ARGS__);}
#define ELOG__W(logger, message, throwable) { \
	logger->warn(__FILE__, __LINE__, message, throwable);}

//error
#define ELOGE(logger, message) { \
	logger->error(__FILE__, __LINE__, message);}
#define ELOG_E(logger, format, ...) { \
	logger->error__(__FILE__, __LINE__, format, ##__VA_ARGS__);}
#define ELOG__E(logger, message, throwable) { \
	logger->error(__FILE__, __LINE__, message, throwable);}

} /* namespace log */
} /* namespace efc */
#endif /* ELOGGER_HH_ */
