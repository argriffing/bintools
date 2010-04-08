#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include <getopt.h>
#include <string>

#include "sum-axis-0.H"

// Decimal / Float and the number of bytes per element
enum EOdFormat {D1, D2, D4, D8, F4, F8};

/* Fill the typechar and widthchar and eodformat.
 * Return -1 on error.
 */
int get_format(const char *optarg,
    char *typechar, char *widthchar, EOdFormat *odformat)
{
  if (strlen(optarg) != 2)
  {
    fprintf(stderr, "each type string should be two characters long\n");
    return -1;
  }
  *typechar = optarg[0];
  *widthchar = optarg[1];
  if (*typechar == 'd')
  {
    switch (*widthchar)
    {
      case '1':
        *odformat = D1;
        break;
      case '2':
        *odformat = D2;
        break;
      case '4':
        *odformat = D4;
        break;
      case '8':
        *odformat = D8;
        break;
      default:
        fprintf(stderr, "the width of a decimal format must be ");
        fprintf(stderr, "'1', '2', '4', or '8'\n");
        return -1;
    }
  } else if (*typechar == 'f') {
    switch (*widthchar)
    {
      case '4':
        *odformat = F4;
        break;
      case '8':
        *odformat = F8;
        break;
      default:
        fprintf(stderr, "the width of a floating point format must be ");
        fprintf(stderr, "'4', or '8'\n");
        return -1;
    }
  } else {
    fprintf(stderr, "the first format character should be 'd' or 'f'\n");
    return -1;
  }
  return 0;
}

int main(int argc, char *argv[])
{
  static struct option long_options[] =
  {
    {"format-in", required_argument, 0, 'i'},
    {"format-out", required_argument, 0, 'o'},
    {"ncols", required_argument, 0, 'n'},
    {0, 0, 0, 0}
  };
  char typechar_in;
  char widthchar_in;
  EOdFormat fmt_in;
  char typechar_out;
  char widthchar_out;
  EOdFormat fmt_out;
  int ncols;
  while (1)
  {
    int option_index = 0;
    int c = getopt_long(argc, argv, "i:o:n:", long_options, &option_index);
    if (c == -1)
      break;
    switch (c)
    {
      case '?':
        return EXIT_FAILURE;
      case 'i':
        if (get_format(optarg,
              &typechar_in, &widthchar_in, &fmt_in) < 0)
          return EXIT_FAILURE;
        break;
      case 'o':
        if (get_format(optarg,
              &typechar_out, &widthchar_out, &fmt_out) < 0)
          return EXIT_FAILURE;
        break;
      case 'n':
        ncols = atoi(optarg);
        if (ncols < 1)
        {
          fprintf(stderr, "expected at least one column\n");
          return EXIT_FAILURE;
        }
        break;
      default:
        fprintf(stderr, "unknown parameter: %s\n", optarg);
        return EXIT_FAILURE;
    }
  }

  // Process according to the types.
  FILE *fin = stdin;
  FILE *fout = stdout;

  if (typechar_out == 'd')
  {
    vector<int64_t> accum;
    // Accumulate high precision decimals.
    if (fmt_in == D1)
      accum = process<int8_t, int64_t>(ncols, fin);
    else if (fmt_in == D2)
      accum = process<int16_t, int64_t>(ncols, fin);
    else if (fmt_in == D4)
      accum = process<int32_t, int64_t>(ncols, fin);
    else if (fmt_in == D8)
      accum = process<int64_t, int64_t>(ncols, fin);
    else if (fmt_in == F4)
      accum = process<float, int64_t>(ncols, fin);
    else if (fmt_in == F8)
      accum = process<double, int64_t>(ncols, fin);
    if (accum.empty())
      return EXIT_FAILURE;
    // Write the low precision output.
    if (fmt_out == D1)
      write_low_precision<int64_t, int8_t>(accum, fout);
    else if (fmt_out == D2)
      write_low_precision<int64_t, int16_t>(accum, fout);
    else if (fmt_out == D4)
      write_low_precision<int64_t, int32_t>(accum, fout);
    else if (fmt_out == D8)
      write_low_precision<int64_t, int64_t>(accum, fout);
    return EXIT_SUCCESS;
  }

  if (typechar_out == 'f')
  {
    vector<double> accum;
    // Accumulate high precision floating point numbers.
    if (fmt_in == D1)
      accum = process<int8_t, double>(ncols, fin);
    else if (fmt_in == D2)
      accum = process<int16_t, double>(ncols, fin);
    else if (fmt_in == D4)
      accum = process<int32_t, double>(ncols, fin);
    else if (fmt_in == D8)
      accum = process<int64_t, double>(ncols, fin);
    else if (fmt_in == F4)
      accum = process<float, double>(ncols, fin);
    else if (fmt_in == F8)
      accum = process<double, double>(ncols, fin);
    if (accum.empty())
      return EXIT_FAILURE;
    // Write the low precision output.
    if (fmt_out == F4)
      write_low_precision<double, float>(accum, fout);
    else if (fmt_out == F8)
      write_low_precision<double, double>(accum, fout);
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
