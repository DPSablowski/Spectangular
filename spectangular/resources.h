#ifndef RESOURCES_H
#define RESOURCES_H

#include <QString>

// Searches for a bundled file (BinaryData.dat, manual) in this order:
//   1. current directory (own, customized copy)
//   2. ~/.local/share/spectangular
//   3. directory of the program (build directory, development with Qt Creator)
//   4. installation directory SPECTANGULAR_DATADIR (make install, e.g. /usr/local/share/spectangular)
//      or ../share/spectangular relative to the program
// Returns: absolute path, or empty if not found.
QString findResource(const QString &name);

// All searched directories, for error messages.
QString resourceSearchPath();

#endif // RESOURCES_H
