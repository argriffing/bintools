#ifndef ELEMENTWISE_SUM
#define ELEMENTWISE_SUM

#include <stdio.h>
#include <stdlib.h>

template <typename T>
int process(int ninputs, FILE **infiles, FILE *fout)
{
  while (1)
  {
    int i;
    T accum = 0;
    for (i=0; i<ninputs; i++)
    {
      T addend;
      int nelements = fread(&addend, sizeof(addend), 1, infiles[i]);
      if (nelements)
      {
        accum += addend;
      } else {
        return 0;
      }
    }
    fwrite(&accum, sizeof(accum), 1, fout);
  }
  return 0;
}

#endif
