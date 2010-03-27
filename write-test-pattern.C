#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>

#include "write-test-pattern.H"

int main(int argc, char *argv[])
{
  int errcode = EXIT_SUCCESS;
  /* read the flag options */
  char typechar = 'd';
  char widthchar = '4';
  int width = 4;
  int nreps = 1;
  int verbose_flag = 0;
  static struct option long_options[] =
  {
    {"verbose", no_argument, &verbose_flag, 1},
    {"format", required_argument, 0, 'f'},
    {"nreps", required_argument, 0, 'n'},
    {0, 0, 0, 0}
  };
  while (1)
  {
    int option_index = 0;
    int c = getopt_long(argc, argv, "vf:n:", long_options, &option_index);
    if (c == -1)
      break;
    switch (c)
    {
      case 0:
        /* process flags and do things on options */
        break;
      case 'f':
        if (strlen(optarg) != 2)
        {
          fprintf(stderr, "the type string should be two characters\n");
          return EXIT_FAILURE;
        }
        typechar = optarg[0];
        widthchar = optarg[1];
        if (typechar != 'd' && typechar != 'f')
        {
          fprintf(stderr, "the first format character should be 'd' or 'f'\n");
          return EXIT_FAILURE;
        }
        switch (widthchar)
        {
          case '4':
            width = 4;
            break;
          case '8':
            width = 8;
            break;
          default:
            fprintf(stderr, "the second format char should be '4' or '8'\n");
            return EXIT_FAILURE;
        }
        break;
      case 'n':
        nreps = atoi(optarg);
        if (nreps < 1)
        {
          fprintf(stderr, "expected a positive number of repeats\n");
          return EXIT_FAILURE;
        }
        break;
      case '?':
        return EXIT_FAILURE;
      default:
        return EXIT_FAILURE;
    }
  }
  /* allocate memory for the pattern */
  if (verbose_flag)
    fprintf(stderr, "allocate pattern...\n");
  int npattern = argc - optind;
  void *pattern = malloc(npattern * width);
  int i;
  /* write the pattern */
  if (verbose_flag)
  {
    fprintf(stderr, "write the pattern using ");
    fprintf(stderr, "nreps=%d npattern=%d...\n", nreps, npattern);
  }
  if (typechar == 'd' && width == 4)
  {
    for (i=0; i<npattern; i++)
      ((int32_t *) pattern)[i] = (int32_t) atol(argv[optind + i]);
    if (process<int32_t>(nreps, npattern, (int32_t *) pattern, stdout) < 0)
      errcode = EXIT_FAILURE;
  } else if (typechar == 'd' && width == 8) {
    for (i=0; i<npattern; i++)
      ((int64_t *) pattern)[i] = (int64_t) atol(argv[optind + i]);
    if (process<int64_t>(nreps, npattern, (int64_t *) pattern, stdout) < 0)
      errcode = EXIT_FAILURE;
  } else if (typechar == 'f' && width == 4) {
    for (i=0; i<npattern; i++)
      ((float *) pattern)[i] = (float) atof(argv[optind + i]);
    if (process<float>(nreps, npattern, (float *) pattern, stdout) < 0)
      errcode = EXIT_FAILURE;
  } else if (typechar == 'f' && width == 8) {
    for (i=0; i<npattern; i++)
      ((double *) pattern)[i] = (double) atof(argv[optind + i]);
    if (process<double>(nreps, npattern, (double *) pattern, stdout) < 0)
      errcode = EXIT_FAILURE;
  }
  /* clean up */
  if (verbose_flag)
    fprintf(stderr, "cleaup...\n");
  free(pattern);
  return errcode;
}

