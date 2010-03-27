#include <stdio.h>
#include <stdlib.h>

template <typename T>
int process(int nreps, int npattern, const T *ppattern, FILE *fout)
{
  int i, j;
  for (i=0; i<nreps; i++)
  {
    for (j=0; i<npattern; j++)
    {
      fwrite(ppattern + j, sizeof(T), 1, fout);
    }
  }
  return 0;
}
