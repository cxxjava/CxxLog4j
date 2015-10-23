/*
 * ELayoutFactory.cpp
 *
 *  Created on: 2015-8-2
 *      Author: cxxjava@163.com
 */

#include "ELayoutFactory.hh"
#include "EOptionConverter.hh"
#include "ESimpleLayout.hh"
#include "ETTCCLayout.hh"
#include "EHTMLLayout.hh"
#include "EPatternLayout.hh"

namespace efc {
namespace log {

static const char* CLASSNAME_SIMPLE_LAYOUT = "org.apache.log4j.SimpleLayout";
static const char* CLASSNAME_TTCC_LAYOUT = "org.apache.log4j.TTCCLayout";
static const char* CLASSNAME_PATTERN_LAYOUT = "org.apache.log4j.PatternLayout";
static const char* CLASSNAME_HTML_LAYOUT = "org.apache.log4j.HTMLLayout";

ELayout* ELayoutFactory::newInstance(EConfig* props, EString& prefix) {
	// Get appender layout : log4j.appender.XXX.layout = [layout]
	EString className = EOptionConverter::findAndSubst(prefix.c_str(), props);
	if (className.isEmpty()) {
		return null;
	}

	if (className.equalsIgnoreCase(CLASSNAME_SIMPLE_LAYOUT)) {
		return new ESimpleLayout(props, prefix);
	}
	else if (className.equalsIgnoreCase(CLASSNAME_TTCC_LAYOUT)) {
		return new ETTCCLayout(props, prefix);
	}
	else if (className.equalsIgnoreCase(CLASSNAME_PATTERN_LAYOUT)) {
		return new EPatternLayout(props, prefix);
	}
	else if (className.equalsIgnoreCase(CLASSNAME_HTML_LAYOUT)) {
		return new EHTMLLayout(props, prefix);
	}

	return null;
}

} /* namespace log */
} /* namespace efc */
