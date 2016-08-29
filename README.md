# spectangular
Spectangular - spectral disentangling

# installation

download the croco.tar.gz and extract

in the extracted folder, you can run the connand
objdump -p CroCo | grep NEEDED

to find the necessary libraries you may to install some of them:

  NEEDED               libcfitsio.so.2
  NEEDED               libCCfits.so.0
  NEEDED               libstdc++.so.6
  NEEDED               libQt5PrintSupport.so.5
  NEEDED               libQt5Widgets.so.5
  NEEDED               libQt5Gui.so.5
  NEEDED               libQt5Core.so.5
  NEEDED               libGL.so.1
  NEEDED               libpthread.so.0
  NEEDED               libm.so.6
  NEEDED               libgcc_s.so.1
  NEEDED               libc.so.6

to run the program: ./CroCo
