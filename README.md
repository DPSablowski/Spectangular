# spectangular
Spectangular - spectral disentangling

# installation

download the spectangular.tar.gz and extract

in the extracted folder, you can run the command

objdump -p Spectangular | grep NEEDED

to find the necessary libraries; you may have to install some of them:

libarmadillo.so.6

liblapack.so.3

libcfitsio.so.2

libCCfits.so.0

libstdc++.so.6

libQt5Multimedia.so.5

libQt5PrintSupport.so.5

libQt5Widgets.so.5

libQt5Gui.so.5

libQt5Network.so.5

libQt5Core.so.5

libGL.so.1

libpthread.so.0

libm.so.6

libgcc_s.so.1

libc.so.6

----
In debian Jessie, enable jessie-backports and run

apt install libarmadillo6 liblapack3 libcfitsio2 libccfits0 libstdc++6 libqt5multimedia5 libqt5printsupport5 libqt5widgets5 libqt5gui5 libqt5network5 libqt5core5a libgcc1 libc6 libopenblas-base libpcre++0

to install the dependencies.
----


to run the program: ./Spectangular
