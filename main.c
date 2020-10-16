#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "Libs/randomLib.h"
#include "Libs/voiture.h"
#include "Libs/course.h"
#include "Libs/sorting.h"
#include "Libs/display.h"

void pause();

void initVoitures(Voiture voitures[], char* noms[], size_t);

void main () {
    int i,j;

    int vitesseMoyenne = VITESSE_MOYENNE; 
    double longueurSections[] = LONGUEUR_SECTIONS;

    char* noms[] = NOMS_VOITURES;
    size_t qteVoitures = sizeof noms / sizeof noms[0];
    Voiture voitures[qteVoitures];
    Voiture* tri[qteVoitures];
 
    generateOrderedArr(voitures, tri, qteVoitures);

    initVoitures(voitures, noms, qteVoitures);
    setSeed(generateSeed());

    for (i=0;i<qteVoitures;i++) {
        essai(tri[i], 5400, vitesseMoyenne, longueurSections);
    }
    displayEssai(tri, qteVoitures, "P1:\n", qteVoitures);

    pause();
    for (i=0;i<qteVoitures;i++) {
        essai(tri[i], 5400, vitesseMoyenne, longueurSections);
    }        
    displayEssai(tri, qteVoitures, "P2:\n", qteVoitures);

    pause();
    for (i=0;i<qteVoitures;i++) {
        essai(tri[i], 3600, vitesseMoyenne, longueurSections);
    }        
    displayEssai(tri, qteVoitures, "P3:\n", qteVoitures);

    pause();
    for (i=0;i<qteVoitures;i++) {
        essai(tri[i], 1080, vitesseMoyenne, longueurSections);
    }
    displayEssai(tri, qteVoitures, "Q1:\n", qteVoitures);

    pause();
    for (i=0;i<qteVoitures - 5;i++) { 
        essai(tri[i], 900, vitesseMoyenne, longueurSections);
    }
    displayEssai(tri, qteVoitures, "Q2:\n", qteVoitures - 5);

    pause();
    for (i=0;i<qteVoitures - 10;i++) {
        essai(tri[i], 720, vitesseMoyenne, longueurSections);
    }
    displayEssai(tri, qteVoitures, "Q3:\n", qteVoitures - 10);

    pause();
    for (i=0;i<qteVoitures - 10;i++) {
        finale(tri[i], QTE_SECTIONS * QTE_TOURS_FINALE, vitesseMoyenne, longueurSections);
    }
    displayFinale(tri, qteVoitures, "FINALE:\n", qteVoitures - 10, QTE_TOURS_FINALE * QTE_SECTIONS);

}

void pause () {
    printf("Press enter to start the next phase...\n");
    while (getchar() != '\n'){};
}

void initVoitures(Voiture voitures[], char* noms[], size_t size) {
    for (int i=0;i<size;i++) {
        resetVoiture(&voitures[i]);
        voitures[i].nomVoiture = noms[i];
    }
}
