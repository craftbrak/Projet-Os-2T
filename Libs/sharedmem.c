#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include "voiture.h"
#include "sharedmem.h"


Voiture* getAllVoitures(SharedInfo shared) {
    Voiture* voiture = (Voiture*)shmat(shared.id, NULL, 0);
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
        printf("Shared memory");
        return 0;
    }
    shared->id = shmid;
    shared->size = amount;
    return 1;
}

int dtVoiture(Voiture* ptr, int index) {
    if (shmdt(ptr) == -1) {
        printf("Shared memory detach");
        return 0;
    }
    return 1;
}

int dtAllVoitures(Voiture* ptr) {
    return dtVoiture(ptr, 0);
}