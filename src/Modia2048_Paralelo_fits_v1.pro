#-------------------------------------------------
#
# Project created by QtCreator 2012-05-10T16:16:41
#
#-------------------------------------------------

QT       += core
QT       += gui
QT       += network
QT       += widgets
QT       += core gui widgets

TARGET = Modia2048_Paralelo_v1
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    socketconnection.cpp \
    window.cpp \
    modia.cpp

HEADERS += \
    socketconnection.h \
    window.h \
    modia.h \


LIBS += -L. \
        -lcfitsio \
        -lm \
        -lnsl \
