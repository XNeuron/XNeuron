QT += core
QT -= gui

CONFIG += c++11

TARGET = Test1
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
PKGCONFIG += opencv

SOURCES += main.cpp

INCLUDEPATH += C:/opencv-2.4.13/opencv/build/include
LIBS += "C:/opencv-2.4.13/opencv/build/x86/vc12/bin/"

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
