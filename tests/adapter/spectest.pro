# Test driver: all program sources except main.cpp, plus spectest.cpp.
# SRC points to the source directory (default: ../../spectangular).
#   mkdir build && cd build && qmake ../adapter/spectest.pro && make

TEMPLATE = app
TARGET = spectest
CONFIG += console
CONFIG -= app_bundle

isEmpty(SRC): SRC = $$PWD/../../spectangular
include($$SRC/spectangular.pri)

SOURCES += spectest.cpp
