PROJECT(mtfd)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

option(WITH_FILECONF "set parameters with config file" OFF)
option(WITH_SHARED "set parameters with config file" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CONFIGURATION_TYPES Release)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${BOOST_HOME}/include)
include_directories(${SEETATECH_HOME}/include)
include_directories(${OPENCV3_HOME}/include)

link_directories(${SEETATECH_HOME}/lib)
link_directories(${OPENCV3_HOME}/lib)
link_directories(${PY_HOME}/lib)

link_libraries("opencv_highgui")
link_libraries("opencv_core")
link_libraries("opencv_imgproc")
link_libraries("opencv_imgcodecs")
link_libraries("VIPLFaceDetector500")
link_libraries("VIPLPointDetector500")
link_libraries("pthread")
link_libraries("uuid")

if (WITH_LOG STREQUAL "ON")
	ADD_DEFINITIONS(-DWITH_LOG4CXX)
	include_directories(${LOG4CXX_HOME}/include)
	link_directories(${LOG4CXX_HOME}/lib)
	link_libraries("log4cxx")
endif()

if (WITH_FILECONF STREQUAL "ON")
	ADD_DEFINITIONS(-DWITH_FILECONF)
	include_directories(${JSONCPP_HOME}/include)
	link_directories(${JSONCPP_HOME}/lib)
	link_libraries("jsoncpp")
endif()

if (WITH_SHARED STREQUAL "ON")
	ADD_DEFINITIONS(-DSHARED_LIB)
	include_directories(${PY_HOME}/include/python2.7)
	link_directories(${PY_HOME}/lib)
	link_libraries("python2.7")
endif()

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O2 -m64 -DOS_LINUX")
endif()

aux_source_directory(./src DIR_SRCS)
add_executable(mtfd ${DIR_SRCS})

target_link_libraries(mtfd)
