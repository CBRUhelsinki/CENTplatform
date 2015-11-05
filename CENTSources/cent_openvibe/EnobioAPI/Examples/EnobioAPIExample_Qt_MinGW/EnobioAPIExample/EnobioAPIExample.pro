#-------------------------------------------------
#
# Project created by QtCreator 2012-01-12T12:50:56
#
#-------------------------------------------------

QT       += core gui

TARGET = EnobioAPIExample
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += includes

LIBS += -L"$${_PRO_FILE_PWD_}/libs" -lEnobioAPI
