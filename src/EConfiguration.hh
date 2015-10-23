/*
 * EConfigure.hh
 *
 *  Created on: 2015-7-31
 *      Author: cxxjava@163.com
 */

#ifndef ECONFIGURATION_HH_
#define ECONFIGURATION_HH_

#include "Efc.hh"
#include "ELoggerConfig.hh"
#include "EFileLogWriter.hh"
#include "EFileAppender.hh"

namespace efc {
namespace log {

class EConfiguration: public EObject {
public:
	EConfiguration(EArray<EString*>& loggerNames, ELogger::Level threshold);

	EA<EString*> getSortedLoggerNames();
	ELogger::Level getGlobalThreshold();

	void addLoggerConfig(ELoggerConfig* config);

	ELoggerConfig* getParentLoggerConfig(const char* name);
	ELoggerConfig* getLoggerConfig(const char* name, boolean defaultParent=true);

	void addLoggerFile(EFile* file, EFileAppender* fa);
	sp<EFileLogWriter> getLoggerFile(EFile* file);

private:
	/**
	 * Desc sorted logger's name
	 */
	EArray<EString*> loggerNames;

	//
	ELogger::Level threshold;

	//root loggerConfig
	EString rootLoggerName;
	ELoggerConfig* rootLoggerConfig;

	//Add values to hashtable by sorted keys.
	EHashMap<EString*,ELoggerConfig*> loggerConfigs;

	//
	sp<EConcurrentHashMap<EFile, EFileLogWriter> > logfiles;
};

} /* namespace log */
} /* namespace efc */
#endif /* ECONFIGURATION_HH_ */
