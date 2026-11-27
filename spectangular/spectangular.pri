# Common sources for the program (Spectangular.pro) and the test driver
# (tests/adapter/spectest.pro). main.cpp does not belong here.

QT += core gui widgets printsupport
CONFIG += c++17

LIBS += -larmadillo -llapack -lCCfits -lcfitsio -ldl

# Multiple cores for LSQR (OpenMP), if available
include($$PWD/openmp.pri)
# ERFA routines for the barycentric correction (BSD licence, see erfa/LICENSE)
include($$PWD/erfa/erfa.pri)

# The numerics are many times slower without optimization; optimize debug builds too.
QMAKE_CXXFLAGS_DEBUG += -O2
QMAKE_CXXFLAGS_RELEASE += -O2

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/mainwindow.cpp \
    $$PWD/plotspec.cpp \
    $$PWD/qcustomplot.cpp \
    $$PWD/correlation.cpp \
    $$PWD/edit.cpp \
    $$PWD/arithmetic.cpp \
    $$PWD/orbitfit.cpp \
    $$PWD/orbit.cpp \
    $$PWD/disentangle.cpp \
    $$PWD/dataio.cpp \
    $$PWD/neldermead.cpp \
    $$PWD/errorestimate.cpp \
    $$PWD/levmar.cpp \
    $$PWD/fluxratio.cpp \
    $$PWD/rvfit.cpp \
    $$PWD/templatefit.cpp \
    $$PWD/continuum.cpp \
    $$PWD/fluxtemplate.cpp \
    $$PWD/barycorr.cpp \
    $$PWD/barycorrdialog.cpp \
    $$PWD/periodogram.cpp \
    $$PWD/linewidth.cpp \
    $$PWD/linefinder.cpp \
    $$PWD/perioddialog.cpp \
    $$PWD/resources.cpp

HEADERS += \
    $$PWD/mainwindow.h \
    $$PWD/plotspec.h \
    $$PWD/qcustomplot.h \
    $$PWD/correlation.h \
    $$PWD/edit.h \
    $$PWD/arithmetic.h \
    $$PWD/orbitfit.h \
    $$PWD/orbit.h \
    $$PWD/disentangle.h \
    $$PWD/dataio.h \
    $$PWD/neldermead.h \
    $$PWD/errorestimate.h \
    $$PWD/levmar.h \
    $$PWD/fluxratio.h \
    $$PWD/rvfit.h \
    $$PWD/templatefit.h \
    $$PWD/continuum.h \
    $$PWD/version.h \
    $$PWD/fluxtemplate.h \
    $$PWD/barycorr.h \
    $$PWD/barycorrdialog.h \
    $$PWD/periodogram.h \
    $$PWD/linewidth.h \
    $$PWD/linefinder.h \
    $$PWD/perioddialog.h \
    $$PWD/resources.h

FORMS += \
    $$PWD/mainwindow.ui \
    $$PWD/plotspec.ui \
    $$PWD/correlation.ui \
    $$PWD/edit.ui \
    $$PWD/arithmetic.ui \
    $$PWD/orbitfit.ui

RESOURCES += $$PWD/images.qrc

# Installation directory for bundled files (set by the Makefile:
# qmake SPECTANGULAR_DATADIR=/usr/local/share/spectangular ...)
!isEmpty(SPECTANGULAR_DATADIR): DEFINES += SPECTANGULAR_DATADIR=\\\"$$SPECTANGULAR_DATADIR\\\"
!isEmpty(SPECTANGULAR_DOCDIR): DEFINES += SPECTANGULAR_DOCDIR=\\\"$$SPECTANGULAR_DOCDIR\\\"

# commit of a git checkout for the start message (version.h); a "git archive" has it filled in
SPECTANGULAR_GIT = $$system(git -C $$shell_quote($$PWD) log -1 --format=%h 2>/dev/null)
!isEmpty(SPECTANGULAR_GIT): DEFINES += SPECTANGULAR_GIT_COMMIT=\\\"$$SPECTANGULAR_GIT\\\"
