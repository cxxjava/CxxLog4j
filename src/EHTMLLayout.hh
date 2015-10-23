/*
 * EHtmlLayout.hh
 *
 *  Created on: 2015-7-25
 *      Author: cxxjava@163.com
 */

#ifndef EHTMLLAYOUT_HH_
#define EHTMLLAYOUT_HH_

#include "EAbstractLayout.hh"

namespace efc {
namespace log {

/**
 * Outputs events as rows in an HTML table on an HTML page.
 * <p>
 * Appenders using this layout should have their encoding set to UTF-8 or UTF-16, otherwise events containing non ASCII
 * characters could result in corrupted log files.
 * </p>
 */

class EHTMLLayout: public EAbstractLayout {
public:
	EHTMLLayout(EConfig* props, EString& prefix);

	/**
	 * Produces a formatted string as specified by the conversion pattern.
	 */
	virtual EString format(ELogEvent& event);

	virtual EString getContentType();

	virtual EString getHeader();

	virtual EString getFooter();

	/**
	 The <b>LocationInfo</b> option takes a boolean value. By
	 default, it is set to false which means there will be no location
	 information output by this layout. If the the option is set to
	 true, then the file name and line number of the statement
	 at the origin of the log statement will be output.

	 <p>If you are embedding this layout within an {@link
	 org.apache.log4j.net.SMTPAppender} then make sure to set the
	 <b>LocationInfo</b> option of that appender as well.
	 */
	void setLocationInfo(boolean flag);

	/**
	 Returns the current value of the <b>LocationInfo</b> option.
	 */
	boolean getLocationInfo();

	/**
	 The <b>Title</b> option takes a String value. This option sets the
	 document title of the generated HTML document.

	 <p>Defaults to 'Log4J Log Messages'.
	 */
	void setTitle(const char* title);

	/**
	 Returns the current value of the <b>Title</b> option.
	 */
	const char* getTitle();

private:
	// Print no location info by default
	boolean locationInfo;// = false;

	const char *title;
};

} /* namespace log */
} /* namespace efc */
#endif /* EHTMLLAYOUT_HH_ */
