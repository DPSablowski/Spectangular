// Configuration test (qmake): does the compiler support OpenMP with header and runtime?
#include <omp.h>
int main()
{
    int n = 0;
#pragma omp parallel reduction(+:n)
    n += 1;
    return n > 0 && omp_get_max_threads() > 0 ? 0 : 1;
}
