/*
 * ERollingFileAppender.hh
 *
 *  Created on: 2015-8-23
 *      Author: cxxjava@163.com
 */

#ifndef EROLLINGFILEAPPENDER_HH_
#define EROLLINGFILEAPPENDER_HH_

#include "EFileAppender.hh"

namespace efc {
namespace log {

/**
   RollingFileAppender extends FileAppender to backup the log files when
   they reach a certain size.

   The log4j extras companion includes alternatives which should be considered
   for new deployments and which are discussed in the documentation
   for org.apache.log4j.rolling.RollingFileAppender.
*/

class ERollingFileAppender: public EFileAppender {
public:
	ERollingFileAppender(EConfiguration* conf, EConfig* props, EString& prefix, ELogger::Level threshold);

	llong getMaxFileSize();
	int getMaxBackupIndex();

private:
	llong maxFileSize_;
	int maxBackupIndex_;
};

} /* namespace log */
} /* namespace efc */
#endif /* EROLLINGFILEAPPENDER_HH_ */
