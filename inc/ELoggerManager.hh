/*
 * ELoggerManager.hh
 *
 *  Created on: 2014-6-23
 *      Author: cxxjava@163.com
 */

#ifndef ELOGGERMANAGER_HH_
#define ELOGGERMANAGER_HH_

#include "./ELogger.hh"

namespace efc {
namespace log {

class ELoggerManager {
public:
	DECLARE_STATIC_INITZZ;

public:
	/**
	 * Logger initialize.
	 * @param config the log config file.
	 */
	static void init(const char* config) THROWS(EIOException);

	/**
	 * Manual flush configuration.
	 */
	static void flushConfig();

	/**
	 * Returns the root logger.
	 *
	 * @return the root logger, named {@link #ROOT_LOGGER_NAME}.
	 */
	static sp<ELogger> getRootLogger();

	/**
	 * Return a logger named according to the name parameter using the statically
	 * bound {@link ILoggerFactory} instance.
	 *
	 * @param name The name of the logger.
	 * @return logger
	 */
	static sp<ELogger> getLogger(const char* name);

	/**
	 * Get all loggers.
	 */
	static sp<EConcurrentCollection<ELogger> > getLoggers();

	/**
	 * Remove a logger.
	 */
	static void remove(const char* name);

	/**
	 * Returns the time when the application started, in milliseconds
	 * elapsed since 01.01.1970.
	 */
	static llong getStartTime();

private:
	ELoggerManager(){};

	static void* imp;
	static boolean initialized;

	static llong startTime;
};

} /* namespace log */
} /* namespace efc */
#endif /* ELOGGERMANAGER_HH_ */
