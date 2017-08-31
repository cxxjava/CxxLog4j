/*
 * EAbstractLayout.hh
 *
 *  Created on: 2015-8-4
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTLAYOUT_HH_
#define EABSTRACTLAYOUT_HH_

#include "./ELayout.hh"

namespace efc {
namespace log {

class EAbstractLayout: public ELayout {
public:
	/**
	 *
	 */
	virtual const char* getName();

	/**
	 * Returns the content type output by this layout. The base class
	 * returns "text/plain".
	 * @return the type of content rendered by the Layout.
	 */
	virtual EString getContentType();

	/**
	 * Returns the header for the layout format. The base class returns
	 * <code>null</code>.
	 * @return The header.
	 */
	virtual EString getHeader();

	/**
	 * Returns the footer for the layout format. The base class returns
	 * <code>null</code>.
	 * @return The footer.
	 */
	virtual EString getFooter();

protected:
	/**
	 *
	 */
	EString name;

	/**
	 * The header to include when the stream is opened. May be null.
	 */
	EString header;

	/**
	 * The footer to add when the stream is closed. May be null.
	 */
	EString footer;

	/**
	 * Constructs a layout with an optional header and footer.
	 *
	 * @param props log4j properties.
	 * @param Only keys having the specified prefix will be set.
	 */
	EAbstractLayout(EConfig* props, EString& prefix);
};

} /* namespace log */
} /* namespace efc */
#endif /* EABSTRACTLAYOUT_HH_ */
