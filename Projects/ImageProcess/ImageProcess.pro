#-------------------------------------------------
#
# Project created by QtCreator 2023-06-16T09:28:44
#
#-------------------------------------------------

QT       += core gui

# 输出版本号
message(Qt Version = $$[QT_VERSION])
# 判断第一个版本号
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# 或者写为
greaterThan(QT_MAJOR_VERSION, 4) {
    message(Qt Version = $$[QT_VERSION])
} else {
    message(Qt Version = $$[QT_VERSION])
}

TARGET = ImageProcess
TEMPLATE = app

CONFIG += warn_off

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = $$PWD/../bin

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ui/logindialog.cpp \
    imageviewfram.cpp \
    algo_imageprocess.cpp

HEADERS += \
        mainwindow.h \
    ui/logindialog.h \
    imageviewfram.h \
    algo_imageprocess.h

FORMS += \
        mainwindow.ui \
    ui/logindialog.ui

RESOURCES += \
    res.qrc

# opencv config

# 判断第二位版本号
QT_VERSION = $$[QT_VERSION]
QT_VERSION = $$split(QT_VERSION, ".")
QT_VER_MAJ = $$member(QT_VERSION, 0)  # 版本号第一位
QT_VER_MIN = $$member(QT_VERSION, 1)  # 版本号第二位
QT_VER_THR = $$member(QT_VERSION, 2)  # 版本号第三位

# 如果QT版本小于5.13
lessThan(QT_VER_MAJ, 5) | lessThan(QT_VER_MIN, 13) {
    message(Qt版本小于5.13)
    INCLUDEPATH += D:/IDE/opencv249/opencv/build/include
    INCLUDEPATH += D:/IDE/opencv249/opencv/build/include/opencv2

    CONFIG(debug, debug|release): {
    LIBS += -LD:/IDE/opencv249/opencv/build/x64/vc12/lib \
    -lopencv_core249d \
    -lopencv_imgproc249d \
    -lopencv_highgui249d \
    -lopencv_ml249d \
    -lopencv_video249d \
    -lopencv_features2d249d \
    -lopencv_calib3d249d \
    -lopencv_objdetect249d \
    -lopencv_contrib249d \
    -lopencv_legacy249d \
    -lopencv_flann249d
    } else:CONFIG(release, debug|release): {
    LIBS += -LD:/IDE/opencv249/opencv/build/x64/vc12/lib \
    -lopencv_core249 \
    -lopencv_imgproc249 \
    -lopencv_highgui249 \
    -lopencv_ml249 \
    -lopencv_video249 \
    -lopencv_features2d249 \
    -lopencv_calib3d249 \
    -lopencv_objdetect249 \
    -lopencv_contrib249 \
    -lopencv_legacy249 \
    -lopencv_flann249
    }

} else {
    message(Qt版本不小于5.13)
    win32:CONFIG(release, debug|release): LIBS += -LD:/IDE/opencv452/opencv/build/x64/vc15/lib/ -lopencv_world452
    else:win32:CONFIG(debug, debug|release): LIBS += -LD:/IDE/opencv452/opencv/build/x64/vc15/lib/ -lopencv_world452d

    INCLUDEPATH += D:/IDE/opencv452/opencv/build/include
    INCLUDEPATH += D:/IDE/opencv452/opencv/build/include/opencv2
    DEPENDPATH += D:/IDE/opencv452/opencv/build/x64/vc15
}


# 判断版本号等于多少
#equals(QT_VER_MAJ, 5) | equals(QT_VER_MIN, 6) {
#    message(Qt版本等于5.6)
#}



