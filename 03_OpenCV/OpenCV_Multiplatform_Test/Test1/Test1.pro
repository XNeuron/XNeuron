QT += core
QT -= gui

CONFIG += c++11

TARGET = Test1
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

install_it.path = $$OUT_PWD
install_it.files = CopyData/*

INSTALLS += install_it

INCLUDEPATH += "$$PWD/opencv/include"
LIBS += -L"$$PWD/opencv/x86/mingw/lib" \
        #-llibopencv_calib3d310      \
        -llibopencv_core310         \
        #-llibopencv_features2d310   \
        #-llibopencv_flann310        \
        -llibopencv_highgui310      \
        -llibopencv_imgcodecs310    \
        -llibopencv_imgproc310      \
        #-llibopencv_ml310           \
        #-llibopencv_objdetect310    \
        #-llibopencv_photo310        \
        #-llibopencv_shape310        \
        #-llibopencv_stitching310    \
        #-llibopencv_superres310     \
        #-llibopencv_ts310           \
        #-llibopencv_videostab310    \
        #-llibopencv_video310        \
        #-llibopencv_videostab310


#unix {
#INCLUDEPATH +=
#LIBS += -L
#}

#win32 {
#INCLUDEPATH += ".\OpenCVWin32\include"
#LIBS += -L".\OpenCVWin32\bin\*.dll"
#}

#win64 {
#INCLUDEPATH += ".\OpenCVWin64\include"
#LIBS += -L".\OpenCVWin64\bin\*.dll"
#}

DISTFILES += \
    Wall.E.png \
    libopencv_highgui310.dll \
    libopencv_imgcodecs310.dll \
    libopencv_imgproc310.dll
