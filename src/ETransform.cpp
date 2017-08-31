/*
 * ETransform.cpp
 *
 *  Created on: 2015-8-11
 *      Author: cxxjava@163.com
 */

#include "./ETransform.hh"

namespace efc {
namespace log {

void ETransform::appendEscapingTags(EString& buf, const char* input) {
	if (!input || !*input) {
		return;
	}

	if (eso_strchr(input, '"') == 0 && eso_strchr(input, '&') == 0
			&& eso_strchr(input, '<') == 0 && eso_strchr(input, '>') == 0) {
		buf.append(input);
		return;
	}

	char specials[] = { 0x22 /* " */, 0x26 /* & */, 0x3C /* < */, 0x3E /* > */,
			0x00 };
	char *p = (char*)input;
	while (char c = *p++) {
		switch (c) {
		case 0x22: // "
			buf.append("&quot;");
			break;

		case 0x26: // &
			buf.append("&amp;");
			break;

		case 0x3C: // <
			buf.append("&lt;");
			break;

		case 0x3E: // >
			buf.append("&gt;");
			break;

		default:
			buf.append(c);
			break;
		}
	}
}

void ETransform::appendEscapingCDATA(EString& buf, const char* input) {
	const char * CDATA_START = "<![CDATA[";
	const char * CDATA_END = "]]>";
	const char * CDATA_PSEUDO_END = "]]&gt;";
	const char * CDATA_EMBEDED_END = "]]>]]&gt;<![CDATA[";
	int CDATA_END_LEN = eso_strlen(CDATA_END);

	if (input && *input) {
		const char *pend = eso_strstr(input, CDATA_END);
		if (!pend) {
			buf.append(input);
		} else {
			int input_length = eso_strlen(input);
			const char *pstart = NULL;
			while (pend) {
				buf.append(input, pend - input);
				buf.append(CDATA_EMBEDED_END);
				pstart = pend + CDATA_END_LEN;
				if (pstart < input + input_length) {
					pend = eso_strstr(pstart, CDATA_END);
				} else {
					return;
				}
			}
			buf.append(pstart);
		}
	}
}

const char* ETransform::toLevelStr(ELogger::Level level) {
	switch (level) {
	case ELogger::LEVEL_TRACE:
		return "TRACE";
	case ELogger::LEVEL_DEBUG:
		return "DEBUG";
	case ELogger::LEVEL_INFO:
		return "INFO";
	case ELogger::LEVEL_WARN:
		return "WARN";
	case ELogger::LEVEL_ERROR:
		return "ERROR";
	case ELogger::LEVEL_OFF:
		return "OFF";
	}
	return "UNKNOWN";
}

} /* namespace log */
} /* namespace efc */
