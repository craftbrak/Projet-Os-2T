#define QTE_SECTIONS 3
#define QTE_VOITURES 5

#include <stdio.h>
#include "Libs/randomLib.h"
#include "Libs/voiture.h"
#include "Libs/course.h"

void initVoitures(Voiture (*voitures)[], char* (*noms)[]);

void main () {
    int i,j;

    char* noms[QTE_VOITURES] = {"10", "20", "30", "40", "50"};
    Voiture voitures[QTE_VOITURES];

    initVoitures(&voitures, &noms);
    setSeed(generateSeed());

    for (j=0;j<QTE_VOITURES;j++) {
        essai(&voitures[j], 3000, 35, 55);

        printf("Voiture %s: \n", voitures[j].nomVoiture);
        for (i=1;i<=QTE_SECTIONS;i++) {
             printf("S%i : %lf s \n", i, voitures[j].sections[i-1]);
        }
    }
}

void initVoitures(Voiture (*voitures)[], char* (*noms)[]) {
    int i;

    for (i=0;i<QTE_VOITURES;i++) {
        resetVoiture(&(*voitures)[i]);
        (*voitures)[i].nomVoiture = (*noms)[i];
    }
}
