#-------------------------------------------------
#
# Project created by QtCreator 2015-12-18T19:33:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MotionViewer
TEMPLATE = app

LIBS += -lvtkCommon -lvtksys -lQVTK -lvtkViews -lvtkWidgets -lvtkInfovis -lvtkRendering -lvtkGraphics -lvtkImaging -lvtkIO -lvtkFiltering -lvtklibxml2 -lvtkDICOMParser -lvtkpng -lvtkpng -lvtktiff -lvtkzlib -lvtkjpeg -lvtkalglib -lvtkexpat -lvtkverdict -lvtkmetaio -lvtkNetCDF -lvtksqlite -lvtkexoIIc -lvtkftgl -lvtkfreetype -lvtkHybrid

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    +=
