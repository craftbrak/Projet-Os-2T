#include <stdio.h>
#include "config.h"
#include "Libs/randomLib.h"
#include "Libs/voiture.h"
#include "Libs/course.h"
#include "Libs/sorting.h"


void initVoitures(Voiture voitures[], char* noms[], size_t);

void main () {
    int i,j;

    int vitesseMoyenne = VITESSE_MOYENNE; 
    double longueurSections[] = LONGUEUR_SECTIONS;

    char* noms[] = NOMS_VOITURES;
    size_t qteVoitures = sizeof noms / sizeof noms[0];
    Voiture voitures[qteVoitures];
    int indexTri[qteVoitures];

    initVoitures(voitures, noms, qteVoitures);
    setSeed(generateSeed());

    for (i=0;i<qteVoitures;i++) {
        essai(&voitures[i], 3000, vitesseMoyenne, longueurSections);
/*
        printf("Voiture %s: \n", voitures[i].nomVoiture);
        for (j=1;j<=QTE_SECTIONS;j++) {
             printf("S%i : %lf s \n", j, voitures[i].sections[j-1]);
        }
*/        
    }
    for (i=1;i<=QTE_SECTIONS;i++) {
        sortSection(voitures, i - 1, indexTri, qteVoitures);
        printf("SECTION %i: \n", i);
        printf("  1er: %s avec %lf s\n", voitures[indexTri[0]].nomVoiture, voitures[indexTri[0]].sections[i - 1]);
        printf("  2eme: %s avec %lf s\n", voitures[indexTri[1]].nomVoiture, voitures[indexTri[1]].sections[i - 1]);
        printf("  3eme: %s avec %lf s\n", voitures[indexTri[2]].nomVoiture, voitures[indexTri[2]].sections[i - 1]);
    }
}

void initVoitures(Voiture voitures[], char* noms[], size_t size) {
    for (int i=0;i<size;i++) {
        resetVoiture(&voitures[i]);
        voitures[i].nomVoiture = noms[i];
    }
}
