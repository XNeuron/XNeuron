#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T15:14:37
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

TARGET = tst_xneurontesttest
CONFIG   += console
CONFIG   -= app_bundle

CONFIG -= std
CONFIG += c++11

QMAKE_CXXFLAGS = -std=c++11
QMAKE_CXXFLAGS *= -std=gnu++11

TEMPLATE = app


SOURCES += tst_xneurontesttest.cpp \
    ../XNeuron/ActivityFunction.cpp \
    ../XNeuron/OnlineTraining.cpp \
    ../XNeuron/xneuron.cpp \
    ../XNeuron/gradienttraining.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../XNeuron/ActivityFunction.h \
    ../XNeuron/OnlineTraining.h \
    ../XNeuron/xneuron.h \
    ../XNeuron/gradienttraining.h
