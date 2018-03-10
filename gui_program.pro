#-------------------------------------------------
#
# Project created by QtCreator 2018-02-16T00:27:59
#
#-------------------------------------------------

QT       += core gui \
            qml \
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(qtrest-master/com_github_kafeg_qtrest.pri)

TARGET = gui_program
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    riotapiloginscreen.cpp \
    riotapi.cpp \
    filedownloader.cpp

HEADERS += \
        mainwindow.h \
    riotapiloginscreen.h \
    riotapi.h \
    filedownloader.h

FORMS += \
        mainwindow.ui \
    riotapiloginscreen.ui

DISTFILES +=
