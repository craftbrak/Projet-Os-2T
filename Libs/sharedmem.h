#pragma once

typedef struct sharedInfo {
    int id;
    int size;
} SharedInfo;

Voiture* getAllVoitures(SharedInfo);

Voiture* getVoiture(SharedInfo, int);

int dtVoiture(Voiture*, int);

int dtAllVoitures(Voiture*);

int sharedMemInit(SharedInfo*, key_t, int);