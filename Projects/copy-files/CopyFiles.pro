QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Forms/rawinfodialog.cpp \
    copyfilesthread.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Forms/rawinfodialog.h \
    copyfilesthread.h \
    mainwindow.h

FORMS += \
    Forms/rawinfodialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

RC_ICONS = incognito.ico


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../IDE/opencv452/opencv/build/x64/vc15/lib/ -lopencv_world452
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../IDE/opencv452/opencv/build/x64/vc15/lib/ -lopencv_world452d

INCLUDEPATH += $$PWD/../../../../../IDE/opencv452/opencv/build/include
INCLUDEPATH += $$PWD/../../../../../IDE/opencv452/opencv/build/include/opencv2
DEPENDPATH += $$PWD/../../../../../IDE/opencv452/opencv/build/x64/vc15

#INCLUDEPATH += D:/IDE/opencv249/opencv/build/include
#INCLUDEPATH += D:/IDE/opencv249/opencv/build/include/opencv2

#CONFIG(debug, debug|release): {
#LIBS += -LD:/IDE/opencv249/opencv/build/x64/vc12/lib \
#-lopencv_core249d \
#-lopencv_imgproc249d \
#-lopencv_highgui249d \
#-lopencv_ml249d \
#-lopencv_video249d \
#-lopencv_features2d249d \
#-lopencv_calib3d249d \
#-lopencv_objdetect249d \
#-lopencv_contrib249d \
#-lopencv_legacy249d \
#-lopencv_flann249d
#} else:CONFIG(release, debug|release): {
#LIBS += -LD:/IDE/opencv249/opencv/build/x64/vc12/lib \
#-lopencv_core249 \
#-lopencv_imgproc249 \
#-lopencv_highgui249 \
#-lopencv_ml249 \
#-lopencv_video249 \
#-lopencv_features2d249 \
#-lopencv_calib3d249 \
#-lopencv_objdetect249 \
#-lopencv_contrib249 \
#-lopencv_legacy249 \
#-lopencv_flann249
#}
