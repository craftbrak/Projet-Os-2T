#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
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

int sharedMemInit(SharedInfo* shared, key_t key, int amount) {
    size_t size = amount * sizeof(Voiture);

    int shmid = shmget(key, size, 0644|IPC_CREAT);
    if (shmid == -1) {
        perror("Shared memory");
        return 0;
    }
    shared->shmid = shmid;
    shared->size = amount;
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
     int ctl = shmctl(shared.shmid, IPC_RMID, NULL);
    if (ctl == -1) {
        perror("Shared memory destroy");
        return 0;
    }
    return 1;
}