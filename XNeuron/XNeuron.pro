#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T15:13:28
#
#-------------------------------------------------

QT       -= gui

TARGET = XNeuron
TEMPLATE = lib

CONFIG += c++11

DEFINES += XNEURON_LIBRARY

SOURCES += xneuron.cpp

HEADERS += xneuron.h\
        xneuron_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
