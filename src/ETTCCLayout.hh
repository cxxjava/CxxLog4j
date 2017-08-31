/*
 * ETTCCLayout.hh
 *
 *  Created on: 2015-8-5
 *      Author: cxxjava@163.com
 */

#ifndef ETTCCLAYOUT_HH_
#define ETTCCLAYOUT_HH_

#include "./EAbstractLayout.hh"

namespace efc {
namespace log {

/**
 TTCC layout format consists of time, thread, category and nested
 diagnostic context information, hence the name.

 <p>Each of the four fields can be individually enabled or
 disabled. The time format depends on the <code>DateFormat</code>
 used.

 <p>Here is an example TTCCLayout output with the
 {@link org.apache.log4j.helpers.RelativeTimeDateFormat}.

 <pre>
176 [main] INFO  org.apache.log4j.examples.Sort - Populating an array of 2 elements in reverse order.
225 [main] INFO  org.apache.log4j.examples.SortAlgo - Entered the sort method.
262 [main] DEBUG org.apache.log4j.examples.SortAlgo.OUTER i=1 - Outer loop.
276 [main] DEBUG org.apache.log4j.examples.SortAlgo.SWAP i=1 j=0 - Swapping intArray[0] = 1 and intArray[1] = 0
290 [main] DEBUG org.apache.log4j.examples.SortAlgo.OUTER i=0 - Outer loop.
304 [main] INFO  org.apache.log4j.examples.SortAlgo.DUMP - Dump of interger array:
317 [main] INFO  org.apache.log4j.examples.SortAlgo.DUMP - Element [0] = 0
331 [main] INFO  org.apache.log4j.examples.SortAlgo.DUMP - Element [1] = 1
343 [main] INFO  org.apache.log4j.examples.Sort - The next log statement should be an error message.
346 [main] ERROR org.apache.log4j.examples.SortAlgo.DUMP - Tried to dump an uninitialized array.
        at org.apache.log4j.examples.SortAlgo.dump(SortAlgo.java:58)
        at org.apache.log4j.examples.Sort.main(Sort.java:64)
467 [main] INFO  org.apache.log4j.examples.Sort - Exiting main method.
</pre>

  <p>The first field is the number of milliseconds elapsed since the
  start of the program. The second field is the thread outputting the
  log statement. The third field is the level, the fourth field is
  the category to which the statement belongs.

  <p>The fifth field (just before the '-') is the nested diagnostic
  context.  Note the nested diagnostic context may be empty as in the
  first two statements. The text after the '-' is the message of the
  statement.

  <p><b>WARNING</b> Do not use the same TTCCLayout instance from
  within different appenders. The TTCCLayout is not thread safe when
  used in his way. However, it is perfectly safe to use a TTCCLayout
  instance from just one appender.

  <p>{@link PatternLayout} offers a much more flexible alternative.
*/

class ETTCCLayout: public EAbstractLayout {
public:
	ETTCCLayout(EConfig* props, EString& prefix);

	/**
	 * Produces a formatted string as specified by the conversion pattern.
	 */
	virtual EString format(ELogEvent& event);
};

} /* namespace log */
} /* namespace efc */
#endif /* ETTCCLAYOUT_HH_ */
