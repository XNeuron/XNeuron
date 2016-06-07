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
    ../XNeuron/xneuron.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../XNeuron/xneuron.h
