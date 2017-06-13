#-------------------------------------------------
#
# Project created by QtCreator 2017-06-11T17:16:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Y86_2_PC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    assembler.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    assembler.h \
    client.h

FORMS    += mainwindow.ui

RESOURCES += \
    background.qrc
