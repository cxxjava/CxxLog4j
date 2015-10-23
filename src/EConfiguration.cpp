/*
 * EConfiguration.cpp
 *
 *  Created on: 2015-7-31
 *      Author: cxxjava@163.com
 */

#include "EConfiguration.hh"
#include "ELogger.hh"
#include "ELoggerManagerImp.hh"

namespace efc {
namespace log {

EConfiguration::EConfiguration(EArray<EString*>& ln, ELogger::Level th) :
		loggerNames(ln), threshold(th), rootLoggerName(ROOT_LOGGER_NAME), rootLoggerConfig(null) {
	ECollections::sort(&loggerNames);

	logfiles = new EConcurrentHashMap<EFile, EFileLogWriter>();
}

EA<EString*> EConfiguration::getSortedLoggerNames() {
	return loggerNames.toArray();
}

ELogger::Level EConfiguration::getGlobalThreshold() {
	return threshold;
}

void EConfiguration::addLoggerConfig(ELoggerConfig* config) {
	loggerConfigs.put(new EString(config->getName()), config);
	if (rootLoggerConfig == null && eso_strcmp(config->getName(), ROOT_LOGGER_NAME) == 0) {
		rootLoggerConfig = config;
	}
}

ELoggerConfig* EConfiguration::getParentLoggerConfig(const char* name) {
	if (!name || !*name || eso_strcmp(name, ROOT_LOGGER_NAME) == 0) {
		return null;
	}

	EString s1(name);
	for (int i = loggerNames.size() - 1; i >= 0; i--) {
		EString* s2 = loggerNames[i];
		//printf("name=%s\n", s2->c_str());
		if (s1.startsWith(*s2) && s1.charAt(s2->length()) == '.') {
			return loggerConfigs.get(s2);
		}
	}

	return loggerConfigs.get(&rootLoggerName);
}

ELoggerConfig* EConfiguration::getLoggerConfig(const char* name, boolean defaultParent) {
	EString loggerName(name);
	if (defaultParent) {
		ELoggerConfig* p = loggerConfigs.get(&loggerName);
		if (p) {
			return p;
		}
		else {
			EString s1(name);
			for (int i = loggerNames.size() - 1; i >= 0; i--) {
				EString* s2 = loggerNames[i];
				//printf("name=%s\n", s2->c_str());
				if (s1.startsWith(*s2) && s1.charAt(s2->length()) == '.') {
					return loggerConfigs.get(s2);
				}
			}
			return loggerConfigs.get(&rootLoggerName);
		}
	}
	else {
		return loggerConfigs.get(&loggerName);
	}
}

void EConfiguration::addLoggerFile(EFile* file, EFileAppender* fa) {
	if (!file) {
		return;
	}

	sp<EFile> file_(new EFile(file));
	sp<EConfiguration> configuration = getLoggerManagerImp()->getCurrentConfigure();
	if (configuration != null) {
		sp<EFileLogWriter> os = configuration->logfiles->get(file);
		if (os != null) {
			os->reset(fa);
			logfiles->put(file_, os);
			return;
		}
	}
	logfiles->putIfAbsent(file_, sp<EFileLogWriter>(new EFileLogWriter(file, fa)));
}

sp<EFileLogWriter> EConfiguration::getLoggerFile(EFile* file) {
	if (!file) {
		return null;
	}
	return logfiles->get(file);
}

} /* namespace log */
} /* namespace efc */
