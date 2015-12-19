#-------------------------------------------------
#
# Project created by QtCreator 2015-12-18T19:33:40
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MotionViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    motionviewerwidget.cpp \
    motiongeometryengine.cpp \
    boxgeometryengine.cpp

HEADERS  += mainwindow.h \
    motionviewerwidget.h \
    motiongeometryengine.h \
    boxgeometryengine.h

FORMS    +=

RESOURCES += \
    shaders.qrc \
    textures.qrc
