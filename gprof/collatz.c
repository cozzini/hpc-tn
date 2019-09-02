/*
 * authors   : Giuseppe Piero Brandino - eXact-lab s.r.l.
 * date      : October 2018
 * copyright : GNU Public License
 */

#include <stdio.h>

/* Computes the length of Collatz sequences 
 * AUTHOR; unknown
 * LAST REVISION: 11/22/17 Giuseppe Piero Brandino
 * */

unsigned int
step (unsigned int x)
{
  if (x % 2 == 0)
    {
      return (x / 2);
    }
  else
    {
      return (3 * x + 1);
    }
}

unsigned int
nseq (unsigned int x0)
{
  unsigned int i = 1, x;
  
  if (x0 == 1 || x0 == 0)
    return i;

  x = step (x0);

  while (x != 1 && x != 0)
    {
      x = step (x);
      i++;
    }

  return i;
}

int
main (void)
{
  unsigned int i, m = 0, im = 0;

  for (i = 1; i < 5000000; i++)
    {
      unsigned int k = nseq (i);

      if (k > m)
        {
          m = k;
          im = i;
          printf ("sequence length = %u for %u\n", m, im);
        }
    }

  return 0;
}
