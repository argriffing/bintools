#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>

int main(int argc, char *argv[])
{
  int errcode = EXIT_SUCCESS;
  /* read the flag options */
  char typechar = 'd';
  char widthchar = '4';
  int width = 4;
  int nreps = 1;
  int verbose_flag = 0;
  struct option long_options[] =
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
    fprintf(stderr, "allocate the pattern...\n");
  int npattern = argc - optind;
  int pattern_width = npattern * width;
  void *pattern = malloc(pattern_width);
  int i;
  /* create the pattern */
  if (verbose_flag)
    fprintf(stderr, "create the the pattern...\n");
  for (i=0; i<npattern; i++)
  {
    const char *s = argv[optind + i];
    if (typechar == 'd' && width == 4) {
      ((int32_t *) pattern)[i] = (int32_t) atol(s);
    } else if (typechar == 'd' && width == 8) {
      ((int64_t *) pattern)[i] = (int64_t) atol(s);
    } else if (typechar == 'f' && width == 4) {
      ((float *) pattern)[i] = (float) atof(s);
    } else if (typechar == 'f' && width == 8) {
      ((double *) pattern)[i] = (double) atof(s);
    }
  }
  /* write the pattern */
  if (verbose_flag)
    fprintf(stderr, "write the pattern %d times...\n", nreps);
  for (i=0; i<nreps; i++)
    fwrite(pattern, pattern_width, 1, stdout);
  /* clean up */
  if (verbose_flag)
    fprintf(stderr, "cleaup...\n");
  free(pattern);
  return errcode;
}

