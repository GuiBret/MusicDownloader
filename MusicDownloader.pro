#-------------------------------------------------
#
# Project created by QtCreator 2016-10-11T17:15:20
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicDownloader
TEMPLATE = app


SOURCES += main.cpp\
        launcher.cpp \
    downloaddisplay.cpp \
    downloadprofile.cpp \
    utils.cpp \
    myprocess.cpp

HEADERS  += launcher.h \
    downloaddisplay.h \
    downloadprofile.h \
    utils.h \
    myprocess.h

FORMS    += launcher.ui \
    downloaddisplay.ui \
    downloadprofile.ui
