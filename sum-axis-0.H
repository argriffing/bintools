#ifndef SUM_AXIS_0
#define SUM_AXIS_0

#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;


template <typename T_high, typename T_low>
int write_low_precision(const vector<T_high> &v, FILE *fout)
{
  int n = v.size();
  int i;
  for (i=0; i<n; i++)
  {
    T_low x = (T_low) v[i];
    fwrite(&x, sizeof(T_low), 1, fout);
  }
  return 0;
}

template <typename T_in, typename T_out>
vector<T_out> process(int ncols, FILE *fin)
{
  vector<T_in> read_vector(ncols, 0);
  vector<T_out> accum_vector(ncols, 0);
  // Accumulate the counts.
  while (1)
  {
    // Read a row.
    int nelements = fread(&read_vector[0], sizeof(T_in), ncols, fin);
    if (nelements == 0)
      break;
    if (nelements < ncols)
    {
      fprintf(stderr, "found a ragged row\n");
      accum_vector.clear();
      break;
    }
    // Add the row to the accumulated row.
    int i;
    for (i=0; i<ncols; i++)
    {
      accum_vector[i] += (T_out) read_vector[i];
    }
  }
  return accum_vector;
}

#endif
