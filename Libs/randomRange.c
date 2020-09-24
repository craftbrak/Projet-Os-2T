#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "randomRange.h"

double randomRange(int min, int max) {
  struct timespec spec;
  double normalized;
  double randomMax = RAND_MAX;
  int randomInt;

  clock_gettime(CLOCK_REALTIME, &spec);
  srandom(spec.tv_nsec);

  randomInt = random();
  normalized = randomInt/randomMax;

  return normalized * (max - min) + min;
}
