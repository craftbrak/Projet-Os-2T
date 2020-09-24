#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "randomRange.h"

long double randomRange(int min, int max) {
  struct timespec spec;
  long double normalized;
  long double randomMax = 2147483648.0l;
  long int randomInt;

  clock_gettime(CLOCK_REALTIME, &spec);
  srandom(spec.tv_nsec);

  randomInt = random();
  normalized = randomInt/randomMax;

  return normalized * (max - min) + min;
}
