#include <stdio.h>
#include "config.h"
#include "Libs/randomLib.h"
#include "Libs/voiture.h"
#include "Libs/course.h"
#include "Libs/sorting.h"
#include "Libs/display.h"

void initVoitures(Voiture voitures[], char* noms[], size_t);

void main () {
    int i,j;

    int vitesseMoyenne = VITESSE_MOYENNE; 
    double longueurSections[] = LONGUEUR_SECTIONS;

    char* noms[] = NOMS_VOITURES;
    size_t qteVoitures = sizeof noms / sizeof noms[0];
    Voiture voitures[qteVoitures];
    Voiture* arrTri[3];

    initVoitures(voitures, noms, qteVoitures);
    setSeed(generateSeed());


    printf("Course d'essai:\n");    
    for (i=0;i<qteVoitures;i++) {
        essai(&voitures[i], 5400, vitesseMoyenne, longueurSections);
/*
        printf("Voiture %s: \n", voitures[i].nomVoiture);
        for (j=1;j<=QTE_SECTIONS;j++) {
             printf("S%i : %lf s \n", j, voitures[i].sections[j-1]);
        }
*/        
    }
    displayEssai(voitures, qteVoitures);
}

void initVoitures(Voiture voitures[], char* noms[], size_t size) {
    for (int i=0;i<size;i++) {
        resetVoiture(&voitures[i]);
        voitures[i].nomVoiture = noms[i];
    }
}
