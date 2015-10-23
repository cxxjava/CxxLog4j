/*
 * ESimpleLayout.hh
 *
 *  Created on: 2015-8-5
 *      Author: cxxjava@163.com
 */

#ifndef ESIMPLELAYOUT_HH_
#define ESIMPLELAYOUT_HH_

#include "EAbstractLayout.hh"

namespace efc {
namespace log {

/**
   SimpleLayout consists of the level of the log statement,
   followed by " - " and then the log message itself. For example,

   <pre>
           DEBUG - Hello world
   </pre>

   <p>
   @author Ceki G&uuml;lc&uuml;
   @since version 0.7.0

   <p>{@link PatternLayout} offers a much more powerful alternative.
*/

class ESimpleLayout: public EAbstractLayout {
public:
	ESimpleLayout(EConfig* props, EString& prefix);

	/**
	 * Produces a formatted string as specified by the conversion pattern.
	 */
	virtual EString format(ELogEvent& event);
};

} /* namespace log */
} /* namespace efc */
#endif /* ESIMPLELAYOUT_HH_ */
