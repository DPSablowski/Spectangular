#include "resources.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QStringList>

namespace {

QStringList searchDirs()
{
    QStringList dirs;
    dirs << QDir::currentPath();
    dirs << QDir::homePath() + "/.local/share/spectangular";
    const QString app = QCoreApplication::applicationDirPath();
    if (!app.isEmpty()) {
        dirs << app;
        dirs << QDir(app + "/../share/spectangular").absolutePath();
        dirs << QDir(app + "/../share/doc/spectangular").absolutePath();
    }
#ifdef SPECTANGULAR_DATADIR
    dirs << QString(SPECTANGULAR_DATADIR);
#endif
#ifdef SPECTANGULAR_DOCDIR
    dirs << QString(SPECTANGULAR_DOCDIR);
#endif
    dirs.removeDuplicates();
    return dirs;
}

} // namespace

QString findResource(const QString &name)
{
    for (const QString &d : searchDirs()) {
        QFileInfo f(QDir(d).filePath(name));
        if (f.isFile())
            return f.absoluteFilePath();
    }
    return QString();
}

QString resourceSearchPath()
{
    return searchDirs().join(", ");
}
