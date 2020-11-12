#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Libs/randomLib.h"
#include "Libs/voiture.h"
#include "Libs/SettingsParser.h"
#include "Libs/sharedmem.h"
#include "Libs/course.h"
#include "Libs/sorting.h"
#include "Libs/display.h"

void pauseCourse();

int initVoitures(SharedInfo, char *[], size_t, int);

void validateSettings(Settings);

int lancerEssai(SharedInfo, Settings, int [], unsigned int, int, int, int, int, int, char *);

int lancerFinale(SharedInfo, Settings, int [], unsigned int, int, int, int, int, int);

int main(int argc, char **argv) {
    char *filename = "config.txt";
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0) {
            printf("Usage: %s [config.txt]\n", argv[0]);
            return -3;
        } else if (access(argv[1], F_OK) == -1) {
            fprintf(stderr, "Cannot open file %s. Please provide an existing configuration file.\n", argv[1]);
            return -3;
        } else {
            filename = argv[1];
        }
    }
    Settings settings = parse_config(filename);
    if (settings == NULL) {
        printf("Error: %s\n", geterror());
        return -2;
    }
    validateSettings(settings);

    int counter = 0;
    unsigned int seed = generateSeed();
    int qte_sections = (int) ((NbrVector *) SettingsGet(settings, "longueur_sections"))->length;
    int qte_tours_finale = (int) *((double *) SettingsGet(settings, "qte_tours_finale"));
    StrVector *noms_voitures = SettingsGet(settings, "noms_voitures");

    char **noms = noms_voitures->data;
    int qteVoitures = (int) noms_voitures->length;
    int tri[qteVoitures];
    SharedInfo shared;

    if (!(sharedMemInit(&shared, settings))) {
        SettingsDestroy(settings);
        return 1;
    }

    if (!(initVoitures(shared, noms, qteVoitures, qte_sections))) {
        dtAllVoitures(shared);
        SettingsDestroy(settings);
        return 1;
    }

    generateOrderedArr(tri, qteVoitures);

    lancerEssai(shared, settings, tri, seed, qte_sections, qteVoitures, 0, counter++, 5400, "P1:");

    lancerEssai(shared, settings, tri, seed, qte_sections, qteVoitures, 0, counter++, 5400, "P2:");

    lancerEssai(shared, settings, tri, seed, qte_sections, qteVoitures, 0, counter++, 3600, "P3:");

    lancerEssai(shared, settings, tri, seed, qte_sections, qteVoitures, 0, counter++, 1080, "Q1:");

    lancerEssai(shared, settings, tri, seed, qte_sections, qteVoitures, 5, counter++, 900, "Q2:");

    lancerEssai(shared, settings, tri, seed, qte_sections, qteVoitures, 10, counter++, 720, "Q3:");

    lancerFinale(shared, settings, tri, seed, qte_sections, qteVoitures, 10, counter, qte_tours_finale);

    dtAllVoitures(shared);
    SettingsDestroy(settings);

    return 0;
}

int lancerEssai
        (SharedInfo shared, Settings settings, int tri[], unsigned int seed, int qte_sections,
         int size, int reduc, int counter, int time, char *title) {
    // NO SEMAPHORE SINCE ONLY ONE PROCESS HAS ACCESS TO THE SHARED MEMORY
    Voiture *voitures = getAllVoitures(shared);
    pid_t pid;
    for (int i = 0; i < size - reduc; i++) {
        // WE RESET THE CARS IN THE PARENT SO THAT WE ARE SURE THEY ARE RESET BEFORE THE DISPLAY STARTS
        resetVoiture((voitures + tri[i]), qte_sections);
        pid = fork();
        if (pid < 0) {
            perror("Fork ");
            return 0;
        } else if (pid == 0) {
            setSeed(seed + counter * size + i);
            essai(shared, tri[i], time, settings);
            exit(0);
        }
    }
    dtVoiture(voitures, 0);
    //while((wait(NULL)) > 0);
    displayEssai(shared, tri, size, title, size - reduc, qte_sections);

    pauseCourse();
    return 1;
}

int lancerFinale
        (SharedInfo shared, Settings settings, int tri[], unsigned int seed, int qte_sections,
         int size, int reduc, int counter, int qte_tours_finale) {
    // NO SEMAPHORE SINCE ONLY ONE PROCESS HAS ACCESS TO THE SHARED MEMORY
    Voiture *voitures = getAllVoitures(shared);
    pid_t pid;
    double longueur = calcLongueur(settings);
    for (int i = 0; i < size - reduc; i++) {
        // WE RESET THE CARS IN THE PARENT SO THAT WE ARE SURE THEY ARE RESET BEFORE THE DISPLAY STARTS
        resetVoiture((voitures + tri[i]), qte_sections);
        pid = fork();
        if (pid < 0) {
            perror("Fork ");
            return 0;
        } else if (pid == 0) {
            setSeed(seed + counter * size + i);
            finale(shared, tri[i], qte_sections * qte_tours_finale, settings);
            exit(0);
        }
    }
    dtVoiture(voitures, 0);
    //while((wait(NULL)) > 0);
    displayFinale(shared, tri, size, "FINALE:", size - reduc, qte_sections, longueur);

    return 1;
}

void pauseCourse() {
    printf("Press enter to start the next phase...\n");
    while (getchar() != '\n');
}

int initVoitures(SharedInfo shared, char *noms[], size_t size, int qte_sections) {
    // NO SEMAPHORE SINCE ONLY ONE PROCESS HAS ACCESS TO THE SHARED MEMORY
    Voiture *voitures = getAllVoitures(shared);
    if (!voitures) {
        return 0;
    }

    for (int i = 0; i < size; i++) {
        resetVoiture(&voitures[i], qte_sections);
        voitures[i].nomVoiture = noms[i];
        voitures[i].state.totalKmParcouru=0;
        voitures[i].state.usurePneu=0;
        voitures[i].state.KmParcouruPneu=0;
    }
    dtVoiture(voitures, 0);
    return 1;
}

void validateSettings(Settings settings) {
    struct pair {
        char *key;
        enum EnumTypes type;
    };
    struct pair keys[] = {
            {"noms_voitures",     StringArray},
            {"longueur_sections", NumberArray},
            {"vitesse_moyenne",   Number},
            {"qte_tours_finale",  Number},
            {"shm_key",           Number},
            {"sem_key",           Number},
            {"delay",             Number},
    };

    int size = sizeof(keys) / sizeof(struct pair);
    for (int i = 0; i < size; ++i) {
        if (SettingsGet(settings, keys[i].key) == NULL) {
            printf("Le fichier de config est incomplet: la variable \"%s\" n'est pas définie.", keys[i].key);
            exit(-2);
        } else if (SettingsGetType(settings, keys[i].key) != keys[i].type) {
            printf("Le type %s était attendu pour la variable \"%s\", mais on a le type %s à la place.",
                   GetTypeName(keys[i].type), keys[i].key,
                   GetTypeName(SettingsGetType(settings, keys[i].key)));
            exit(-2);
        }
    }
}
