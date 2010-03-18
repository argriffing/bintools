#include <stdio.h>
#include <stdlib.h>

struct kahan_t
{
  double sum;
  double c;
};

void kahan_reset(struct kahan_t *p)
{
  p->sum = 0;
  p->c = 0;
}

void kahan_add(struct kahan_t *p, double value)
{
  double y = value - p->c;
  double t = p->sum + y;
  p->c = (t - p->sum) - y;
  p->sum = t;
}

int process(FILE *fin, FILE *fout, int ndoubles_per_box)
{
  double v;
  double avg;
  int ndoubles_curr = 0;
  struct kahan_t kahan;
  kahan_reset(&kahan);
  while (fread(&v, sizeof(double), 1, fin))
  {
    /* accumulate using kahan summation */
    kahan_add(&kahan, v);
    /* increment the count of doubles in the box */
    ++ndoubles_curr;
    /* if the box is full then emit the average */
    if (ndoubles_curr == ndoubles_per_box)
    {
      /* emit the average */
      avg = kahan.sum / ndoubles_curr;
      fwrite(&avg, sizeof(double), 1, fout);
      /* reset the kahan summation state */
      kahan_reset(&kahan);
      /* reset the number of doubles in the current box */
      ndoubles_curr = 0;
    }
  }
  /* emit the average of the final partial box */
  if (ndoubles_curr)
  {
    avg = kahan.sum / ndoubles_curr;
    fwrite(&avg, sizeof(double), 1, fout);
  }
  return 0;
}

int main(int argc, const char **argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <ndoubles-per-box>\n", argv[0]);
    return EXIT_FAILURE;
  }
  int ndoubles_per_box = atoi(argv[1]);
  if (ndoubles_per_box < 1)
  {
    fprintf(stderr, "expected at least one double per box\n");
    return EXIT_FAILURE;
  }
  if (process(stdin, stdout, ndoubles_per_box) < 0)
  {
    return EXIT_FAILURE;
  } else {
    return EXIT_SUCCESS;
  }
}
