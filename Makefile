INC_FLAGS = -Iinclude -I$(OPENCV3_HOME)/include -I$(JSONCPP_HOME)/include -I$(LOG4CXX_HOME)/include -I$(SEETATECH_HOME)/include -I$(BOOST_HOME)/include
LIB_FLAGS = -L$(OPENCV3_HOME)/lib -L$(JSONCPP_HOME)/lib -L$(LOG4CXX_HOME)/lib -L$(SEETATECH_HOME)/lib 

TARGETS = mtfd
CFLAGS = -std=c++11 -O2 -Wall
CFLAGS += -DOS_LINUX
CFLAGS += $(HAVELOG)
CFLAGS += $(FILECONF)
CFLAGS += -o $(TARGETS)
CFLAGS += $(INC_FLAGS)
CFLAGS += $(LIB_FLAGS)

ifeq ($(HAVELOG), -DWITH_LOG4CXX)
LIB_LOG4CXX = -llog4cxx
endif

ifeq ($(FILECONF), -DWITH_FILECONF)
LIB_JSONCPP = -ljsoncpp
endif

all: 
	g++ src/*.cc $(CFLAGS) -lseeta_facedet_lib -lseeta_fa_lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lpthread -luuid $(LIB_LOG4CXX) $(LIB_JSONCPP)
	mv $(TARGETS) release -f

clean:
	rm release/$(TARGETS) -f

