#include <stdio.h>
#include <stdlib.h>

int process(FILE *fin, FILE *fout, size_t offset, size_t stride, size_t width)
{
  int errcode = 0;
  char *buffer = malloc(width);
  size_t fread_result;
  int gap = stride - width;
  /* seek to the offset */
  if (fseek(fin, offset, SEEK_SET) < 0)
  {
    goto end;
  }
  /* filter the stream */
  while (1)
  {
    /* read from the channel */
    fread_result = fread(buffer, 1, width, fin);
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
     /* if the demux is nondegenerate then seek according to the stride */
    if (gap)
    {
      if (fseek(fin, gap, SEEK_CUR) < 0)
      {
        goto end;
      }
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
