/*
 * EPatternLayout.cpp
 *
 *  Created on: 2015-7-24
 *      Author: cxxjava@163.com
 */

#include "./EPatternLayout.hh"
#include "./EOptionConverter.hh"
#include "./ETransform.hh"
#include "../inc/ELoggerManager.hh"

namespace efc {
namespace log {

namespace pl {

PatternConverter::PatternConverter(const FormattingInfo& i) {
	minLen = i.minLen;
	maxLen = i.maxLen;
	leftAlign = i.leftAlign;
	trimStart = i.trimStart;
}

PatternConverter::~PatternConverter() {
}

void PatternConverter::formatAndAppend(EString& output, ELogEvent& event) {
	EString s = convert(event);

	if (s.isEmpty()) {
		if (minLen > 0)
			spacePad(output, minLen);
		return;
	}

	int len = s.length();

	if (len > maxLen) {
		if (trimStart)
			output.append(s.substring(len - maxLen));
		else
			output.append(s.substring(0, maxLen));
	}
	else if (len < minLen) {
		if (leftAlign) {
			output.append(s);
			spacePad(output, minLen - len);
		} else {
			spacePad(output, minLen - len);
			output.append(s);
		}
	} else {
		output.append(s);
	}
}

/**
 Fast space padding method.
 */
void PatternConverter::spacePad(EString& sbuf, int length) {
	const char* SPACES[] = {" ", "  ", "    ", "        ", //1,2,4,8 spaces
					"                ", // 16 spaces
					"                                " }; // 32 spaces
	while (length >= 32) {
		sbuf.append(SPACES[5]);
		length -= 32;
	}

	for (int i = 4; i >= 0; i--) {
		if ((length & (1 << i)) != 0) {
			sbuf.append(SPACES[i]);
		}
	}
}

/**
 * This PatternConverter returns a constant string.
 */
class LiteralPatternConverter : public PatternConverter
{
public:
	LiteralPatternConverter(EString& str_) :
			PatternConverter(FormattingInfo()), str(str_) {
	}
    virtual EString convert(ELogEvent& event) {
        return str;
    }

private:
    EString str;
};

/**
 * This PatternConverter is used to format most of the "simple" fields
 * found in the InternalLoggingEvent object.
 */
class BasicPatternConverter: public PatternConverter {
public:
	enum Type {
		THREAD_CONVERTER,
		PROCESS_CONVERTER,
		LOGLEVEL_CONVERTER,
		NDC_CONVERTER,
		MESSAGE_CONVERTER,
		NEWLINE_CONVERTER,
		BASENAME_CONVERTER,
		FILE_CONVERTER,
		LINE_CONVERTER,
		FULL_LOCATION_CONVERTER,
		THROWABLE_CONVERT,
		FUNCTION_CONVERTER
	};
	BasicPatternConverter(const FormattingInfo& info, Type type_) :
			PatternConverter(info), type(type_) {
	}
	virtual EString convert(ELogEvent& event) {
		switch (type) {
		case LOGLEVEL_CONVERTER:
			return ETransform::toLevelStr(event.getLevel());

		case BASENAME_CONVERTER:
			return event.getFileName();

		case PROCESS_CONVERTER:
			return EString((long)eso_os_process_current());

		case NDC_CONVERTER:
			return event.getNDC();

		case MESSAGE_CONVERTER:
			return event.getMessage();

		case NEWLINE_CONVERTER:
			return ES_LINE_SEPARATOR;

		case FILE_CONVERTER:
			return event.getFileName();

		case THREAD_CONVERTER:
			return event.getThreadName();

		case LINE_CONVERTER: {
			if (event.getLineNumber() != -1)
				return EString(event.getLineNumber());
		}
			break;

		case FULL_LOCATION_CONVERTER: {
			EString sbuf(event.getFileName());
			if (!sbuf.isEmpty()) {
				sbuf.append(":").append(event.getLineNumber());
				return sbuf;
			}
		}
			break;

		case THROWABLE_CONVERT:
			return event.getThrowableStr();

		case FUNCTION_CONVERTER:
			//...
			break;
		}

		return "";
	}

private:
	// Disable copy
	BasicPatternConverter(const BasicPatternConverter&);
	BasicPatternConverter& operator=(BasicPatternConverter&);

	Type type;
};

/**
 * This PatternConverter is used to format the Logger field found in
 * the InternalLoggingEvent object.
 */
class LoggerPatternConverter: public PatternConverter {
public:
	LoggerPatternConverter(const FormattingInfo& info, int prec) :
			PatternConverter(info), precision(prec) {
	}
	virtual EString convert(ELogEvent& event) {
		EString name(event.getLoggerName());

		if (precision <= 0) {
			return name;
		} else {
			int len = name.length();

			// We substract 1 from 'len' when assigning to 'end' to avoid out of
			// bounds exception in return r.substring(end+1, len). This can happen
			// if precision is 1 and the logger name ends with a dot.
			int end = len - 1;
			for (int i = precision; i > 0; --i) {
				end = name.lastIndexOf('.', end - 1);
				if (end == -1) {
					return name;
				}
			}
			return name.substring(end + 1);
		}
	}

private:
	int precision;
};

/**
 * This PatternConverter is used to format an environment variable
 */
class EnvPatternConverter: public PatternConverter {
public:
	EnvPatternConverter(const FormattingInfo& info, const EString& env) :
			PatternConverter(info), envKey(env) {
	}
	virtual EString convert(ELogEvent& event) {
		return ESystem::getEnv(envKey.c_str());
	}

private:
	const EString envKey;
};

//! This pattern is used to format miliseconds since process start.
class RelativeTimestampConverter: public PatternConverter {
public:
	RelativeTimestampConverter(const FormattingInfo& info) :
			PatternConverter(info) {
	}
	virtual EString convert(ELogEvent& event) {
		return EString(event.getTimeMillis() - ELoggerManager::getStartTime());
	}
};

/**
 * This PatternConverter is used to format the timestamp field found in
 * the InternalLoggingEvent object.  It will be formatted according to
 * the specified "pattern".
 */
class DatePatternConverter: public PatternConverter {
public:
	DatePatternConverter(const FormattingInfo& info, EString& pattern) :
			PatternConverter(info), format(pattern) {
	}
	virtual EString convert(ELogEvent& event) {
		if (format.equalsIgnoreCase("ABSOLUTE")) {
			ECalendar cal(event.getTimeMillis(), event.getTimeZone());
			return cal.toString("%d %b %Y %H:%M:%S,%s");
		}
		else if (format.equalsIgnoreCase("DATE")) {
			ECalendar cal(event.getTimeMillis(), event.getTimeZone());
			return cal.toString("%H:%M:%S,%s");
		}
		else if (format.equalsIgnoreCase("ISO8601")) {
			ECalendar cal(event.getTimeMillis(), event.getTimeZone());
			return ECalendar::toISO8601String(&cal);
		}
		else {
			// Convert log4j simple date format to calendar format.
			// dd MM yyyy HH:mm:ss,SSS --> %Y-%m-%d %H:%M:%S,%s
			EString fmt2 = format;
			fmt2.replace("yyyy", "%Y"); fmt2.replace("yy", "%y");
			fmt2.replace("MMM", "%b"); fmt2.replace("MM", "%m");
			fmt2.replace("dd", "%d");
			fmt2.replace("HH", "%H");
			fmt2.replace("mm", "%M");
			fmt2.replace("ss", "%S");
			fmt2.replace("SSS", "%s");

			ECalendar cal(event.getTimeMillis(), event.getTimeZone());

			return cal.toString(fmt2.c_str());
		}
	}
private:
	EString format;
};

/**
 * This PatternConverter is used to format the MDC field found in
 * the InternalLoggingEvent object, optionally limited to
 * \c k Mapped diagnostic context key.
 */
class MDCPatternConverter: public PatternConverter {
public:
	MDCPatternConverter(const FormattingInfo& info, const EString& k) :
			PatternConverter(info), key(k.c_str()) {
	}
	virtual EString convert(ELogEvent& event) {
		if (!key.isEmpty()) {
			return event.getMDC(key.c_str());
		}
		else {
			EString result;

			EHashMap<EString*, EString*>* hm = event.getMDC();
			if (hm) {
				sp<EIterator<EMapEntry<EString*, EString*>*> > iter = hm->entrySet()->iterator();
				while(iter->hasNext()) {
					EMapEntry<EString*, EString*>* me = iter->next();
					EString x = EString::formatOf("{%s,%s}",
							me->getKey()->c_str(),
							me->getValue()->c_str());
					result += x;
				}
			}

			return result;
		}
	}
private:
	EString key;
};


/**
 * This PatternConverter is used to format the NDC field found in
 * the InternalLoggingEvent object, optionally limited to
 * \c precision levels (using space to separate levels).
 */
class NDCPatternConverter: public PatternConverter {
public:
	NDCPatternConverter(const FormattingInfo& info, int precision_) :
			PatternConverter(info), precision(precision_) {
	}
	virtual EString convert(ELogEvent& event) {
		EString text = event.getNDC();
		if (precision <= 0)
			return text;
		else {
			int p = text.indexOf(' ');
			for (int i = 1; i < precision && p != -1; ++i)
				p = text.indexOf(' ', p + 1);

			return text.substring(0, p);
		}
	}

private:
	int precision;
};


/**
 * This class parses a "pattern" string into an array of
 * PatternConverter objects.
 * <p>
 * @see PatternLayout for the formatting of the "pattern" string.
 */
class PatternParser {
public:
	PatternParser(EString& pattern_) : pattern(pattern_)
    , state(LITERAL_STATE)
    , pos(0) {
	}

	void parseTo(EVector<pl::PatternConverter*>& parsedPattern) {
		const char ESCAPE_CHAR = '%';

		char c;
		pos = 0;
		while (pos < pattern.length()) {
			c = pattern[pos++];
			switch (state) {
			case LITERAL_STATE:
				// In literal state, the last char is always a literal.
				if (pos == pattern.length()) {
					currentLiteral += c;
					continue;
				}
				if (c == ESCAPE_CHAR) {
					// peek at the next char.
					switch (pattern[pos]) {
					case ESCAPE_CHAR:
						currentLiteral += c;
						pos++; // move pointer
						break;
					default:
						if (!currentLiteral.isEmpty()) {
							parsedPattern.add(
									new LiteralPatternConverter(
											currentLiteral));
						}
						currentLiteral.clear();
						currentLiteral += c; // append %
						state = CONVERTER_STATE;
						formattingInfo.reset();
					}
				} else {
					currentLiteral += c;
				}
				break;

			case CONVERTER_STATE:
				currentLiteral += c;
				switch (c) {
				case '-':
					formattingInfo.leftAlign = true;
					break;
				case '.':
					state = DOT_STATE;
					break;
				default:
					if (c >= '0' && c <= '9') {
						formattingInfo.minLen = c - '0';
						state = MIN_STATE;
					} else {
						finalizeConverter(parsedPattern, c);
					}
				} // switch
				break;

			case MIN_STATE:
				currentLiteral += c;
				if (c >= '0' && c <= '9') {
					formattingInfo.minLen = formattingInfo.minLen * 10
							+ (c - '0');
				} else if (c == '.') {
					state = DOT_STATE;
				} else {
					finalizeConverter(parsedPattern, c);
				}
				break;

			case DOT_STATE:
				currentLiteral += c;
				if (c == '-')
					formattingInfo.trimStart = false;
				else if (c >= '0' && c <= '9') {
					formattingInfo.maxLen = c - '0';
					state = MAX_STATE;
				} else {
					EString buf;
					buf << "Error occured in position " << pos
							<< ".\n Was expecting digit, instead got char \""
							<< c << "\".";
					state = LITERAL_STATE;
				}
				break;

			case MAX_STATE:
				currentLiteral += c;
				if (c >= '0' && c <= '9')
					formattingInfo.maxLen = formattingInfo.maxLen * 10
							+ (c - '0');
				else {
					finalizeConverter(parsedPattern, c);
					state = LITERAL_STATE;
				}
				break;
			} // end switch
		} // end while

		if (!currentLiteral.isEmpty()) {
			parsedPattern.add(new LiteralPatternConverter(currentLiteral));
		}
	}

private:
	// Types
	enum ParserState {
		LITERAL_STATE, CONVERTER_STATE, DOT_STATE, MIN_STATE, MAX_STATE
	};

	// Methods
	EString extractOption() {
		if ((pos < pattern.length()) && (pattern[pos] == '{')) {
			int end = pattern.indexOf('}', pos);
			if (end != -1) {
				EString r = pattern.substring(pos + 1, end);
				pos = end + 1;
				return r;
			} else {
				EString buf;
				buf << "No matching '}' found in conversion pattern string \""
							<< pattern
							<< "\"";
				pos = pattern.length();
			}
		}

		return "";
	}

	int extractPrecisionOption() {
		EString opt = extractOption();
		int r = 0;
		if (!opt.isEmpty())
			r = EInteger::parseInt(opt.c_str());

		return r;
	}

	void finalizeConverter(EVector<pl::PatternConverter*>& parsedPattern, char c) {
		PatternConverter* pc = 0;
		switch (c) {
		case 'c':
			pc = new LoggerPatternConverter(formattingInfo,
					extractPrecisionOption());
			break;

		case 'C':
			pc = new BasicPatternConverter(formattingInfo,
					BasicPatternConverter::BASENAME_CONVERTER);
			break;

		case 'd':
		case 'D': {
			EString dOpt = extractOption();
			if (dOpt.isEmpty()) {
				dOpt = "%Y-%m-%d %H:%M:%S,%s";
			}
			pc = new DatePatternConverter(formattingInfo, dOpt);
		}
			break;

		case 'E':
			pc = new EnvPatternConverter(formattingInfo, extractOption());
			break;

		case 'F':
			pc = new BasicPatternConverter(formattingInfo,
					BasicPatternConverter::FILE_CONVERTER);
			break;

		case 'i':
			pc = new BasicPatternConverter(formattingInfo,
					BasicPatternConverter::PROCESS_CONVERTER);
			break;

		case 'l':
			pc = new BasicPatternConverter(formattingInfo,
					BasicPatternConverter::FULL_LOCATION_CONVERTER);
			break;

		case 'L':
			pc = new BasicPatternConverter(formattingInfo,
					BasicPatternConverter::LINE_CONVERTER);
			break;

		case 'm':
			pc = new BasicPatternConverter(formattingInfo,
					BasicPatternConverter::MESSAGE_CONVERTER);
			break;

		case 'M':
			pc = new BasicPatternConverter(formattingInfo,
					BasicPatternConverter::FUNCTION_CONVERTER);
			break;

		case 'n':
			pc = new BasicPatternConverter(formattingInfo,
					BasicPatternConverter::NEWLINE_CONVERTER);
			break;

		case 'p':
			pc = new BasicPatternConverter(formattingInfo,
					BasicPatternConverter::LOGLEVEL_CONVERTER);
			break;

		case 'r':
			pc = new RelativeTimestampConverter(formattingInfo);
			break;

		case 't':
			pc = new BasicPatternConverter(formattingInfo,
					BasicPatternConverter::THREAD_CONVERTER);
			break;

		case 'T':
			pc = new BasicPatternConverter(formattingInfo,
					BasicPatternConverter::THROWABLE_CONVERT);
			break;

		case 'x':
			pc = new NDCPatternConverter(formattingInfo, 0);
			break;

		case 'X':
			pc = new MDCPatternConverter(formattingInfo, extractOption());
			break;

		default:
			EString buf;
			buf << "Unexpected char [" << c << "] at position " << pos
					<< " in conversion patterrn.";
			pc = new LiteralPatternConverter(currentLiteral);
			break;
		}
//
		parsedPattern.add(pc);
		currentLiteral.clear();
		state = LITERAL_STATE;
		formattingInfo.reset();
	}

private:
	// Data
	EString pattern;
	ParserState state;
	int pos;
	EString currentLiteral;
	FormattingInfo formattingInfo;
};

} /* namespace pl */

//=============================================================================

EPatternLayout::EPatternLayout(EConfig* props, EString& prefix) :
		EAbstractLayout(props, prefix) {
	EString pattern = prefix + ".ConversionPattern";
	conversionPattern = EOptionConverter::findAndSubst(pattern.c_str(), props);
	if (conversionPattern.isEmpty()) {
		conversionPattern ="%m%n";
	}

	parsedPattern.setThreadSafe(false);

	pl::PatternParser pp(conversionPattern);
	pp.parseTo(parsedPattern);
}

const char* EPatternLayout::getConversionPattern() {
	return conversionPattern.c_str();
}

EString EPatternLayout::format(ELogEvent& event) {
	EString output;

	sp<EEnumeration<pl::PatternConverter*> > e = parsedPattern.elements();
	for (; e != null && e->hasMoreElements();) {
		pl::PatternConverter *pc = (pl::PatternConverter*)e->nextElement();
		if (pc) pc->formatAndAppend(output, event);
	}

	return output;
}

} /* namespace log */
} /* namespace efc */
