#include <stdio.h>
#include "config.h"
#include "Libs/randomLib.h"
#include "Libs/voiture.h"
#include "Libs/course.h"


void initVoitures(Voiture (*voitures)[], char* (*noms)[], size_t);

void main () {
    int i,j;

    int vitesseMoyenne = VITESSE_MOYENNE; 
    double longueurSections[] = LONGUEUR_SECTIONS;

    char* noms[] = NOMS_VOITURES;
    size_t qteVoitures = sizeof noms / sizeof noms[0];
    Voiture voitures[qteVoitures];

    initVoitures(&voitures, &noms, qteVoitures);
    setSeed(generateSeed());

    for (j=0;j<qteVoitures;j++) {
        essai(&voitures[j], 3000, vitesseMoyenne, &longueurSections);

        printf("Voiture %s: \n", voitures[j].nomVoiture);
        for (i=1;i<=QTE_SECTIONS;i++) {
             printf("S%i : %lf s \n", i, voitures[j].sections[i-1]);
        }
    }
}

void initVoitures(Voiture (*voitures)[], char* (*noms)[], size_t size) {
    int i;

    for (i=0;i<size;i++) {
        resetVoiture(&(*voitures)[i]);
        (*voitures)[i].nomVoiture = (*noms)[i];
    }
}
