#-------------------------------------------------
#
# Project created by QtCreator 2018-05-15T15:29:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yqfind
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += C:/mysoftware/opencvmingw/include
INCLUDEPATH += C:/mysoftware/opencvmingw/include/opencv
INCLUDEPATH += C:/mysoftware/opencvmingw/include/opencv2
DEPENDPATH += C:/mysoftware/opencvmingw/include

win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_core320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_highgui320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_imgproc320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_calib3d320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_features2d320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_video320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_videoio320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_videostab320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_flann320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_objdetect320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_photo320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_imgcodecs320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_superres320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_shape320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_stitching320.dll
win32: LIBS += -LC:/mysoftware/opencvmingw/lib/ -llibopencv_ml320.dll
