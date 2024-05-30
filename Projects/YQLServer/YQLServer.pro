#-------------------------------------------------
#
# Project created by QtCreator 2023-02-23T08:34:09
#
#-------------------------------------------------

QT       += core gui network sql serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YQLServer
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

#QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
#QMAKE_CFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
#QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

INCLUDEPATH += ./include
INCLUDEPATH += ./network
INCLUDEPATH += ./threads
INCLUDEPATH += ./ui


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    network/controllerserver.cpp \
    threads/copythread.cpp \
    threads/worker.cpp \
    ui/cutlightdlg.cpp \
    ui/iplineedit.cpp \
    ui/snbindingdialog.cpp \
    ui/logindlg.cpp \
    ui/modifypswdialog.cpp

HEADERS += \
        mainwindow.h \
    version.h \
    include/commondef.h \
    include/connection.h \
    include/DccmErrorDef.h \
    include/DccmInspectionType.h \
    include/DccmSocket.h \
    network/controllerserver.h \
    threads/copythread.h \
    threads/worker.h \
    ui/cutlightdlg.h \
    ui/iplineedit.h \
    ui/snbindingdialog.h \
    ui/logindlg.h \
    ui/modifypswdialog.h

FORMS += \
        mainwindow.ui \
    ui/cutlightdlg.ui \
    ui/snbindingdialog.ui \
    ui/logindlg.ui \
    ui/modifypswdialog.ui

RESOURCES += \
    res.qrc

CONFIG += resources_big

CONFIG += debug_and_release
CONFIG(debug,debug|release){
    DEFINES += DEBUG

} else {

}

RC_ICONS = logo.ico
RC_FILE += resource.rc

VERSION = 1.0.0.4

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
