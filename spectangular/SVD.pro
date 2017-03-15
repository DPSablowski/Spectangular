#-------------------------------------------------
#
# Project created by QtCreator 2015-09-24T10:33:26
#
#-------------------------------------------------

QT       += core gui
QT += multimedia
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Spectangular
TEMPLATE = app
LIBS += -larmadillo
#LIBS += -larpack++
LIBS += -llapack
LIBS += -lcfitsio
LIBS += -lCCfits
LIBS += -lm
LIBS += -lstdc++
#LIBS += -lopenblas

QMAKE_CFLAGS_RELEASE    = -O2
QMAKE_CXXFLAGS += -std=c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    plotspec.cpp \
    qcustomplot.cpp \
    correlation.cpp \
    edit.cpp \
    simplesubtraction.cpp \
    arithmetic.cpp \
    orbitfit.cpp

HEADERS  += mainwindow.h \
    plotspec.h \
    qcustomplot.h \
    correlation.h \
    edit.h \
    simplesubtraction.h \
    arithmetic.h \
    orbitfit.h

FORMS    += mainwindow.ui \
    plotspec.ui \
    correlation.ui \
    edit.ui \
    simplesubtraction.ui \
    arithmetic.ui \
    orbitfit.ui

RESOURCES += \
    images.qrc
