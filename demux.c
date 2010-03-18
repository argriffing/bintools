#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
 * This is like fseek(fin, n, SEEK_CUR) for a pipe (which is not fseekable).
 * Return the number of successfully read (and discarded) bytes.
 */
int advance(FILE *fin, int n)
{
  int i;
  for (i=0; i<n && (fgetc(fin) != EOF); i++);
  return i;
}

int process(FILE *fin, FILE *fout, int offset, int stride, int width)
{
  int errcode = 0;
  char *buffer = malloc(width);
  int fread_result;
  int gap = stride - width;
  /* seek past the first few bytes */
  if (advance(fin, offset) < offset)
  {
    goto end;
  }
  /* filter the stream */
  while (1)
  {
    /* read from the channel */
    fread_result = (int) fread(buffer, 1, width, fin);
    if (!fread_result)
    {
      goto end;
    }
    if (fread_result < width)
    {
      fprintf(stderr, "incomplete read\n");
      errcode = -1; goto end;
    }
    /* write to the output stream */
    fwrite(buffer, 1, width, fout);
    /* seek according to the stride */
    if (advance(fin, gap) < gap)
    {
      goto end;
    }
  }
end:
  free(buffer);
  return errcode;
}

int main(int argc, const char **argv)
{
  if (argc != 4)
  {
    fprintf(stderr, "Usage: %s <offset> <stride> <width>\n", argv[0]);
    return EXIT_FAILURE;
  }
  int offset = atoi(argv[1]);
  int stride = atoi(argv[2]);
  int width = atoi(argv[3]);
  if (offset < 0)
  {
    fprintf(stderr, "expected the offset to be nonnegative\n");
    return EXIT_FAILURE;
  }
  if (stride < 1)
  {
    fprintf(stderr, "expected the stride to be positive\n");
    return EXIT_FAILURE;
  }
  if (width < 1)
  {
    fprintf(stderr, "expected the width to be positive\n");
    return EXIT_FAILURE;
  }
  if (width > stride)
  {
    fprintf(stderr, "the width is greater than the stride\n");
    return EXIT_FAILURE;
  }
  if (process(stdin, stdout, offset, stride, width) < 0)
  {
    return EXIT_FAILURE;
  } else {
    return EXIT_SUCCESS;
  }
}
