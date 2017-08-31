/*
 * ELayoutFactory.hh
 *
 *  Created on: 2015-8-2
 *      Author: cxxjava@163.com
 */

#ifndef ELAYOUTFACTORY_HH_
#define ELAYOUTFACTORY_HH_

#include "./ELayout.hh"

namespace efc {
namespace log {

class ELayoutFactory {
public:
	static ELayout* newInstance(EConfig* props, EString& prefix);
};

} /* namespace log */
} /* namespace efc */
#endif /* ELAYOUTFACTORY_HH_ */
