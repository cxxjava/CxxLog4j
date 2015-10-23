################OPTION###################
# release or debug
VERTYPE=RELEASE

KERNEL:=$(shell uname)
LIBDIR = linux

ARCH:=$(shell uname -m)
RC:=$(ARCH)
BIT32:=i686
BIT64:=x86_64

$(info KERNEL=$(KERNEL))
$(info ARCH=$(ARCH))

ifeq ($(KERNEL),Darwin)
    LIBDIR = osx
endif

ifeq ($(RC),$(BIT32))
	SHAREDLIB = -lefc32 -leso32 -ldl -lpthread -lcrypto -lrt
else
#	SHAREDLIB = -lefc64 -leso64 -ldl -lpthread -lcrypto -lrt
	SHAREDLIB = -lefc64 -leso64 -ldl -lpthread -lcrypto
endif

ifeq ($(VERTYPE), RELEASE)
COMPILEOPTION = -c -g -O2
OUTPUT = testlog
else
COMPILEOPTION = -c -g -DDEBUG
OUTPUT = testlog_r
endif

CCOMPILE = gcc
CPPCOMPILE = g++
INCLUDEDIR = -I../../cxxjdk/inc/comm \
	-I../../cxxjdk/inc/libc \
	-I../../cxxjdk/inc/efc \
	-I../../cxxjdk/inc/efc/concurrent \
	-I../ \
	-I../inc

LINK = g++
LINKOPTION = -std=c++11 -g -o ./$(OUTPUT)
LIBDIRS = -L../../cxxjdk/lib/$(LIBDIR)
APPENDLIB = 

OBJS = 	testlog.o \
	../src/EAbstractAppender.o \
	../src/EAbstractLayout.o \
	../src/EAppenderFactory.o \
	../src/EConfiguration.o \
	../src/EConfigurator.o \
	../src/EConsoleAppender.o \
	../src/EDailyRollingFileAppender.o \
	../src/EFileAppender.o \
	../src/EFileLogWriter.o \
	../src/EHTMLLayout.o \
	../src/ELayoutFactory.o \
	../src/ELogEvent.o \
	../src/ELoggerConfig.o \
	../src/ELoggerImp.o \
	../src/ELoggerManager.o \
	../src/ELoggerManagerImp.o \
	../src/EMDC.o \
	../src/ENDC.o \
	../src/EOptionConverter.o \
	../src/EPatternLayout.o \
	../src/ERollingFileAppender.o \
	../src/ESimpleLayout.o \
	../src/ETTCCLayout.o \
	../src/ETransform.o 

$(OUTPUT): $(OBJS) $(APPENDLIB)
	$(LINK) $(LINKOPTION) $(LIBDIRS) $(OBJS) $(SHAREDLIB) $(APPENDLIB)

all: clean $(OUTPUT) clear
.PRECIOUS:%.cpp %.c %.C
.SUFFIXES:
.SUFFIXES:  .c .o .cpp .ecpp .pc .ec .C .cc .cxx

.cpp.o:
	$(CPPCOMPILE) -std=c++11 -c -o $*.o $(COMPILEOPTION) $(INCLUDEDIR)  $*.cpp
	
.cc.o:
	$(CCOMPILE) -std=c++11 -c -o $*.o $(COMPILEOPTION) $(INCLUDEDIR)  $*.cx

.cxx.o:
	$(CPPCOMPILE) -std=c++11 -c -o $*.o $(COMPILEOPTION) $(INCLUDEDIR)  $*.cxx

.c.o:
	$(CCOMPILE) -c -o $*.o $(COMPILEOPTION) $(INCLUDEDIR) $*.c

.C.o:
	$(CPPCOMPILE) -c -o $*.o $(COMPILEOPTION) $(INCLUDEDIR) $*.C	

.ecpp.C:
	$(ESQL) -e $(ESQL_OPTION) $(INCLUDEDIR) $*.ecpp 
	
.ec.c:
	$(ESQL) -e $(ESQL_OPTION) $(INCLUDEDIR) $*.ec
	
.pc.cpp:
	$(PROC)  CPP_SUFFIX=cpp $(PROC_OPTION)  $*.pc

clean: 
	rm -f $(OBJS)
	rm -f ./$(OUTPUT)

clear: 
	rm -f $(OBJS)
