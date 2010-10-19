#-------------------------------------------------
#
# Project created by QtCreator 2010-09-28T13:27:10
#
#-------------------------------------------------

QT       += core gui network

TARGET = Downloader
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    qdthread.cpp \
    dthread.cpp \
    download.cpp \
    downloader.cpp \
    dratemanager.cpp

HEADERS  += dialog.h \
    qdthread.h \
    dthread.h \
    download.h \
    downloader.h \
    dratemanager.h

FORMS    += dialog.ui
