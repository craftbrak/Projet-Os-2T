#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include "Settings.h"
#include "voiture.h"
#include "sharedmem.h"
#include "logger.h"


Voiture *getAllVoitures(SharedInfo shared) {
    Voiture *voiture = (Voiture *) shmat(shared.shmid, NULL, 0);
    if (voiture == (void *) -1) {
        logError(shared, "Shared memory attach");
        return NULL;
    }
    return voiture;
}

Voiture *getVoiture(SharedInfo shared, int index) {
    return (getAllVoitures(shared) + index);
}

int sharedMemInit(SharedInfo *shared, Settings settings) {
    key_t shm_key = (int) *((double *) SettingsGet(settings, "shm_key"));
    key_t sem_key = (int) *((double *) SettingsGet(settings, "sem_key"));
    int qteVoitures = (int) ((NbrVector *) SettingsGet(settings, "noms_voitures"))->length;
    size_t size = qteVoitures * sizeof(Voiture);
    union semun u;
    u.val = 1;

    int shmid = shmget(shm_key, size, 0644 | IPC_CREAT);
    if (shmid == -1) {
        perror("Shared memory");
        return 0;
    }
    int semid = semget(shm_key, qteVoitures + 1, 0644 | IPC_CREAT);
    if (semid == -1) {
        perror("Semaphore");
        return 0;
    }
    for (int i = 0; i <= qteVoitures; i++) {
        if ((semctl(semid, i, SETVAL, u)) == -1) {
            perror("Semaphore set value");
            return 0;
        }
    }
    shared->shmid = shmid;
    shared->shm_key = shm_key;
    shared->size = qteVoitures;
    shared->semid = semid;
    shared->sem_key = sem_key;
    return 1;
}

int dtVoiture(SharedInfo shared, Voiture *ptr, int index) {
    if (shmdt(ptr - index) == -1) {
        logError(shared, "Shared memory detach");
        return 0;
    }
    return 1;
}

int dtAllVoitures(SharedInfo shared) {
    if ((shmctl(shared.shmid, IPC_RMID, 0)) == -1) {
        logError(shared, "Shared memory destroy");
        return 0;
    }
    if ((semctl(shared.semid, 0, IPC_RMID)) == -1) {
        logError(shared, "Semaphore destroy");
        return 0;
    }
    return 1;
}

int getAllVoituresCopy(Voiture buffer[], SharedInfo shared) {
    for (int i = 0; i < shared.size; i++) {
        if (!getVoitureCopy(i, buffer + i, shared)) {
            return 0;
        }
    }
    return 1;
}

int getVoitureCopy(int index, Voiture *buffer, SharedInfo shared) {
    Voiture *cible = getVoiture(shared, index);
    if (!cible) {
        return 0;
    }
    if (!getSemaphore(index + 1, shared)) {
        return 0;
    }
    *buffer = *cible;
    if (!freeSemaphore(index + 1, shared)) {
        return 0;
    }
    //printf("getcopy %d\n", index);
    dtVoiture(shared, cible, index);
    return 1;
}

int setVoiture(int index, Voiture *buffer, SharedInfo shared) {
    Voiture *cible = getVoiture(shared, index);
    if (!cible) {
        return 0;
    }
    if (!getSemaphore(index + 1, shared)) {
        return 0;
    }
    *cible = *buffer;
    if (!freeSemaphore(index + 1, shared)) {
        return 0;
    }
    //printf("setcopy %d\n", index);
    dtVoiture(shared, cible, index);
    return 1;
}

int getSemaphore(int index, SharedInfo shared) {
    struct sembuf buf = {index, -1, SEM_UNDO};
    if (semop(shared.semid, &buf, 1) < 0) {
        logError(shared, "Semaphore set -1");
        return 0;
    }
    return 1;
}

int freeSemaphore(int index, SharedInfo shared) {
    struct sembuf buf = {index, +1, SEM_UNDO};
    if (semop(shared.semid, &buf, 1) < 0) {
        logError(shared, "Semaphore set +1");
        return 0;
    }
    return 1;
}