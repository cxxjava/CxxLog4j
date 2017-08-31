/*
 * ETransform.hh
 *
 *  Created on: 2015-8-11
 *      Author: cxxjava@163.com
 */

#ifndef ETRANSFORM_HH_
#define ETRANSFORM_HH_

#include "Efc.hh"
#include "../inc/ELogger.hh"

namespace efc {
namespace log {

/**
 * Utility class for transforming strings.
 */

class ETransform {
public:
	/**
	 * This method takes a string which may contain HTML tags (ie,
	 * &lt;b&gt;, &lt;table&gt;, etc) and replaces any
	 * '<',  '>' , '&' or '"'
	 * characters with respective predefined entity references.
	 *
	 * @param buf StringBuffer holding the escaped data to this point.
	 * @param input The text to be converted.
	 * */
	static void appendEscapingTags(EString& buf, const char* input);

	/**
	 * Ensures that embeded CDEnd strings (]]>) are handled properly
	 * within message, NDC and throwable tag text.
	 *
	 * @param buf StringBuffer holding the XML data to this point.  The
	 * initial CDStart (<![CDATA[) and final CDEnd (]]>) of the CDATA
	 * section are the responsibility of the calling method.
	 * @param input The String that is inserted into an existing CDATA Section within buf.
	 * */
	static void appendEscapingCDATA(EString& buf, const char* input);

	/**
	 * convert logger level from integer to string
	 */
	static const char* toLevelStr(ELogger::Level level);

};

} /* namespace log */
} /* namespace efc */
#endif /* ETRANSFORM_HH_ */
