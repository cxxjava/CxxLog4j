/*
 * EDailyRollingFileAppender.hh
 *
 *  Created on: 2015-8-23
 *      Author: cxxjava@163.com
 */

#ifndef EDAILYROLLINGFILEAPPENDER_HH_
#define EDAILYROLLINGFILEAPPENDER_HH_

#include "./ERollingFileAppender.hh"

namespace efc {
namespace log {

/**
   DailyRollingFileAppender extends {@link FileAppender} so that the
   underlying file is rolled over at a user chosen frequency.

   DailyRollingFileAppender has been observed to exhibit
   synchronization issues and data loss.  The log4j extras
   companion includes alternatives which should be considered
   for new deployments and which are discussed in the documentation
   for org.apache.log4j.rolling.RollingFileAppender.

   <p>The rolling schedule is specified by the <b>DatePattern</b>
   option. This pattern should follow the {@link SimpleDateFormat}
   conventions. In particular, you <em>must</em> escape literal text
   within a pair of single quotes. A formatted version of the date
   pattern is used as the suffix for the rolled file name.

   <p>For example, if the <b>File</b> option is set to
   <code>/foo/bar.log</code> and the <b>DatePattern</b> set to
   <code>'.'yyyy-MM-dd</code>, on 2001-02-16 at midnight, the logging
   file <code>/foo/bar.log</code> will be copied to
   <code>/foo/bar.log.2001-02-16</code> and logging for 2001-02-17
   will continue in <code>/foo/bar.log</code> until it rolls over
   the next day.

   <p>Is is possible to specify monthly, weekly, half-daily, daily,
   hourly, or minutely rollover schedules.

   <p><table border="1" cellpadding="2">
   <tr>
   <th>DatePattern</th>
   <th>Rollover schedule</th>
   <th>Example</th>

   <tr>
   <td><code>'.'yyyy-MM</code>
   <td>Rollover at the beginning of each month</td>

   <td>At midnight of May 31st, 2002 <code>/foo/bar.log</code> will be
   copied to <code>/foo/bar.log.2002-05</code>. Logging for the month
   of June will be output to <code>/foo/bar.log</code> until it is
   also rolled over the next month.

   <tr>
   <td><code>'.'yyyy-ww</code>

   <td>Rollover at the first day of each week. The first day of the
   week depends on the locale.</td>

   <td>Assuming the first day of the week is Sunday, on Saturday
   midnight, June 9th 2002, the file <i>/foo/bar.log</i> will be
   copied to <i>/foo/bar.log.2002-23</i>.  Logging for the 24th week
   of 2002 will be output to <code>/foo/bar.log</code> until it is
   rolled over the next week.

   <tr>
   <td><code>'.'yyyy-MM-dd</code>

   <td>Rollover at midnight each day.</td>

   <td>At midnight, on March 8th, 2002, <code>/foo/bar.log</code> will
   be copied to <code>/foo/bar.log.2002-03-08</code>. Logging for the
   9th day of March will be output to <code>/foo/bar.log</code> until
   it is rolled over the next day.

   <tr>
   <td><code>'.'yyyy-MM-dd-a</code>

   <td>Rollover at midnight and midday of each day.</td>

   <td>At noon, on March 9th, 2002, <code>/foo/bar.log</code> will be
   copied to <code>/foo/bar.log.2002-03-09-AM</code>. Logging for the
   afternoon of the 9th will be output to <code>/foo/bar.log</code>
   until it is rolled over at midnight.

   <tr>
   <td><code>'.'yyyy-MM-dd-HH</code>

   <td>Rollover at the top of every hour.</td>

   <td>At approximately 11:00.000 o'clock on March 9th, 2002,
   <code>/foo/bar.log</code> will be copied to
   <code>/foo/bar.log.2002-03-09-10</code>. Logging for the 11th hour
   of the 9th of March will be output to <code>/foo/bar.log</code>
   until it is rolled over at the beginning of the next hour.


   <tr>
   <td><code>'.'yyyy-MM-dd-HH-mm</code>

   <td>Rollover at the beginning of every minute.</td>

   <td>At approximately 11:23,000, on March 9th, 2001,
   <code>/foo/bar.log</code> will be copied to
   <code>/foo/bar.log.2001-03-09-10-22</code>. Logging for the minute
   of 11:23 (9th of March) will be output to
   <code>/foo/bar.log</code> until it is rolled over the next minute.

   </table>

   <p>Do not use the colon ":" character in anywhere in the
   <b>DatePattern</b> option. The text before the colon is interpeted
   as the protocol specificaion of a URL which is probably not what
   you want.
*/

class EDailyRollingFileAppender: public efc::log::ERollingFileAppender {
public:
	EDailyRollingFileAppender(EConfiguration* conf, EConfig* props, EString& prefix, ELogger::Level threshold);

	EString& getDatePattern();

protected:
	/**
	 The date pattern. By default, the pattern is set to
	 "'.'yyyy-MM-dd" meaning daily rollover.
	 */
	EString datePattern_;
};

} /* namespace log */
} /* namespace efc */
#endif /* EDAILYROLLINGFILEAPPENDER_HH_ */
