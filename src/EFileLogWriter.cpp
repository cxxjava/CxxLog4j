/*
 * EFileLogWriter.cpp
 *
 *  Created on: 2015-8-23
 *      Author: cxxjava@163.com
 */

#include "./EFileLogWriter.hh"
#include "./EFileAppender.hh"
#include "./ERollingFileAppender.hh"
#include "./EDailyRollingFileAppender.hh"

namespace efc {
namespace log {

namespace flw {
class FileLogWriterImp {
public:
	virtual ~FileLogWriterImp() {
		try {
			writeFooter();
		} catch (...) {
		}
		delete output;
		delete ofile;
	}
	FileLogWriterImp(EFile* f, EFileAppender* fa) {
		ofile = new EFile(f);
		reset(fa);
	}
	virtual void reset(EFileAppender* fa) {
		bufferedIO = fa->fileIsBufferedIO();
		bufferSize = fa->getFileBufferSize();

		ELayout* layout = fa->getLayout();
		if (layout) {
			header = layout->getHeader();
			footer = layout->getFooter();
		}

		openFile(fa->fileIsAppend(), bufferedIO, bufferSize);
	}
	virtual void write(const void* data, int len) {
		if (!data || len<=0 || !output) {
			return;
		}

		output->write(data, len);
		if (!bufferedIO) {
			output->flush();
		}
	}
	virtual void writeHeader() {
		FileLogWriterImp::write(header.c_str(), header.length());
	}
	virtual void writeFooter() {
		FileLogWriterImp::write(footer.c_str(), footer.length());
	}

protected:
	EFile* ofile;
	EOutputStream* output;

	boolean bufferedIO;
	int bufferSize;

	EString header;
	EString footer;

	void closeFile() {
		writeFooter();

		delete output;
		output = null;
	}

	void openFile(boolean append, boolean bufferedIO, int bufferSize) {
		if (bufferedIO) {
			output = new EBufferedOutputStream(new EFileOutputStream(ofile, append), bufferSize, true);
		}
		else {
			output = new EFileOutputStream(ofile, append);
		}

		writeHeader();
	}
};

class RollingFileLogWriterImp : public FileLogWriterImp {
public:
	virtual ~RollingFileLogWriterImp() {
		//
	}
	RollingFileLogWriterImp(EFile* file, ERollingFileAppender* rfa) :
		FileLogWriterImp(file, rfa),
		maxFileSize(rfa->getMaxFileSize()),
		maxBackupIndex(rfa->getMaxBackupIndex()),
		count(file->length()) {
		//
	}
	virtual void reset(ERollingFileAppender* rfa) {
		FileLogWriterImp::reset(rfa);
		maxFileSize = rfa->getMaxFileSize();
		maxBackupIndex = rfa->getMaxBackupIndex();
	}
	virtual void write(const void* data, int len) {
		if (maxBackupIndex > 0 || count < maxFileSize) {
			FileLogWriterImp::write(data, len);
		}
		checkRollover(len);
	}

protected:
	llong maxFileSize;
	int maxBackupIndex;
	llong count;

	void checkRollover(int len) {
		count += len;
		if (maxBackupIndex > 0 && maxFileSize > 0 && count >= maxFileSize) {
			count = 0;
			rollOver();
		}
	}
	void rollOver() {
		boolean renameSucceeded = true;

		// If maxBackups <= 0, then there is no file renaming to be done.
		if (maxBackupIndex > 0) {
			// Delete the oldest file, to keep Windows happy.
			EString fileName(ofile->getPath());

			EString x = fileName + '.' + maxBackupIndex;
			EFile file(x.c_str());
			if (file.exists()) {
				renameSucceeded = file.remove();
			}

			// Map {(maxBackupIndex - 1), ..., 2, 1} to {maxBackupIndex, ..., 3, 2}
			for (int i = maxBackupIndex - 1; i >= 1 && renameSucceeded; i--) {
				x = fileName + "." + i;
				EFile file(x.c_str());
				if (file.exists()) {
					x = fileName + '.' + (i + 1);
					EFile target(x.c_str());
					//printf("Renaming file " + file + " to " + target);
					renameSucceeded = file.renameTo(&target);
				}
			}

			if (renameSucceeded) {
				// Rename fileName to fileName.1
				x = fileName + '.' + 1;
				EFile target(x.c_str());

				this->closeFile(); // keep windows happy.

				//printf("Renaming file " + file + " to " + target);
				renameSucceeded = ofile->renameTo(&target);
			}
		}

		//
		//   if all renames were successful, then
		//
		if (renameSucceeded) {
			try {
				this->openFile(true, bufferedIO, bufferSize);
			} catch (EInterruptedIOException& e) {
				EThread::currentThread()->interrupt();
			} catch (EIOException& e) {
				//printf("setFile(" + fileName + ", false) call failed.", e);
			}
		}
	}
};

class DailyRollingFileLogWriterImp : public FileLogWriterImp {
private:
	/**
	 *  RollingCalendar is a helper class to DailyRollingFileAppender.
	 *  Given a periodicity type and the current time, it computes the
	 *  start of the next interval.
	 * */
	class RollingCalendar: public ECalendar {
	public:
		RollingCalendar() :
				ECalendar(), type(DailyRollingFileLogWriterImp::TOP_OF_DAY) {
		}

		RollingCalendar(llong millis, int timezone) :
				ECalendar(millis, timezone), type(
						DailyRollingFileLogWriterImp::TOP_OF_DAY) {
		}

		void setType(int type) {
			this->type = type;
		}

		llong getNextCheckMillis(EDate& now) {
			return getNextCheckDate(now).getTime();
		}

		EDate getNextCheckDate(EDate& now) {
			this->setTime(now);

			switch (type) {
			case DailyRollingFileLogWriterImp::TOP_OF_MINUTE:
				this->set(ECalendar::SECOND, 0);
				this->set(ECalendar::MILLISECOND, 0);
				this->add(ECalendar::MINUTE, 1);
				break;
			case DailyRollingFileLogWriterImp::TOP_OF_HOUR:
				this->set(ECalendar::MINUTE, 0);
				this->set(ECalendar::SECOND, 0);
				this->set(ECalendar::MILLISECOND, 0);
				this->add(ECalendar::HOUR_OF_DAY, 1);
				break;
			case DailyRollingFileLogWriterImp::HALF_DAY:
			{
				this->set(ECalendar::MINUTE, 0);
				this->set(ECalendar::SECOND, 0);
				this->set(ECalendar::MILLISECOND, 0);
				int hour = get(ECalendar::HOUR_OF_DAY);
				if (hour < 12) {
					this->set(ECalendar::HOUR_OF_DAY, 12);
				} else {
					this->set(ECalendar::HOUR_OF_DAY, 0);
					this->add(ECalendar::DAY_OF_MONTH, 1);
				}
			}
				break;
			case DailyRollingFileLogWriterImp::TOP_OF_DAY:
				this->set(ECalendar::HOUR_OF_DAY, 0);
				this->set(ECalendar::MINUTE, 0);
				this->set(ECalendar::SECOND, 0);
				this->set(ECalendar::MILLISECOND, 0);
				this->add(ECalendar::DATE, 1);
				break;
			case DailyRollingFileLogWriterImp::TOP_OF_WEEK:
				this->set(ECalendar::DAY_OF_WEEK, getFirstDayOfWeek());
				this->set(ECalendar::HOUR_OF_DAY, 0);
				this->set(ECalendar::MINUTE, 0);
				this->set(ECalendar::SECOND, 0);
				this->set(ECalendar::MILLISECOND, 0);
				this->add(ECalendar::WEEK_OF_YEAR, 1);
				break;
			case DailyRollingFileLogWriterImp::TOP_OF_MONTH:
				this->set(ECalendar::DATE, 1);
				this->set(ECalendar::HOUR_OF_DAY, 0);
				this->set(ECalendar::MINUTE, 0);
				this->set(ECalendar::SECOND, 0);
				this->set(ECalendar::MILLISECOND, 0);
				this->add(ECalendar::MONTH, 1);
				break;
			default:
				throw EIllegalStateException(__FILE__, __LINE__, "Unknown periodicity type.");
			}
			return getTime();
		}
	private:
		int type;
	};

public:
	virtual ~DailyRollingFileLogWriterImp() {
		//
	}
	DailyRollingFileLogWriterImp(EFile* file, EDailyRollingFileAppender* drfa) :
		FileLogWriterImp(file, drfa),
		datePattern(drfa->getDatePattern()),
		checkPeriod(TOP_OF_DAY) {
		rc.setType(computeCheckPeriod());
		nextCheck = rc.getNextCheckMillis(now);
	}
	virtual void reset(EDailyRollingFileAppender* drfa) {
		FileLogWriterImp::reset(drfa);
		datePattern = drfa->getDatePattern();
		rc.setType(computeCheckPeriod());
		nextCheck = rc.getNextCheckMillis(now);
	}
	virtual void write(const void* data, int len) {
		FileLogWriterImp::write(data, len);
		checkRollover(len);
	}

protected:
	EString datePattern;

	void checkRollover(int len) {
		llong n = ESystem::currentTimeMillis();
		if (n >= nextCheck) {
			try {
				rollOver();
				now.setTime(n);
				nextCheck = rc.getNextCheckMillis(now);
			} catch (EInterruptedIOException& e) {
				EThread::currentThread()->interrupt();
			} catch (EIOException& e) {
				//printf("rollOver() failed.", e);
			}
		}
	}
	void rollOver() {
		boolean renameSucceeded = true;

		// Delete the oldest file, to keep Windows happy.
		EString fileName(ofile->getPath());

		EString targetName = fileName + datePatternConvert();
		EFile targetFile(targetName.c_str());
		if (targetFile.exists()) {
			renameSucceeded = targetFile.remove();
		}

		if (renameSucceeded) {
			// Rename fileName to targetName

			this->closeFile(); // keep windows happy.

			renameSucceeded = ofile->renameTo(&targetFile);
		}

		//
		//   if all renames were successful, then
		//
		if (renameSucceeded) {
			try {
				this->openFile(true, bufferedIO, bufferSize);
			} catch (EInterruptedIOException& e) {
				EThread::currentThread()->interrupt();
			} catch (EIOException& e) {
				//printf("setFile(" + fileName + ", false) call failed.", e);
			}
		}
	}

	EString datePatternConvert() {
		EString prefix;
		int start = 0;
		int end = 0;

		if (datePattern[0] == '\'') {
			start = 1;
		}

		end = datePattern.lastIndexOf('\'');
		if (end > start) {
			prefix = datePattern.substring(start, end);
		}

		EString fmt2 = datePattern.substring(ES_MAX(end+1, start));
		fmt2.replace("yyyy", "%Y"); fmt2.replace("yy", "%y");
		fmt2.replace("MM", "%m");
		fmt2.replace("dd", "%d");
		fmt2.replace("HH", "%H");
		fmt2.replace("mm", "%M");

		ECalendar cal;
		cal.setTime(now);
		EString str = cal.toString(fmt2.c_str());
		if (str.endsWith("ww")) {
			int w = cal.get(ECalendar::WEEK_OF_YEAR);
			return prefix + str.substring(0, str.length() - 2) + w;
		}
		else if (str.endsWith("a")) {
			int am_pm = cal.get(ECalendar::AM_PM);
			return prefix + str.substring(0, str.length() - 1) + (am_pm == ECalendar::AM ? "A" : "P");
		}
		else {
			return prefix + str;
		}
	}

	int computeCheckPeriod() {
		if (datePattern.isEmpty()) {
			datePattern = "'.'yyyy-MM-dd"; //use default pattern.
		}

		if (datePattern.endsWith("mm")) {
			return TOP_OF_MINUTE;
		}
		else if (datePattern.endsWith("HH")) {
			return TOP_OF_HOUR;
		}
		else if (datePattern.endsWith("a")) {
			return HALF_DAY;
		}
		else if (datePattern.endsWith("dd")) {
			return TOP_OF_DAY;
		}
		else if (datePattern.endsWith("ww")) {
			return TOP_OF_WEEK;
		}
		else if (datePattern.endsWith("MM")) {
			return TOP_OF_MONTH;
		}
		return TOP_OF_DAY; // default!
	}

private:
	// The code assumes that the following constants are in a increasing
	// sequence.
	static const int TOP_OF_MINUTE = 0;
	static const int TOP_OF_HOUR   = 1;
	static const int HALF_DAY      = 2;
	static const int TOP_OF_DAY    = 3;
	static const int TOP_OF_WEEK   = 4;
	static const int TOP_OF_MONTH  = 5;

	int checkPeriod;

	/**
	 * The next time we estimate a rollover should occur.
	 */
	llong nextCheck;

	EDate now;
	RollingCalendar rc;
};
}
using namespace flw;

EFileLogWriter::~EFileLogWriter() {
	delete imp;
	delete ofile;
}

EFileLogWriter::EFileLogWriter(EFile* file, EFileAppender* fa) :
		imp(null) {
	ES_ASSERT(file);
	ES_ASSERT(fa);

	//from sub class to super class.

	if (imp == null) {
		EDailyRollingFileAppender* drfa = dynamic_cast<EDailyRollingFileAppender*>(fa);
		if (drfa) {
			emType = DAILY_ROLLING_FILE_APPENDER;
			imp = new DailyRollingFileLogWriterImp(file, drfa);
		}
	}

	if (imp == null) {
		ERollingFileAppender* rfa = dynamic_cast<ERollingFileAppender*>(fa);
		if (rfa) {
			emType = ROLLING_FILE_APPENDER;
			imp = new RollingFileLogWriterImp(file, rfa);
		}
	}

	if (imp == null) { //default is ".FileAppender"
		emType = FILE_APPENDER;
		imp = new FileLogWriterImp(file, fa);
	}

	ofile = new EFile(file);
}

void EFileLogWriter::reset(EFileAppender* fa) {
	try {
		SYNCHRONIZED(this) {
			//from sub class to super class.

			EDailyRollingFileAppender* drfa = dynamic_cast<EDailyRollingFileAppender*>(fa);
			if (drfa) {
				if (emType == DAILY_ROLLING_FILE_APPENDER) {
					static_cast<DailyRollingFileLogWriterImp*>(imp)->reset(drfa);
					return;
				}
				else {
					delete imp; //!
					emType = DAILY_ROLLING_FILE_APPENDER;
					imp = new DailyRollingFileLogWriterImp(ofile, drfa);
					return;
				}
			}

			ERollingFileAppender* rfa = dynamic_cast<ERollingFileAppender*>(fa);
			if (rfa) {
				if (emType == ROLLING_FILE_APPENDER) {
					static_cast<RollingFileLogWriterImp*>(imp)->reset(rfa);
					return;
				}
				else {
					delete imp; //!
					emType = ROLLING_FILE_APPENDER;
					imp = new RollingFileLogWriterImp(ofile, rfa);
					return;
				}
			}

			if (emType == FILE_APPENDER) {
				static_cast<FileLogWriterImp*>(imp)->reset(fa);
				return;
			}
			else {
				delete imp; //!
				emType = FILE_APPENDER;
				imp = new FileLogWriterImp(ofile, fa);
				return;
			}
        }}
	} catch (EIOException& e) {
	}
}

void EFileLogWriter::write(const void* data, int len) {
	SYNCHRONIZED(this) {
		imp->write(data, len);
    }}
}

} /* namespace log */
} /* namespace efc */
