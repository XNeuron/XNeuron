#-------------------------------------------------
#
# Project created by QtCreator 2011-12-28T14:43:25
#
#-------------------------------------------------

TARGET = ShowImage
TEMPLATE = app
QT += widgets

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

//PKGCONFIG += opencv
CONFIG += link_pkgconfig

INCLUDEPATH += include
LIBS += -Llib
-opencv_world310.lib
-opencv_world310d.lib

