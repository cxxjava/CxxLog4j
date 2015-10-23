/*
 * EMDC.hh
 *
 *  Created on: 2015-8-9
 *      Author: cxxjava@163.com
 */

#ifndef EMDC_HH_
#define EMDC_HH_

#include "Efc.hh"

namespace efc {
namespace log {

namespace mdc {
	class ThreadLocal;
}


/**
   The MDC class is similar to the {@link NDC} class except that it is
   based on a map instead of a stack. It provides <em>mapped
   diagnostic contexts</em>. A <em>Mapped Diagnostic Context</em>, or
   MDC in short, is an instrument for distinguishing interleaved log
   output from different sources. Log output is typically interleaved
   when a server handles multiple clients near-simultaneously.

   <p><b><em>The MDC is managed on a per thread basis</em></b>. A
   child thread automatically inherits a <em>copy</em> of the mapped
   diagnostic context of its parent.

   <p>The MDC class requires JDK 1.2 or above. Under JDK 1.1 the MDC
   will always return empty values but otherwise will not affect or
   harm your application.

   @since 1.2

   @author Ceki G&uuml;lc&uuml;
*/

class EMDC: public EObject {
public:
	DECLARE_STATIC_INITZZ;

public:

	/**
	 *  Places a key/value pair in the MDC for the current thread
	 *  which will be removed during the corresponding destructor.  Both
	 *  construction and destruction are expected to be on the same thread.
	 *  @param key key
	 *  @param value value.
	 */
	EMDC(const char* key, const char* value);
	~EMDC();

	/**
	 * Clear any nested diagnostic information if any. This method is
	 * useful in cases where the same thread can be potentially used
	 * over and over in different unrelated contexts.
	 * <p>
	 * This method is equivalent to calling the {@link #setMaxDepth}
	 * method with a zero <code>maxDepth</code> argument.
	 * </p>
	 */
	static void clear();

	/**
	 * Get the context identified by the <code>key</code> parameter.
	 *
	 * <p>This method has no side effects.
	 * @param key key.
	 * @return value for key, empty if not set.
	 */
	static const char* get(const char* key);
	static EHashMap<EString*, EString*>* get();

	/**
	 * Put a context value (the <code>o</code> parameter) as identified
	 * with the <code>key</code> parameter into the current thread's
	 * context map.
	 *
	 * <p>If the current thread does not have a context map it is
	 * created as a side effect.
	 * @param key key
	 * @param value value.
	 */
	static void put(const char* key, const char* value);

	/**
	 * Remove the the context identified by the <code>key</code>
	 * parameter.
	 * @param key key.
	 * @return value if key had been set, empty if not.
	 */
	static void remove(const char* key);

private:
	EString key;
	// No instances allowed.
	EMDC() {}

	static mdc::ThreadLocal* localObj;
	static mdc::ThreadLocal* getLocalObj();
};

} /* namespace log */
} /* namespace efc */
#endif /* EMDC_HH_ */
