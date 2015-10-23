/*
 * ETTCCLayout.cpp
 *
 *  Created on: 2015-8-5
 *      Author: cxxjava@163.com
 */

#include "ETTCCLayout.hh"
#include "ETransform.hh"

namespace efc {
namespace log {

ETTCCLayout::ETTCCLayout(EConfig* props, EString& prefix) : EAbstractLayout(props, prefix) {
}

EString ETTCCLayout::format(ELogEvent& event) {
	EString output;

	ECalendar cal(event.getTimeMillis(), event.getTimeZone());
	output.concat(cal.toString("%Y-%m-%d %H:%M:%S,%s").c_str());

	output.append((char)0x5B /* '[' */);
	output.append(event.getThreadName());
	output.append((char)0x5D /* ']' */);
	output.append((char)0x20 /* ' ' */);

	output.append(ETransform::toLevelStr(event.getLevel()));
	output.append((char)0x20 /* ' ' */);

	output.append(event.getLoggerName());
	output.append((char)0x20 /* ' ' */);

	EString ndc = event.getNDC();
	if (ndc.length() > 0) {
		output.append(ndc);
		output.append((char)0x20 /* ' ' */);
	}

	output.append((char)0x2D /* '-' */);
	output.append((char)0x20 /* ' ' */);
	output.append(event.getMessage());
	output.append(ES_LINE_SEPARATOR);

	return output;
}

} /* namespace log */
} /* namespace efc */
