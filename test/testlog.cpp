#include "es_main.h"
#include "Efc.hh"
#include "ELog.hh"

static sp<ELogger> rootLogger = ELoggerManager::getRootLogger();
static sp<ELogger> logger = ELoggerManager::getLogger("XXX.YYY.ZZZ");
static void test_logger() {
	ELoggerManager::init("log4e.properties");
//	ELoggerManager::getRootLogger()->setLevel(ELogger::LEVEL_OFF);

	try {

		logger->info("xxxxxxxxxxxxxx");
		logger->warn_("xxxxxxxxxxxxxx%s", "z");

		throw EException(__FILE__, __LINE__, "exception:");
	} catch (EException& e) {
//		ELOG_E(logger, "xxxxxxxxxxxxxx[%s]", e.getMessage());
//		ELOG__E(logger, "xxxxxxxxxxxxxx", e);
		ELOG__E(logger, "message", e);
		logger->error(e);
		logger->error("message", e);
	}

	ELOG_E(logger, "format s=%d", 9999);

	ELOG_E(rootLogger, "root log s=%d", 9999);

	for (int i=0; i<5; i++) {
		ENDC::push(new EString(i));

		sp<ELogger> logger2 = ELoggerManager::getLogger("XXX");
		ELOG_E(logger2, "!@#$%%^&*(&^%@#%%&*\" i s=%d", i);
		logger2->close();

		delete ENDC::pop();
	}
	ENDC::clear();

	ELOG_E(logger, "ndc xxx");
}

static void test_logger_multi_threads() {
	ELoggerManager::init("log4e.properties");
//	ELoggerManager::getRootLogger()->setLevel(ELogger::LEVEL_OFF);

	class ThreadX : public EThread {
	public:
		virtual void run() {
			long i = 0;
			while (true) {
				ENDC ndc("abc");
				EMDC mdc("x1", "v1");
				mdc.put("x2", "v2");

				ELOG_I(logger, "i=%d", i++);
				ELOG_W(logger, "i=%d", i++);
				ELOG_E(logger, "i=%d", i++);

				EThread::sleep(100);
			}
		}
	};

	EArray<ThreadX*> arr;
	for (int i = 0; i < 10; i++) {
		ThreadX* thread = new ThreadX();
		arr.add(thread);
		thread->start();
	}

	while (true) {
		ELoggerManager::flushConfig(); //!
		EThread::sleep(50);
	}

	for (int i = 0; i < arr.size(); i++) {
		ThreadX* thread = arr.getAt(i);
		thread->join(100);
	}
}

static void test_test(int argc, const char** argv) {
//	test_logger();
	test_logger_multi_threads();
//
//	EThread::sleep(3000);
}

MAIN_IMPL(testlog) {
	printf("main()\n");

	ESystem::init(argc, argv);

	printf("inited.\n");

	int i = 0;
	try {
		boolean loop = EBoolean::parseBoolean(ESystem::getProgramArgument("loop"));
		do {
			test_test(argc, argv);

//		} while (++i < 5);
		} while (1);
	}
	catch (EException& e) {
		e.printStackTrace();
	}
	catch (...) {
		printf("catch all...\n");
	}

	printf("exit...\n");

	ESystem::exit(0);

	return 0;
}
