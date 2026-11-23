# OpenMP for LSQR, only if the compiler supports it including header and runtime
# (GCC: libgomp; Clang additionally needs libomp-dev). Otherwise single-core, with a note.
#
# Link with the compiler that compiles: if only the compiler is switched (e.g. a Qt Creator
# kit with Clang on the linux-g++ mkspec), qmake keeps g++ as linker. g++ -fopenmp links
# GCC's libgomp, but Clang's OpenMP code calls LLVM's libomp (__kmpc_*), so the link fails
# with "undefined reference to __kmpc_global_thread_num" and similar.
!isEmpty(QMAKE_CXX):!equals(QMAKE_LINK, $$QMAKE_CXX) {
    QMAKE_LINK = $$QMAKE_CXX
    QMAKE_LINK_SHLIB = $$QMAKE_CXX
}
OPENMP_TEST = $$PWD/config_tests/openmp.cpp
system($$QMAKE_CXX -fopenmp $$OPENMP_TEST -o $$OUT_PWD/.openmp_test > /dev/null 2>&1) {
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_LFLAGS += -fopenmp
    system(rm -f $$OUT_PWD/.openmp_test)
} else {
    message("OpenMP not available with $$QMAKE_CXX: LSQR uses one core (Clang: install libomp-dev).")
}
