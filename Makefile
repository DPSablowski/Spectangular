# Spectangular - build and install on Linux without Qt Creator.
#
#   make                      build (directory build/)
#   sudo make install         install to /usr/local
#   make install PREFIX=$HOME/.local     without root, into your own home
#   make check                test suite (Python 3 with numpy; FITS tests with astropy)
#   make uninstall            remove installed files (same PREFIX as for install)
#   make deps                 check whether Qt 5, Armadillo, CCfits and cfitsio are present
#   make clean / distclean    clean up / delete the build directory
#   make manual               rebuild Spectangular_Manual.pdf from doc/manual (needs LaTeX)
#
# Variables: PREFIX (/usr/local), DESTDIR (for packages), BUILDDIR (build),
#            QMAKE (Qt 5 qmake, searched for), JOBS (number of parallel compiler processes).
# Qt Creator can still build the project directly via spectangular/Spectangular.pro.

PREFIX   ?= /usr/local
DESTDIR  ?=
BUILDDIR ?= build
JOBS     ?= $(shell nproc 2>/dev/null || echo 2)

BINDIR   = $(PREFIX)/bin
DATADIR  = $(PREFIX)/share/spectangular
DOCDIR   = $(PREFIX)/share/doc/spectangular
APPSDIR  = $(PREFIX)/share/applications
ICONDIR  = $(PREFIX)/share/pixmaps

# Search for the Qt 5 qmake (plain qmake may be Qt 6 on some systems)
QMAKE ?= $(shell for q in qmake-qt5 /usr/lib/qt5/bin/qmake /usr/lib/x86_64-linux-gnu/qt5/bin/qmake \
                         /usr/lib64/qt5/bin/qmake qmake; do \
                   command -v $$q >/dev/null 2>&1 && $$q -query QT_VERSION 2>/dev/null | grep -q '^5\.' \
                   && { command -v $$q; break; }; done)

SRC     = spectangular
PROGRAM = $(BUILDDIR)/Spectangular
PKGS    = armadillo CCfits cfitsio

.PHONY: all deps install uninstall check clean distclean help manual

all: $(PROGRAM)

help:
	@sed -n '2,15p' Makefile | sed 's/^# \{0,1\}//'

deps:
	@ok=1; \
	if [ -z "$(QMAKE)" ]; then echo "missing: qmake for Qt 5"; ok=0; \
	else echo "Qt 5:      $(QMAKE) (Qt $$($(QMAKE) -query QT_VERSION))"; fi; \
	for p in $(PKGS); do \
	  if pkg-config --exists $$p 2>/dev/null; then echo "$$p: $$(pkg-config --modversion $$p)"; \
	  else echo "missing: $$p"; ok=0; fi; \
	done; \
	if [ $$ok = 0 ]; then \
	  echo ""; \
	  echo "Debian/Ubuntu: sudo apt install build-essential qtbase5-dev libarmadillo-dev libccfits-dev libopenblas0-pthread pkg-config"; \
	  echo "Fedora:        sudo dnf install gcc-c++ make qt5-qtbase-devel armadillo-devel CCfits-devel openblas pkgconf"; \
	  echo "openSUSE:      sudo zypper install gcc-c++ make libqt5-qtbase-devel armadillo-devel CCfits-devel openblas-devel pkg-config"; \
	  exit 1; \
	fi

$(BUILDDIR)/Makefile: $(SRC)/Spectangular.pro $(SRC)/spectangular.pri
	@$(MAKE) --no-print-directory deps
	mkdir -p $(BUILDDIR)
	cd $(BUILDDIR) && $(QMAKE) ../$(SRC)/Spectangular.pro CONFIG+=release \
	    SPECTANGULAR_DATADIR=$(DATADIR) SPECTANGULAR_DOCDIR=$(DOCDIR)

# Always pass on to make in the build directory; it checks itself what needs rebuilding
$(PROGRAM): $(BUILDDIR)/Makefile FORCE
	$(MAKE) -C $(BUILDDIR) -j$(JOBS)

FORCE:

install: $(PROGRAM)
	install -d $(DESTDIR)$(BINDIR) $(DESTDIR)$(DATADIR) $(DESTDIR)$(DOCDIR) \
	           $(DESTDIR)$(APPSDIR) $(DESTDIR)$(ICONDIR)
	install -m 755 $(PROGRAM) $(DESTDIR)$(BINDIR)/Spectangular
	install -m 644 $(SRC)/BinaryData.dat $(DESTDIR)$(DATADIR)/BinaryData.dat
	install -m 644 Spectangular_Manual.pdf $(DESTDIR)$(DOCDIR)/Spectangular_Manual.pdf
	install -m 644 README.md CHANGELOG.md LICENSE $(DESTDIR)$(DOCDIR)/
	install -m 644 resources/Logo.png $(DESTDIR)$(ICONDIR)/spectangular.png
	install -m 644 resources/spectangular.desktop $(DESTDIR)$(APPSDIR)/spectangular.desktop
	@echo ""
	@echo "Installed: $(DESTDIR)$(BINDIR)/Spectangular"
	@case ":$$PATH:" in *":$(BINDIR):"*) ;; \
	  *) echo "Note: $(BINDIR) is not in PATH; e.g. in ~/.bashrc: export PATH=\"$(BINDIR):\$$PATH\"";; esac

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/Spectangular
	rm -f $(DESTDIR)$(APPSDIR)/spectangular.desktop $(DESTDIR)$(ICONDIR)/spectangular.png
	rm -rf $(DESTDIR)$(DATADIR) $(DESTDIR)$(DOCDIR)

# Test suite: builds test driver and unit tests in $(BUILDDIR)/tests and runs them
check: $(PROGRAM)
	mkdir -p $(BUILDDIR)/tests/adapter $(BUILDDIR)/tests/unit
	cd $(BUILDDIR)/tests/adapter && $(QMAKE) ../../../tests/adapter/spectest.pro && $(MAKE) -j$(JOBS)
	cd $(BUILDDIR)/tests/unit && $(QMAKE) ../../../tests/unit/unittests.pro && $(MAKE) -j$(JOBS)
	cd tests && SPECTANGULAR=$(CURDIR)/$(PROGRAM) SPECTEST=$(CURDIR)/$(BUILDDIR)/tests/adapter/spectest \
	    UNITTESTS=$(CURDIR)/$(BUILDDIR)/tests/unit/unittests python3 run_tests.py

# User manual: LaTeX source in doc/manual, PDF in the repository root (installed and
# opened by Help -> Manual)
manual:
	cd doc/manual && latexmk -pdf -interaction=nonstopmode spectangular_manual.tex
	cp doc/manual/spectangular_manual.pdf Spectangular_Manual.pdf

clean:
	-[ -f $(BUILDDIR)/Makefile ] && $(MAKE) -C $(BUILDDIR) clean

distclean:
	rm -rf $(BUILDDIR)
