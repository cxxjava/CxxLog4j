/*
 * EOptionConverter.cpp
 *
 *  Created on: 2015-7-27
 *      Author: cxxjava@163.com
 */

#include "EOptionConverter.hh"
#include "ESystem.hh"
#include "ENumberFormatException.hh"

namespace efc {
namespace log {

static const char* DELIM_START = "${";
static const char DELIM_STOP = '}';
static const int DELIM_START_LEN = 2;
static const int DELIM_STOP_LEN = 1;

EString EOptionConverter::convertSpecialChars(const char* s) {
	EString sbuf;
	char c;
	int i = 0;
	int len = eso_strlen(s);
	while (i < len) {
		c = s[i++];
		if (c == '\\') {
			c = s[i++];
			if (c == 'n')
				c = '\n';
			else if (c == 'r')
				c = '\r';
			else if (c == 't')
				c = '\t';
			else if (c == 'f')
				c = '\f';
			else if (c == '\b')
				c = '\b';
			else if (c == '\"')
				c = '\"';
			else if (c == '\'')
				c = '\'';
			else if (c == '\\')
				c = '\\';
		}
		sbuf.append(c);
	}
	return sbuf;
}

const char* EOptionConverter::getSystemProperty(const char* key,
		const char* def) {
	return ESystem::getProperty(key, def);
}

EString EOptionConverter::substVars(const char* value, EConfig* props) {
	EString sbuf;

	int i = 0;
	int j, k;
	EString val(value);

	while (true ) {
		j = val.indexOf(DELIM_START, i);
		if (j == -1) {
			// no more variables
			if (i == 0) { // this is a simple string
				return val;
			} else { // add the tail string which contails no variables and return the result.
				sbuf.append(val.substring(i, val.length()));
				return sbuf;
			}
		} else {
			sbuf.append(val.substring(i, j));
			k = val.indexOf(DELIM_STOP, j);
			if (k == -1) {
				throw EIllegalArgumentException(
						EString::formatOf(
								"\"%s\" has no closing brace. Opening brace at position %d.",
								val.c_str(), j).c_str(), __FILE__, __LINE__);
			} else {
				j += DELIM_START_LEN;
				EString key = val.substring(j, k);
				// first try in System properties
				const char* replacement = getSystemProperty(key.c_str(), null);
				// then try props parameter
				if (replacement == null && props != null) {
					replacement = props->getString(key.c_str());
				}

				if (replacement != null) {
					// Do variable substitution on the replacement string
					// such that we can solve "Hello ${x2}" as "Hello p1"
					// the where the properties are
					// x1=p1
					// x2=${x1}
					EString recursiveReplacement = substVars(replacement,
							props);
					sbuf.append(recursiveReplacement);
				}
				i = k + DELIM_STOP_LEN;
			}
		}
	}
}

EString EOptionConverter::findAndSubst(const char* key, EConfig* props) {
	const char* value = props->getString(key);
	if (value == null)
		return null;

	try {
		return substVars(value, props);
	} catch (EIllegalArgumentException& e) {
		return value;
	}
}

ELogger::Level EOptionConverter::toLevel(const char* value,
		ELogger::Level defaultValue) {
	if (!value || !*value)
		return defaultValue;

	EString s(value);

	if (s.equalsIgnoreCase("ALL"))
		return ELogger::LEVEL_TRACE;
	if (s.equalsIgnoreCase("DEBUG"))
		return ELogger::LEVEL_DEBUG;
	if (s.equalsIgnoreCase("INFO"))
		return ELogger::LEVEL_INFO;
	if (s.equalsIgnoreCase("WARN"))
		return ELogger::LEVEL_WARN;
	if (s.equalsIgnoreCase("ERROR"))
		return ELogger::LEVEL_ERROR;
	if (s.equalsIgnoreCase("FATAL"))
		return ELogger::LEVEL_ERROR;
	if (s.equalsIgnoreCase("OFF"))
		return ELogger::LEVEL_OFF;
	if (s.equalsIgnoreCase("TRACE"))
		return ELogger::LEVEL_TRACE;

	return defaultValue;
}

llong EOptionConverter::toFileSize(const char* value, llong defval) {
	if (value == null)
		return defval;

	EString s(value);
	s.toUpperCase();
	long multiplier = 1;
	int index;

	if ((index = s.indexOf("KB")) != -1) {
		multiplier = 1024;
		s = s.substring(0, index);
	} else if ((index = s.indexOf("MB")) != -1) {
		multiplier = 1024 * 1024;
		s = s.substring(0, index);
	} else if ((index = s.indexOf("GB")) != -1) {
		multiplier = 1024 * 1024 * 1024;
		s = s.substring(0, index);
	}
	if (s.length() > 0) {
		try {
			return ELLong::valueOf(s.c_str()).llongValue() * multiplier;
		} catch (ENumberFormatException& e) {
		}
	}
	return defval;
}

} /* namespace log */
} /* namespace efc */
