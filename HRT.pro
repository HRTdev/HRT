#-------------------------------------------------
#
# Project created by QtCreator 2012-11-22T14:35:42
#
#-------------------------------------------------

QT       += core gui opengl widgets sql

TARGET = HRT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    squareitem.cpp \
    myscene.cpp \
    basiccell.cpp \
    buffercell.cpp \
    gridcell.cpp \
    menuclass.cpp \
    openprojectmenu.cpp \
    bufgrid.cpp

HEADERS  += mainwindow.h \
    squareitem.h \
    myscene.h \
    basiccell.h \
    buffercell.h \
    gridcell.h \
    menuclass.h \
    openprojectmenu.h \
    bufgrid.h

FORMS    += mainwindow.ui

OTHER_FILES +=

RESOURCES +=

#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg
QMAKE_CXXFLAGS_RELEASE = -O0 -march=native #-fomit-frame-pointer
