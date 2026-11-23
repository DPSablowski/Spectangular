# Unit tests of the GUI-free modules (without Qt GUI).
#   mkdir build-unit && cd build-unit && qmake ../unit/unittests.pro && make

TEMPLATE = app
TARGET = unittests
CONFIG += console c++17
CONFIG -= app_bundle qt

isEmpty(SRC): SRC = $$PWD/../../spectangular
INCLUDEPATH += $$SRC
LIBS += -larmadillo -llapack -ldl
include($$SRC/openmp.pri)

SOURCES += unittests.cpp \
    $$SRC/orbit.cpp \
    $$SRC/dataio.cpp \
    $$SRC/disentangle.cpp \
    $$SRC/neldermead.cpp \
    $$SRC/errorestimate.cpp \
    $$SRC/levmar.cpp \
    $$SRC/fluxratio.cpp \
    $$SRC/rvfit.cpp \
    $$SRC/templatefit.cpp \
    $$SRC/continuum.cpp \
    $$SRC/barycorr.cpp
include($$SRC/erfa/erfa.pri)
