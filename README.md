# spectangular
Spectangular - spectral disentangling

# installation

You can use Qt Creator to built the software. Development is based on Qt Creator 4.12.2 and Qt 5.15.0 GCC 64bit from version 1.01 on.

See also the manual pdf

download the spectangular.tar.gz and extract

in the extracted folder, you can run the command

objdump -p Spectangular | grep NEEDED

to find the necessary libraries; you may have to install some of them:
libarmadillo
libcfitsio
libCCfits

In Debian Bullseye you can install the necessary libraries and compile it like this

```bash
apt install libarmadillo10 libarmadillo-dev libccfits0v5 libccfits-dev qtbase5-dev qtmultimedia5-dev

git clone https://github.com/DPSablowski/Spectangular.git
cd Spectangular/spectangular
qmake Spectangular.pro
make
```

to run the program: ```./Spectangular``` or copy it into your bin path.
