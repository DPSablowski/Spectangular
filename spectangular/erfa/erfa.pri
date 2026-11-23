# ERFA subset (barycentric correction), C sources
ERFA_SOURCES = anp anpm apco apco13 apcs bpn2xy c2ixys cal2jd cp cr dat dtdb eform eors epv00 era00 \
    erfadatextra fad03 fae03 faf03 faju03 fal03 falp03 fama03 fame03 faom03 fapa03 fasa03 faur03 fave03 \
    fw2m gd2gc gd2gce ir jd2cal nut00a nut06a obl06 pfw06 pm pn pnm06a pom00 pvtob refco rx rxp rxpv ry rz \
    s06 sp00 sxp taitt taiut1 tr trxp trxpv utctai utcut1 zp
for(f, ERFA_SOURCES): SOURCES += $$PWD/$${f}.c
HEADERS += $$PWD/erfa.h $$PWD/erfam.h $$PWD/erfaextra.h $$PWD/erfadatextra.h
QMAKE_CFLAGS += -O2
