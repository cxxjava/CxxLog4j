/*
 * EAppenderFactory.hh
 *
 *  Created on: 2015-8-2
 *      Author: cxxjava@163.com
 */

#ifndef EAPPENDERFACTORY_HH_
#define EAPPENDERFACTORY_HH_

#include "EAppender.hh"
#include "EConfiguration.hh"

namespace efc {
namespace log {

class EAppenderFactory {
public:
	static EAppender* newInstance(EConfiguration* conf, EConfig* props, EString& prefix, sp<EConfiguration>& configure);
};

} /* namespace log */
} /* namespace efc */
#endif /* EAPPENDERFACTORY_HH_ */
