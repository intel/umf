#-------------------------------------------------
#
# Project created by QtCreator 2013-07-15T16:34:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = photo-book
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutwindow.cpp \
    photographics.cpp

HEADERS  += mainwindow.hpp \
    aboutwindow.hpp \
    photographics.hpp

FORMS    += mainwindow.ui about.ui photographics.ui
