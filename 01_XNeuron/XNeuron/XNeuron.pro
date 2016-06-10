#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T15:13:28
#
#-------------------------------------------------

QT       -= gui

TARGET = XNeuron
TEMPLATE = lib

CONFIG -= std
CONFIG += c++11
QMAKE_CXXFLAGS = -std=c++11

QMAKE_CXXFLAGS = -std=gnu++11

DEFINES += XNEURON_LIBRARY

SOURCES += xneuron.cpp \
    ActivityFunction.cpp

HEADERS += xneuron.h\
        xneuron_global.h \
    ActivityFunction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
