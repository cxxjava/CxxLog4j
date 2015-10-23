/*
 * EOptionConverter.hh
 *
 *  Created on: 2015-7-27
 *      Author: cxxjava@163.com
 */

#ifndef EOPTIONCONVERTER_HH_
#define EOPTIONCONVERTER_HH_

#include "Efc.hh"
#include "ELogger.hh"

namespace efc {
namespace log {

class EOptionConverter {
public:
	static EString convertSpecialChars(const char* s);

	/**
	 Very similar to <code>System.getProperty</code> except
	 that the {@link SecurityException} is hidden.

	 @param key The key to search for.
	 @param def The default value to return.
	 @return the string value of the system property, or the default
	 value if there is no property with that key.

	 @since 1.1 */
	static const char* getSystemProperty(const char* key, const char* def);

	/**
	 Perform variable substitution in string <code>val</code> from the
	 values of keys found in the system propeties.

	 <p>The variable substitution delimeters are <b>${</b> and <b>}</b>.

	 <p>For example, if the System properties contains "key=value", then
	 the call
	 <pre>
	 String s = OptionConverter.substituteVars("Value of key is ${key}.");
	 </pre>

	 will set the variable <code>s</code> to "Value of key is value.".

	 <p>If no value could be found for the specified key, then the
	 <code>props</code> parameter is searched, if the value could not
	 be found there, then substitution defaults to the empty string.

	 <p>For example, if system propeties contains no value for the key
	 "inexistentKey", then the call

	 <pre>
	 String s = OptionConverter.subsVars("Value of inexistentKey is [${inexistentKey}]");
	 </pre>
	 will set <code>s</code> to "Value of inexistentKey is []"

	 <p>An {@link java.lang.IllegalArgumentException} is thrown if
	 <code>val</code> contains a start delimeter "${" which is not
	 balanced by a stop delimeter "}". </p>

	 <p><b>Author</b> Avy Sharell</a></p>

	 @param val The string on which variable substitution is performed.
	 @throws IllegalArgumentException if <code>val</code> is malformed.
	*/
	static EString substVars(const char* val, EConfig* props) THROWS(EIllegalArgumentException);

	/**
	 Find the value corresponding to <code>key</code> in
	 <code>props</code>. Then perform variable substitution on the
	 found value.
	 */
	static EString findAndSubst(const char* key, EConfig* props);

	/**
	 Converts a standard or custom priority level to a Level
	 object.  <p> If <code>value</code> is of form
	 "level#classname", then the specified class' toLevel method
	 is called to process the specified level string; if no '#'
	 character is present, then the default {@link org.apache.log4j.Level}
	 class is used to process the level value.

	 <p>As a special case, if the <code>value</code> parameter is
	 equal to the string "NULL", then the value <code>null</code> will
	 be returned.

	 <p> If any error occurs while converting the value to a level,
	 the <code>defaultValue</code> parameter, which may be
	 <code>null</code>, is returned.

	 <p> Case of <code>value</code> is insignificant for the level level, but is
	 significant for the class name part, if present.

	 @since 1.1 */
	static ELogger::Level toLevel(const char* value, ELogger::Level defaultValue=ELogger::LEVEL_TRACE);

	/**
	 Get the size that the output file is allowed to reach.

	 <p>In configuration files, the <b>MaxFileSize</b> option takes an
	 long integer in the range 0 - 2^63. You can specify the value
	 with the suffixes "KB", "MB" or "GB" so that the integer is
	 interpreted being expressed respectively in kilobytes, megabytes
	 or gigabytes. For example, the value "10KB" will be interpreted
	 as 10240.
	 */
	static llong toFileSize(const char* value, llong defval);

private:
	 /** OptionConverter is a static class. */
	 EOptionConverter() {}
};

} /* namespace log */
} /* namespace efc */
#endif /* EOPTIONCONVERTER_HH_ */
