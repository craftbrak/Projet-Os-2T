#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "config.h"
#include "Libs/randomLib.h"
#include "Libs/voiture.h"
#include "Libs/sharedmem.h"
#include "Libs/course.h"
#include "Libs/sorting.h"
#include "Libs/display.h"

#define SHM_KEY 0x1234

void pauseCourse();

void initVoitures(Voiture voitures[], char* noms[], size_t);

int main () {
    int i;

    SharedInfo shared;
    int vitesseMoyenne = VITESSE_MOYENNE;
    double longueurSections[] = LONGUEUR_SECTIONS;

    char* noms[] = NOMS_VOITURES;
    int qteVoitures = sizeof noms / sizeof noms[0];
    Voiture* voitures;
    Voiture* tri[qteVoitures];

    if (!(sharedMemInit(&shared, SHM_KEY, qteVoitures))) {
        return 1;
    }

    voitures = getAllVoitures(shared);
    if (!voitures) {
        return 1;
    }

    generateOrderedArr(voitures, tri, qteVoitures);

    initVoitures(voitures, noms, qteVoitures);
    setSeed(generateSeed());

    for (i=0;i<qteVoitures;i++) {
        essai(tri[i], 5400, vitesseMoyenne, longueurSections);
    }
    displayEssai(tri, qteVoitures, "P1:\n", qteVoitures);

    pauseCourse();
    for (i=0;i<qteVoitures;i++) {
        essai(tri[i], 5400, vitesseMoyenne, longueurSections);
    }        
    displayEssai(tri, qteVoitures, "P2:\n", qteVoitures);

    pauseCourse();
    for (i=0;i<qteVoitures;i++) {
        essai(tri[i], 3600, vitesseMoyenne, longueurSections);
    }        
    displayEssai(tri, qteVoitures, "P3:\n", qteVoitures);

    pauseCourse();
    for (i=0;i<qteVoitures;i++) {
        essai(tri[i], 1080, vitesseMoyenne, longueurSections);
    }
    displayEssai(tri, qteVoitures, "Q1:\n", qteVoitures);

    pauseCourse();
    for (i=0;i<qteVoitures - 5;i++) { 
        essai(tri[i], 900, vitesseMoyenne, longueurSections);
    }
    displayEssai(tri, qteVoitures, "Q2:\n", qteVoitures - 5);

    pauseCourse();
    for (i=0;i<qteVoitures - 10;i++) {
        essai(tri[i], 720, vitesseMoyenne, longueurSections);
    }
    displayEssai(tri, qteVoitures, "Q3:\n", qteVoitures - 10);

    pauseCourse();
    for (i=0;i<qteVoitures - 10;i++) {
        finale(tri[i], QTE_SECTIONS * QTE_TOURS_FINALE, vitesseMoyenne, longueurSections);
    }
    displayFinale(tri, qteVoitures, "FINALE:\n", qteVoitures - 10, QTE_TOURS_FINALE * QTE_SECTIONS);

    dtAllVoitures(voitures);
}

void pauseCourse () {
    printf("Press enter to start the next phase...\n");
    while (getchar() != '\n'){};
}

void initVoitures(Voiture voitures[], char* noms[], size_t size) {
    for (int i=0;i<size;i++) {
        resetVoiture(&voitures[i]);
        voitures[i].nomVoiture = noms[i];
    }
}
