#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Libs/randomLib.h"
#include "Libs/voiture.h"
#include "Libs/sharedmem.h"
#include "Libs/course.h"
#include "Libs/sorting.h"
#include "Libs/display.h"
#include "Libs/SettingsParser.h"

void pauseCourse();

void initVoitures(Voiture voitures[], char* noms[], size_t, int);
void freeVoitures(Voiture voitures[], size_t size);

void validateSettings(Settings settings);

int main (int argc, char **argv) {
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

    int i,j;
    double vitesseMoyenne = *((double *) SettingsGet(settings, "vitesse_moyenne"));
    int qte_sections = (int) *((double *) SettingsGet(settings, "qte_sections"));
    int qte_tours_finale = (int) *((double *) SettingsGet(settings, "qte_tours_finale"));
    int shm_key = (int) *((double *) SettingsGet(settings, "qte_tours_finale"));
    double *longueurSections = ((NbrVector *) SettingsGet(settings, "longueur_sections"))->data;
    StrVector *noms_voitures = SettingsGet(settings, "noms_voitures");

    char **noms = noms_voitures->data;
    int qteVoitures = (int) noms_voitures->length;
    Voiture *voitures;
    Voiture* tri[qteVoitures];
    SharedInfo shared;
    if (!(sharedMemInit(&shared, shm_key, qteVoitures))) {
        return 1;
    }

    voitures = getAllVoitures(shared);
    if (!voitures) {
        return 1;
    }

    generateOrderedArr(voitures, tri, qteVoitures);
    initVoitures(voitures, noms, qteVoitures, qte_sections);
    setSeed(generateSeed());

    for (i=0;i<qteVoitures;i++) {
        essai(tri[i], 5400, vitesseMoyenne, longueurSections, qte_sections);
    }
    displayEssai(tri, qteVoitures, "P1:\n", qteVoitures, qte_sections);

    pauseCourse();
    for (i=0;i<qteVoitures;i++) {
        essai(tri[i], 5400, vitesseMoyenne, longueurSections, qte_sections);
    }
    displayEssai(tri, qteVoitures, "P2:\n", qteVoitures, qte_sections);

    pauseCourse();
    for (i=0;i<qteVoitures;i++) {
        essai(tri[i], 3600, vitesseMoyenne, longueurSections, qte_sections);
    }
    displayEssai(tri, qteVoitures, "P3:\n", qteVoitures, qte_sections);

    pauseCourse();
    for (i=0;i<qteVoitures;i++) {
        essai(tri[i], 1080, vitesseMoyenne, longueurSections, qte_sections);
    }
    displayEssai(tri, qteVoitures, "Q1:\n", qteVoitures, qte_sections);

    pauseCourse();
    for (i=0;i<qteVoitures - 5;i++) {
        essai(tri[i], 900, vitesseMoyenne, longueurSections, qte_sections);
    }
    displayEssai(tri, qteVoitures, "Q2:\n", qteVoitures - 5, qte_sections);

    pauseCourse();
    for (i=0;i<qteVoitures - 10;i++) {
        essai(tri[i], 720, vitesseMoyenne, longueurSections, qte_sections);
    }
    displayEssai(tri, qteVoitures, "Q3:\n", qteVoitures - 10, qte_sections);

    pauseCourse();
    for (i=0;i<qteVoitures - 10;i++) {
        finale(tri[i], qte_sections * qte_tours_finale, vitesseMoyenne, longueurSections, qte_sections);
    }
    displayFinale(tri, qteVoitures, "FINALE:\n", qteVoitures - 10, qte_tours_finale * qte_sections, qte_sections);

    freeVoitures(voitures, qteVoitures); // à faire dans dtAllVoitures
    dtAllVoitures(voitures);
    SettingsDestroy(settings);
}

void pauseCourse () {
    printf("Press enter to start the next phase...\n");
    while (getchar() != '\n'){};
}

void initVoitures(Voiture voitures[], char* noms[], size_t size, int qte_sections) {
    for (int i=0;i<size;i++) {
        // TODO: Mémoire partagée ? (malloc sections & copie nom)
        voitures[i].sections = malloc(sizeof(double) * qte_sections);
        resetVoiture(&voitures[i], qte_sections);
        voitures[i].nomVoiture = noms[i];
    }
}

void freeVoitures(Voiture voitures[], size_t size) {
    for (int i=0;i<size;i++) {
        free(voitures[i].sections);
    }
}

void validateSettings(Settings settings) {
    struct pair {
        char *key;
        enum EnumTypes type;
    };
    struct pair keys[] = {
            {"noms_voitures",      StringArray},
            {"qte_sections",      Number},
            {"longueur_sections", NumberArray},
            {"vitesse_moyenne",   Number},
            {"qte_tours_finale",  Number},
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