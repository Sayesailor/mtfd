PROJECT(mtfd)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

option(WITH_FILECONF "set parameters with config file" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CONFIGURATION_TYPES Release)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})
include_directories(${BOOST_HOME}/include)
include_directories(${LOG4CXX_HOME}/include)
include_directories(${SEETATECH_HOME}/include)
include_directories(${OPENCV_HOME}/include)

link_directories(${LOG4CXX_HOME}/lib)
link_directories(${SEETATECH_HOME}/lib)
link_directories(${OPENCV_HOME}/lib)

link_libraries("log4cxx")
link_libraries("opencv_highgui")
link_libraries("opencv_core")
link_libraries("opencv_imgproc")
link_libraries("opencv_imgcodecs")
link_libraries("seeta_facedet_lib")
link_libraries("seeta_fa_lib")
link_libraries("pthread")
link_libraries("uuid")

if (WITH_FILECONF STREQUAL "ON")
	ADD_DEFINITIONS(-DWITH_FILECONF)
	include_directories(${JSONCPP_HOME}/include)
	link_directories(${JSONCPP_HOME}/lib)
	link_libraries("jsoncpp")
endif()

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O2 -m64 -std=c++11 -DOS_LINUX")
endif()

aux_source_directory(./src DIR_SRCS)
add_executable(mtfd ${DIR_SRCS})

#set(EXTRA_LIBS ${EXTRA_LIBS} log4cxx opencv_highgui opencv_core opencv_imgproc opencv_imgcodecs seeta_facedet_lib seeta_fa_lib pthread uuid)
#target_link_libraries(mtfd ${EXTRA_LIBS})
target_link_libraries(mtfd)
