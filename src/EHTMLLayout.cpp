/*
 * EHTMLLayout.cpp
 *
 *  Created on: 2015-7-25
 *      Author: cxxjava@163.com
 */

#include "EHTMLLayout.hh"
#include "EOptionConverter.hh"
#include "ETransform.hh"
#include "ELoggerManager.hh"

namespace efc {
namespace log {

EHTMLLayout::EHTMLLayout(EConfig* props, EString& prefix) :
		EAbstractLayout(props, prefix), locationInfo(false),
		title("Log4J Log Messages") {
	EString title = prefix + ".Title";
	EString titleStr = EOptionConverter::findAndSubst(title.c_str(), props);

	EString locationInfo = prefix + ".LocationInfo";
	EString locationInfoStr = EOptionConverter::findAndSubst(locationInfo.c_str(), props);

	this->setTitle(titleStr.c_str());
	this->setLocationInfo(EBoolean::parseBoolean(locationInfoStr.c_str()));
}

EString EHTMLLayout::format(ELogEvent& event) {
	EString sbuf;

	sbuf.append(ES_LINE_SEPARATOR "<tr>" ES_LINE_SEPARATOR);

	sbuf.append("<td>");
	sbuf.append(event.getTimeMillis() - ELoggerManager::getStartTime());
	sbuf.append("</td>" ES_LINE_SEPARATOR);

	sbuf.append("<td title=\"");
	ETransform::appendEscapingTags(sbuf, event.getThreadName());
	sbuf.append(" thread\">");
	ETransform::appendEscapingTags(sbuf, event.getThreadName());
	sbuf.append("</td>" ES_LINE_SEPARATOR);

	sbuf.append("<td title=\"Level\">");
	if (event.getLevel() == ELogger::LEVEL_DEBUG) {
		sbuf.append("<font color=\"#339933\">");
		sbuf.append(ETransform::toLevelStr(event.getLevel()));
		sbuf.append("</font>");
	} else if (event.getLevel() >= ELogger::LEVEL_WARN) {
		sbuf.append("<font color=\"#993300\"><strong>");
		sbuf.append(ETransform::toLevelStr(event.getLevel()));
		sbuf.append("</strong></font>");
	} else {
		sbuf.append(ETransform::toLevelStr(event.getLevel()));
	}
	sbuf.append("</td>" ES_LINE_SEPARATOR);

	sbuf.append("<td title=\"");
	ETransform::appendEscapingTags(sbuf, event.getLoggerName());
	sbuf.append(" category\">");
	ETransform::appendEscapingTags(sbuf, event.getLoggerName());
	sbuf.append("</td>" ES_LINE_SEPARATOR);

	if (locationInfo) {
		sbuf.append("<td>");
		ETransform::appendEscapingTags(sbuf, event.getFileName());
		sbuf.append(':');
		sbuf.append(event.getLineNumber());
		sbuf.append("</td>" ES_LINE_SEPARATOR);
	}

	sbuf.append("<td title=\"Message\">");
	ETransform::appendEscapingTags(sbuf, event.getMessage());
	sbuf.append("</td>" ES_LINE_SEPARATOR);
	sbuf.append("</tr>" ES_LINE_SEPARATOR);

	if (!event.getNDC().isEmpty()) {
		sbuf.append(
				"<tr><td bgcolor=\"#EEEEEE\" style=\"font-size : xx-small;\" colspan=\"6\" title=\"Nested Diagnostic Context\">");
		sbuf.append("NDC: ");
		ETransform::appendEscapingTags(sbuf, event.getNDC().c_str());
		sbuf.append("</td></tr>" ES_LINE_SEPARATOR);
	}

	EString s = event.getThrowableStr();
	if (s.length() > 0) {
		sbuf.append(
				"<tr><td bgcolor=\"#993300\" style=\"color:White; font-size : xx-small;\" colspan=\"6\">" ES_LINE_SEPARATOR);
		ETransform::appendEscapingTags(sbuf, s.c_str());
		sbuf.append(ES_LINE_SEPARATOR "</td></tr>" ES_LINE_SEPARATOR);
	}

	return sbuf;
}

EString EHTMLLayout::getContentType() {
	return "text/html";
}

EString EHTMLLayout::getHeader() {
	ECalendar now;

	EString sbuf;
	sbuf.append(
			"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">"
					ES_LINE_SEPARATOR);
	sbuf.append("<html>" ES_LINE_SEPARATOR);
	sbuf.append("<head>" ES_LINE_SEPARATOR);
	sbuf.append("<title>").append(this->title).append("</title>" ES_LINE_SEPARATOR);
	sbuf.append("<style type=\"text/css\">" ES_LINE_SEPARATOR);
	sbuf.append("<!--" ES_LINE_SEPARATOR);
	sbuf.append(
			"body, table {font-family: arial,sans-serif; font-size: x-small;}"
					ES_LINE_SEPARATOR);
	sbuf.append(
			"th {background: #336699; color: #FFFFFF; text-align: left;}"
					ES_LINE_SEPARATOR);
	sbuf.append("-->" ES_LINE_SEPARATOR);
	sbuf.append("</style>" ES_LINE_SEPARATOR);
	sbuf.append("</head>" ES_LINE_SEPARATOR);
	sbuf.append(
			"<body bgcolor=\"#FFFFFF\" topmargin=\"6\" leftmargin=\"6\">"
					ES_LINE_SEPARATOR);
	sbuf.append("<hr size=\"1\" noshade>" ES_LINE_SEPARATOR);
	sbuf.append("Log session start time ").append(now.toString().c_str()).append(
			"<br>" ES_LINE_SEPARATOR);
	sbuf.append("<br>" ES_LINE_SEPARATOR);
	sbuf.append(
			"<table cellspacing=\"0\" cellpadding=\"4\" border=\"1\" bordercolor=\"#224466\" width=\"100%\">"
					ES_LINE_SEPARATOR);
	sbuf.append("<tr>" ES_LINE_SEPARATOR);
	sbuf.append("<th>Time</th>" ES_LINE_SEPARATOR);
	sbuf.append("<th>Thread</th>" ES_LINE_SEPARATOR);
	sbuf.append("<th>Level</th>" ES_LINE_SEPARATOR);
	sbuf.append("<th>Category</th>" ES_LINE_SEPARATOR);
	if (locationInfo) {
		sbuf.append("<th>File:Line</th>" ES_LINE_SEPARATOR);
	}
	sbuf.append("<th>Message</th>" ES_LINE_SEPARATOR);
	sbuf.append("</tr>" ES_LINE_SEPARATOR);
	return sbuf;
}

EString EHTMLLayout::getFooter() {
	EString sbuf;
	sbuf.append("</table>" ES_LINE_SEPARATOR);
	sbuf.append("<br>" ES_LINE_SEPARATOR);
	sbuf.append("</body></html>");
	return sbuf;
}

void EHTMLLayout::setLocationInfo(boolean flag) {
	locationInfo = flag;
}

boolean EHTMLLayout::getLocationInfo() {
	return locationInfo;
}

void EHTMLLayout::setTitle(const char* title) {
	this->title = title;
}

const char* EHTMLLayout::getTitle() {
	return title;
}

} /* namespace log */
} /* namespace efc */
