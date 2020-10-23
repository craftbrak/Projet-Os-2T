#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include "Settings.h"
#include "voiture.h"
#include "sharedmem.h"


Voiture* getAllVoitures(SharedInfo shared) {
    Voiture* voiture = (Voiture*)shmat(shared.shmid, NULL, 0);
    if (voiture == (void *) -1) {
        perror("Shared memory attach");
        return NULL;
    }
    return voiture;
}

Voiture* getVoiture(SharedInfo shared, int index) {
    return (getAllVoitures(shared) + index);
}

int sharedMemInit(SharedInfo* shared, Settings settings) {
    key_t shm_key = (int) *((double *) SettingsGet(settings, "shm_key"));
    key_t sem_key = (int) *((double *) SettingsGet(settings, "sem_key"));
    int qteVoitures = (int) ((NbrVector *)SettingsGet(settings, "noms_voitures"))->length;
    size_t size = qteVoitures * sizeof(Voiture);

    int shmid = shmget(shm_key, size, 0644|IPC_CREAT);
    if (shmid == -1) {
        perror("Shared memory");
        return 0;
    }
    int semid = semget(shm_key, qteVoitures, 0644|IPC_CREAT);
    if (semid == -1) {
        perror("Semaphore");
        return 0;
    }
    shared->shmid = shmid;
    shared->shm_key = shm_key;
    shared->size = qteVoitures;
    shared->semid = semid;
    shared->sem_key = sem_key;
    return 1;
}

int dtVoiture(Voiture* ptr, int index) {
    if (shmdt(ptr + index) == -1) {
        perror("Shared memory detach");
        return 0;
    }
    return 1;
}

int dtAllVoitures(Voiture* ptr, SharedInfo shared) {
    if(!dtVoiture(ptr, 0)) {
        return 0;
    }
    if ((shmctl(shared.shmid, IPC_RMID, 0)) == -1) {
        perror("Shared memory destroy");
        return 0;
    }
    if ((semctl(shared.semid, 0, IPC_RMID)) == -1) {
        perror("Semaphore destroy");
        return 0;
    }
    return 1;
}

void getAllVoituresCopy(Voiture buffer[], SharedInfo shared) {
    for (int i=0;i<shared.size;i++) {
        getVoitureCopy(i, buffer + i, shared);
    }
}

void getVoitureCopy(int index, Voiture* buffer, SharedInfo shared) {
    Voiture* cible = getVoiture(shared, index);
    //SEMAPHORE ON
    *buffer = *cible;
    //SEMAPHORE OFF
    dtVoiture(cible, index);
}

void setVoiture(int index, Voiture buffer, SharedInfo shared) {
    Voiture* cible = getVoiture(shared, index);
    //SEMAPHORE ON
    *cible = buffer;
    //SEMAPHORE OFF
    dtVoiture(cible, index);
}