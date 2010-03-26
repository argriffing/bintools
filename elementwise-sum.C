#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>

#include "elementwise-sum.H"

int main(int argc, char *argv[])
{
  int errcode = EXIT_SUCCESS;
  static struct option long_options[] =
  {
    {"format", required_argument, 0, 'f'},
    {0, 0, 0, 0}
  };
  int option_index = 0;
  int c = getopt_long(argc, argv, "f:", long_options, &option_index);
  char typechar;
  char widthchar;
  switch (c)
  {
    case '?':
      return EXIT_FAILURE;
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
      if (widthchar != '4' && widthchar != '8')
      {
        fprintf(stderr, "the second format character should be '4' or '8'\n");
        return EXIT_FAILURE;
      }
      break;
    default:
      fprintf(stderr, "unknown format\n");
      return EXIT_FAILURE;
  }
  if (optind >= argc)
  {
    fprintf(stderr, "expected a filename\n");
    return EXIT_FAILURE;
  }
  /* open the output file for writing binary data */
  FILE *fout = fopen(argv[optind], "wb");
  if (!fout)
  {
    fprintf(stderr, "Failed to open %s for writing.\n", argv[optind]);
    return EXIT_FAILURE;
  }
  /* open the input files for reading the binary data */
  int i;
  int ninputs = argc-3;
  FILE **infiles = (FILE **) malloc(ninputs*sizeof(FILE*));
  for (i=0; i<ninputs; i++) infiles[i] = NULL;
  for (i=0; i<ninputs; i++)
  {
    infiles[i] = fopen(argv[optind+1+i], "rb");
  }
  /* do the summation */
  if (typechar == 'd' && widthchar == '4')
  {
    if (process<int32_t>(ninputs, infiles, fout) < 0) errcode = EXIT_FAILURE;
  } else if (typechar == 'd' && widthchar == '8') {
    if (process<int64_t>(ninputs, infiles, fout) < 0) errcode = EXIT_FAILURE;
  } else if (typechar == 'f' && widthchar == '4') {
    if (process<float>(ninputs, infiles, fout) < 0) errcode = EXIT_FAILURE;
  } else if (typechar == 'f' && widthchar == '8') {
    if (process<double>(ninputs, infiles, fout) < 0) errcode = EXIT_FAILURE;
  }
  /* clean up */
  if (fout) fclose(fout);
  for (i=0; i<ninputs; i++)
  {
    if (infiles[i]) fclose(infiles[i]);
  }
  free(infiles);
  return errcode;
}

