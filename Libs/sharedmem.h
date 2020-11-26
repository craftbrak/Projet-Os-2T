#pragma once

typedef struct sharedInfo {
    int shm_key;
    int shmid;
    int sem_key;
    int semid;
    int size;
} SharedInfo;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

Voiture *getAllVoitures(SharedInfo);

Voiture *getVoiture(SharedInfo, int);

int dtVoiture(SharedInfo, Voiture *, int);

int dtAllVoitures(SharedInfo);

int sharedMemInit(SharedInfo *, Settings);

int getAllVoituresCopy(Voiture[], SharedInfo);

int getVoitureCopy(int, Voiture *, SharedInfo);

int setVoiture(int, Voiture *, SharedInfo);

int getSemaphore(int, SharedInfo);

int freeSemaphore(int, SharedInfo);