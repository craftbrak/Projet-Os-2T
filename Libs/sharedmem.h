#pragma once

typedef struct sharedInfo {
    int shmid;
    int size;
} SharedInfo;

Voiture* getAllVoitures(SharedInfo);

Voiture* getVoiture(SharedInfo, int);

int dtVoiture(Voiture*, int);

int dtAllVoitures(Voiture*, SharedInfo);

int sharedMemInit(SharedInfo*, key_t, int);