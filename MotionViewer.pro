#-------------------------------------------------
#
# Project created by QtCreator 2015-12-18T19:33:40
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(3rdparty/qtxlsx/src/xlsx/qtxlsx.pri)

TARGET = MotionViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    motionviewerwidget.cpp \
    motiongeometryengine.cpp \
    boxgeometryengine.cpp \
    motion.cpp \
    pose.cpp \
    drawconfigure.cpp \
    planegeometryengine.cpp \
    motionbuilder.cpp

HEADERS  += mainwindow.h \
    motionviewerwidget.h \
    motiongeometryengine.h \
    boxgeometryengine.h \
    motion.h \
    pose.h \
    drawconfigure.h \
    planegeometryengine.h \
    motionbuilder.h

FORMS    +=

RESOURCES += \
    shaders.qrc \
    textures.qrc
