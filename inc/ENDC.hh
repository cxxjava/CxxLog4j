/*
 * ENDC.hh
 *
 *  Created on: 2015-8-9
 *      Author: cxxjava@163.com
 */

#ifndef ENDC_HH_
#define ENDC_HH_

#include "Efc.hh"

namespace efc {
namespace log {

namespace ndc {
	class ThreadLocal;
}

/**
   The NDC class implements <i>nested diagnostic contexts</i> as
   defined by Neil Harrison in the article "Patterns for Logging
   Diagnostic Messages" part of the book "<i>Pattern Languages of
   Program Design 3</i>" edited by Martin et al.

   <p>A Nested Diagnostic Context, or NDC in short, is an instrument
   to distinguish interleaved log output from different sources. Log
   output is typically interleaved when a server handles multiple
   clients near-simultaneously.

   <p>Interleaved log output can still be meaningful if each log entry
   from different contexts had a distinctive stamp. This is where NDCs
   come into play.

   <p><em><b>Note that NDCs are managed on a per thread
   basis</b></em>. NDC operations such as {@link #push push}, {@link
   #pop}, {@link #clear}, {@link #getDepth} and {@link #setMaxDepth}
   affect the NDC of the <em>current</em> thread only. NDCs of other
   threads remain unaffected.

   <p>For example, a servlet can build a per client request NDC
   consisting the clients host name and other information contained in
   the the request. <em>Cookies</em> are another source of distinctive
   information. To build an NDC one uses the {@link #push push}
   operation. Simply put,

   <p><ul>
     <li>Contexts can be nested.

     <p><li>When entering a context, call <code>NDC.push</code>. As a
     side effect, if there is no nested diagnostic context for the
     current thread, this method will create it.

     <p><li>When leaving a context, call <code>NDC.pop</code>.

     <p><li><b>When exiting a thread make sure to call {@link #remove
     NDC.remove()}</b>.
   </ul>

   <p>There is no penalty for forgetting to match each
   <code>push</code> operation with a corresponding <code>pop</code>,
   except the obvious mismatch between the real application context
   and the context set in the NDC.

   <p>If configured to do so, {@link PatternLayout} and {@link
   TTCCLayout} instances automatically retrieve the nested diagnostic
   context for the current thread without any user intervention.
   Hence, even if a servlet is serving multiple clients
   simultaneously, the logs emanating from the same code (belonging to
   the same category) can still be distinguished because each client
   request will have a different NDC tag.

   <p>Heavy duty systems should call the {@link #remove} method when
   leaving the run method of a thread. This ensures that the memory
   used by the thread can be freed by the Java garbage
   collector. There is a mechanism to lazily remove references to dead
   threads. In practice, this means that you can be a little sloppy
   and sometimes forget to call {@link #remove} before exiting a
   thread.

   <p>A thread may inherit the nested diagnostic context of another
   (possibly parent) thread using the {@link #inherit inherit}
   method. A thread may obtain a copy of its NDC with the {@link
   #cloneStack cloneStack} method and pass the reference to any other
   thread, in particular to a child.

   @author Ceki G&uuml;lc&uuml;
   @since 0.7.0

*/

class ENDC: public EObject {
public:
	DECLARE_STATIC_INITZZ;

public:

	/**
	 Creates a nested diagnostic context.
	 Since java performs no automatic cleanup of objects when a
	 scope is left, in log4j push() and pop() must be used
	 to manage the NDC. For convenience, log4cxx provides
	 an NDC constructor and destructor which simply call the push() and
	 pop() methods, allowing for automatic cleanup when the current
	 scope ends.

	 @param message The new diagnostic context information.
	 @see The #push method.
	 */
	ENDC(const char* message);

	/**
	Removes the topmost element from the NDC stack.

	@see The #pop method.
	*/
	~ENDC();

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
	 * <font color="#FF4040"><b>Never use this method directly, use the {@link
	 * org.apache.log4j.spi.LoggingEvent#getNDC} method instead</b></font>.
	 */
	static EString get();

	/**
	 * Get the current nesting depth of this diagnostic context.
	 *
	 * @see #setMaxDepth
	 */
	static int getDepth();

	/**
	 * Clients should call this method before leaving a diagnostic
	 * context.
	 * <p>
	 * The returned value is the value that was pushed last. If no
	 * context is available, then the empty string "" is returned.
	 * </p>
	 * @return String The innermost diagnostic context.
	 */
	static EString* pop();

	/**
	 * Looks at the last diagnostic context at the top of this NDC
	 * without removing it.
	 * <p>
	 * The returned value is the value that was pushed last. If no
	 * context is available, then the empty string "" is returned.
	 * </p>
	 * @return String The innermost diagnostic context.
	 */
	static EString* peek();

	/**
	 * Push new diagnostic context information for the current thread.
	 * <p>
	 * The contents of the <code>message</code> parameter is
	 * determined solely by the client.
	 * </p>
	 * @param message The new diagnostic context information.
	 */
	static void push(EString* message);

	/**
	 * Remove the diagnostic context for this thread.
	 * <p>
	 * Each thread that created a diagnostic context by calling
	 * {@link #push} should call this method before exiting. Otherwise,
	 * the memory used by the <b>thread</b> cannot be reclaimed by the
	 * VM.
	 * </p>
	 * <p>
	 * As this is such an important problem in heavy duty systems and
	 * because it is difficult to always guarantee that the remove
	 * method is called before exiting a thread, this method has been
	 * augmented to lazily remove references to dead threads. In
	 * practice, this means that you can be a little sloppy and
	 * occasionally forget to call {@code remove} before exiting a
	 * thread. However, you must call <code>remove</code> sometime. If
	 * you never call it, then your application is sure to run out of
	 * memory.
	 * </p>
	 */
	static void remove();

	/**
	 * Set maximum depth of this diagnostic context. If the current
	 * depth is smaller or equal to <code>maxDepth</code>, then no
	 * action is taken.
	 * <p>
	 * This method is a convenient alternative to multiple {@link
	 * #pop} calls. Moreover, it is often the case that at the end of
	 * complex call sequences, the depth of the NDC is
	 * unpredictable. The <code>setMaxDepth</code> method circumvents
	 * this problem.
	 * </p>
	 * <p>
	 * For example, the combination
	 * </p>
	 * <pre>
	 * void foo() {
	 * &nbsp;  int depth = NDC.getDepth();
	 *
	 * &nbsp;  ... complex sequence of calls
	 *
	 * &nbsp;  NDC.setMaxDepth(depth);
	 * }
	 * </pre>
	 * <p>
	 * ensures that between the entry and exit of foo the depth of the
	 * diagnostic stack is conserved.
	 * </p>
	 *
	 * @see #getDepth
	 * @param maxDepth The maximum depth of the stack.
	 */
	static void setMaxDepth(int maxDepth);

private:
	// No instances allowed.
	ENDC() {}

	static ndc::ThreadLocal* localObj;
	static ndc::ThreadLocal* getLocalObj();
};

} /* namespace log */
} /* namespace efc */
#endif /* ENDC_HH_ */
