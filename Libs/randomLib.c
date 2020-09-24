#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/random.h>
#include "randomLib.h"

double randomRange(int min, int max) {
	struct timespec spec;
	double normalized;

	normalized = drand48();

	return normalized * (max - min) + min;
}

void setSeed(unsigned int seed) {
	if (seed == 0) {
		seed = generateSeed();
		if (seed == -1) {
			printf("Impossible d'obtenir un seed aléatoire: %d\n", errno);
			exit(-1);
		}
	}
	srand48(seed);
}

unsigned int generateSeed() {
	unsigned int seed;

	// Random from /dev/random (or /dev/urandom)
	int size = getrandom(&seed, sizeof(seed), 0);
	if (size == -1 || size != sizeof(seed))
		return -1;

	return seed;
}
