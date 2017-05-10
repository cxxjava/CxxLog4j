/*
 * ELoggerConfig.hh
 *
 *  Created on: 2015-7-27
 *      Author: cxxjava@163.com
 */

#ifndef ELOGGERCONFIG_HH_
#define ELOGGERCONFIG_HH_

#include "Efc.hh"
#include "ELogger.hh"
#include "EAppender.hh"

namespace efc {
namespace log {

class EConfiguration;

class ELoggerConfig: public EObject {
public:

	/**
	 * Constructor that sets the name, level and additive values.
	 *
	 * @param conf Current configuration.
	 * @param name The Logger name.
	 * @param level The Level.
	 * @param additive true if the Logger is additive, false otherwise.
	 */
	ELoggerConfig(EConfiguration* conf, const char* name, ELogger::Level level, boolean additive);

	/**
	 *
	 */
	const char* getName();

	/**
	 *
	 */
	ELogger::Level getLevel();

	/**
	 *
	 */
	void setLevel(ELogger::Level level);

	/**
	 *
	 */
	boolean getAdditivity();

	/**
	 *
	 */
	boolean isRootConfig();

	/**
	 * Adds an Appender to the LoggerConfig.
	 *
	 * @param appender The Appender to add.
	 * @param threshold The Level to use.
	 */
	void addAppender(sp<EAppender>& appender);

	/**
	 * Logging and return current min-level.
	 */
	ELogger::Level log(ELogEvent& event);

private:
	EString name;

	EArrayList<sp<EAppender> > appenders;

	EConfiguration* configuration;

	/** The current log level */
	EAtomicInteger /*ELogger::Level*/ currentLogLevel;
	/** The min log threshold at appenders */
	ELogger::Level minThreshold;

	/** The logger additive flag */
	boolean additive;

	/**
	 * Default constructor.
	 */
	ELoggerConfig();

	/**
	 * Is the given log level currently enabled?
	 *
	 * @param logLevel is this level enabled?
	 */
	boolean isLevelEnabled(int logLevel);
};

} /* namespace log */
} /* namespace efc */
#endif /* ELOGGERCONFIG_HH_ */
