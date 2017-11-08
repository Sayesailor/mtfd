<BR>there are 3rd libs request:
<BR>VIPLFaceDetector500 VIPLPointDetector500 libopencv_core libopencv_highgui libopencv_imgproc libopencv_imgcodecs libjsoncpp libpthread liblog4cxx libuuid
<BR>
<BR>make HAVELOG=-DWITH_LOG4CXX OUTPUT_TARGET=-DSHARED_LIB FILECONF=-DWITH_FILECONF
<BR>
<BR>mkdir build
<BR>cd build
<BR>cmake -DBOOST_HOME=/usr/local/boost -DJSONCPP_HOME=/usr/local/jsoncpp -DLOG4CXX_HOME=/usr/local/log4cxx -DSEETATECH_HOME=/usr/local/seetatech -DOPENCV_HOME=/usr/local/opencv -DWITH_FILECONF=ON ..
<BR>#replace -DXXX_HOME= with your path
<BR>make
