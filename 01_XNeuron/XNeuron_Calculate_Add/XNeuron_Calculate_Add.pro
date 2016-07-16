QT += core
QT -= gui

CONFIG += c++11

TARGET = XNeuron_Calculate_Add
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../XNeuron/ActivityFunction.cpp \
    ../XNeuron/gradienttraining.cpp \
    ../XNeuron/OnlineTraining.cpp \
    ../XNeuron/xneuron.cpp

HEADERS += \
    ../XNeuron/ActivityFunction.h \
    ../XNeuron/gradienttraining.h \
    ../XNeuron/OnlineTraining.h \
    ../XNeuron/xneuron.h
