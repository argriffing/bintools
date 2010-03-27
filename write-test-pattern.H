#ifndef WRITE_TEST_PATTERN_H
#define WRITE_TEST_PATTERN_H

#include <stdio.h>
#include <stdlib.h>

template <typename T>
int process(int nreps, int npattern, const T *pattern, FILE *fout)
{
  int i, j;
  for (i=0; i<nreps; i++)
  {
    for (j=0; j<npattern; j++)
    {
      fwrite(pattern + j, sizeof(T), 1, fout);
    }
  }
  return 0;
}

#endif
