INC_FLAGS = -I. -Iinclude -I$(OPENCV3_HOME)/include -I$(SEETATECH_HOME)/include -I$(BOOST_HOME)/include
LIB_FLAGS = -L$(OPENCV3_HOME)/lib -L$(SEETATECH_HOME)/lib -L/usr/lib64

ifeq ($(OUTPUT_TARGET), -DSHARED_LIB)
TARGETS = libmtfd.so
else
TARGETS = mtfd
endif

CFLAGS = -std=c++11 -Wall

ifeq ($(DEBUG), ON)
CFLAGS += -g -O0
else
CFLAGS += -O2
endif

ifeq ($(OUTPUT_TARGET), -DSHARED_LIB)
    ifeq ($(PY), -DSUPPORT_PYTHON3)
    INC_FLAGS += -I$(PYTHON3_INC)
    else
    INC_FLAGS += -I$(PYTHON2_INC)
    endif
CFLAGS += -fPIC -shared -fno-strict-aliasing
endif

ifeq ($(HAVELOG), -DWITH_LOG4CXX)
INC_FLAGS += -I$(LOG4CXX_HOME)/include
LIB_FLAGS += -L$(LOG4CXX_HOME)/lib
LIB_LOG4CXX = -llog4cxx
endif

ifeq ($(FILECONF), -DWITH_FILECONF)
INC_FLAGS += -I$(JSONCPP_HOME)/include 
LIB_FLAGS += -L$(JSONCPP_HOME)/lib
LIB_JSONCPP = -ljsoncpp
endif

CFLAGS += -DOS_LINUX
CFLAGS += $(HAVELOG)
CFLAGS += $(FILECONF)
CFLAGS += $(OUTPUT_TARGET)
CFLAGS += $(PY)
CFLAGS += $(INC_FLAGS)
CFLAGS += $(LIB_FLAGS)

all: 
	g++ src/*.cc $(CFLAGS) -lVIPLFaceDetector500 -lVIPLPointDetector500 -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lpthread -luuid $(LIB_LOG4CXX) $(LIB_JSONCPP) -o $(TARGETS)
	cp $(TARGETS) stcrop/facecrop -f
	mv $(TARGETS) release -f

clean:
	rm release/$(TARGETS) -f
	rm stcrop/facecrop/$(TARGETS) -f


