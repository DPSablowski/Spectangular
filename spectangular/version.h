#ifndef SPECTANGULAR_VERSION_H
#define SPECTANGULAR_VERSION_H

// Identification of the build, shown at the start (console and Messages).
// In a source archive made with "git archive", git fills in the commit (export-subst, see
// .gitattributes); in a git checkout qmake passes it as SPECTANGULAR_GIT_COMMIT.
#define SPECTANGULAR_ARCHIVE_COMMIT "89dabab (2026-11-23)"

inline const char *spectangularCommit()
{
    static const char *archive = SPECTANGULAR_ARCHIVE_COMMIT;
    if (archive[0] != '$') return archive;
#ifdef SPECTANGULAR_GIT_COMMIT
    return SPECTANGULAR_GIT_COMMIT;
#else
    return "unknown";
#endif
}

#endif // SPECTANGULAR_VERSION_H
