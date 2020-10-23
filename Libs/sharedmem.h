#pragma once

typedef struct sharedInfo {
    int shm_key;
    int shmid;
    int sem_key;
    int semid;
    int size;
} SharedInfo;

Voiture* getAllVoitures(SharedInfo);

Voiture* getVoiture(SharedInfo, int);

int dtVoiture(Voiture*, int);

int dtAllVoitures(Voiture*, SharedInfo);

int sharedMemInit(SharedInfo*, Settings);

void getAllVoituresCopy(Voiture[], SharedInfo);

void getVoitureCopy(int, Voiture*, SharedInfo);

void setVoiture(int, Voiture, SharedInfo);