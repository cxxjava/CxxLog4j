/*
 * ELayout.hh
 *
 *  Created on: 2015-7-24
 *      Author: cxxjava@163.com
 */

#ifndef ELAYOUT_HH_
#define ELAYOUT_HH_

#include "Efc.hh"
#include "./ELogEvent.hh"

namespace efc {
namespace log {

interface ELayout {
	virtual ~ELayout(){}

	virtual const char* getName() = 0;

    /**
     * Implement this method to create your own layout format.
     * @param event The LoggingEvent.
     * @return The formatted LoggingEvent.
     */
    virtual EString format(ELogEvent& event) = 0;

    /**
     * Returns the content type output by this layout. The base class
     * returns "text/plain".
     * @return the type of content rendered by the Layout.
     */
    virtual EString getContentType() = 0;

    /**
     * Returns the header for the layout format. The base class returns
     * <code>null</code>.
     * @return The header.
     */
    virtual EString getHeader() = 0;

    /**
     * Returns the footer for the layout format. The base class returns
     * <code>null</code>.
     * @return The footer.
     */
    virtual EString getFooter() = 0;
};

} /* namespace log */
} /* namespace efc */
#endif /* ELAYOUT_HH_ */
