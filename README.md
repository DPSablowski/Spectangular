# spectangular
Spectangular - spectral disentangling

# installation

You can use Qt Creator to built the software. Development is based on Qt Creator 4.12.2 and Qt 5.15.0 GCC 64bit from version 1.01 on.

See also the manual pdf

download the spectangular.tar.gz and extract

in the extracted folder, you can run the command

objdump -p Spectangular | grep NEEDED

to find the necessary libraries; you may have to install some of them:

libarmadillo.so.8

libcfitsio.so.5

libCCfits.so.0

libstdc++.so.6

libQt5PrintSupport.so.5

libQt5Widgets.so.5

libQt5Gui.so.5

libQt5Core.so.5

libm.so.6

libgcc_s.so.1

libc.so.6


In debian Jessie, enable jessie-backports and run

apt install libarmadillo8 libcfitsio5 libccfits0 libstdc++6 libqt5printsupport5 libqt5widgets5 libqt5gui5 libqt5core5a libgcc1 libc6 libopenblas-base libpcre++0

to install the dependencies.



to run the program: ./Spectangular
