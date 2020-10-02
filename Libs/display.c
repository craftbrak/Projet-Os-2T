#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../config.h"
#include "voiture.h"
#include "sorting.h"
#include "display.h"

void displayEssai (Voiture voitures[], int length) {
    Voiture* triTemps[length];
    Voiture* triSections[QTE_SECTIONS][3];
    
    sortLapTime(voitures, triTemps, length);
    for (int i=0;i<QTE_SECTIONS;i++) {
        sortSection(voitures, i, triSections[i], length, 3);
    }

    headerEssai();
    entryEssai(triTemps[0], NULL, triSections);
    for (int i=1;i<length;i++) {
        entryEssai(triTemps[i],triTemps[i-1],triSections);
    }

    printf("\n\033[0;46m 1er \033[0;42m 2eme \033[0;44m 3eme \033[0m\n");
}

void headerEssai() {
    printf(" VOITURE ");
    for (int i=0;i<QTE_SECTIONS;i++) {
        printf("    S%i     ", i + 1);
    }
    printf("   TOUR    ");
    printf("   GAP    ");
    printf(" STAND \n");
    for (int i=0;i < 37 + QTE_SECTIONS*11;i++){
        printf("━");
    }
    printf("\n");
}

void entryEssai(Voiture* voiture, Voiture* precedent, Voiture* triSections[QTE_SECTIONS][3]) {
    switch (strlen(voiture->nomVoiture)) {
        case 1:
            printf("    %s    ", voiture->nomVoiture);
            break;
        case 2:
            printf("   %s    ", voiture->nomVoiture);
    }
    for (int i=0;i<QTE_SECTIONS;i++) {
        if (voiture == triSections[i][0]) {
            printf("\033[0;46m");
        } else if (voiture == triSections[i][1]) {
            printf("\033[0;42m");
        } else if (voiture == triSections[i][2]) {
            printf("\033[0;44m");
        }
        printf(" %.4lfs \033[0m ", voiture->sections[i]);
    }
    printf(" %.4lfs ", voiture->bestLap);
    if (precedent) {
        printf(" +%.4lfs ", voiture->bestLap - precedent->bestLap);
    } else {
        printf("   ----   ");
    }
    if (voiture->pit) {
        printf(" %.0lfs ");
    } else {
        printf("  ---  ");
    }
    printf("\n");
}
