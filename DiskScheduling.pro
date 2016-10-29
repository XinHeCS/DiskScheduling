#-------------------------------------------------
#
# Project created by QtCreator 2016-04-01T17:16:50
#
#-------------------------------------------------

QT       += core gui
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#qtHaveModule(printsupport): QT += printsupport

TARGET = DiskScheduling
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui
