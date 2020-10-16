#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../config.h"
#include "voiture.h"
#include "sorting.h"
#include "display.h"

void displayEssai (Voiture* triTemps[], int length, char* titre, int amount) {
    Voiture* triSections[QTE_SECTIONS][3];
    
    sortLapTime(triTemps, amount);
    for (int i=0;i<QTE_SECTIONS;i++) {
        sortSection(triTemps, i, triSections[i], amount, 3);
    }

    printf("%s \n", titre);
    headerEssai();
    entryEssai(triTemps[0], NULL, triSections, amount);
    for (int i=1;i<length;i++) {
        entryEssai(triTemps[i],triTemps[i-1],triSections, i < amount);
    }

    printf("\n\033[30m\033[103m 1er \033[47m 2eme \033[0m\033[48;5;172m 3eme \033[0m\n");
}

void headerEssai() {
    printf(" VOITURE ");
    for (int i=0;i<QTE_SECTIONS;i++) {
        printf("   S%i    ", i + 1);
    }
    printf("   TOUR   ");
    printf("   GAP   ");
    printf(" STAND \n");
    for (int i=0;i < 35 + QTE_SECTIONS*9;i++){
        printf("━");
    }
    printf("\n");
}

void entryEssai(Voiture* voiture, Voiture* precedent, Voiture* triSections[QTE_SECTIONS][3], int participe) {
    if (!participe) {
        printf("\033[100m");
    }
    switch (strlen(voiture->nomVoiture)) {
        case 1:
            printf("    %s    ", voiture->nomVoiture);
            break;
        case 2:
            printf("   %s    ", voiture->nomVoiture);
    }
    for (int i=0;i<QTE_SECTIONS;i++) {
        if (participe) {
            if (voiture == triSections[i][0]) {
                printf("\033[30m\033[103m");
            } else if (voiture == triSections[i][1]) {
                printf("\033[30m\033[47m");
            } else if (voiture == triSections[i][2]) {
                printf("\033[48;5;172m");
            }
            printf(" %.3fs \033[0m", voiture->sections[i]);
        } else {
            printf(" %.3fs ", voiture->sections[i]);
        }
    }
    printf(" %.3fs ", voiture->bestLap);
    if (precedent && participe) {
        printf(" %+.3fs ", voiture->bestLap - precedent->bestLap);
    } else {
        printf("   ---   ");
    }
    if (voiture->pit) {
        printf(" %.0fs ");
    } else {
        printf("  ---  ");
    }
    printf("\033[0m\n");
}

void displayFinale (Voiture* triTemps[], int length, char* titre, int amount, int maxSections) {
    Voiture* triSections[QTE_SECTIONS][3];
    Voiture* triBestLap[amount];
    double minTime = getMinTime(triTemps, amount, maxSections);

    copyArr(triTemps, triBestLap, amount);

    sortLapTime(triBestLap, amount);
    sortSpeed(triTemps, amount);
    for (int i=0;i<QTE_SECTIONS;i++) {
        sortSection(triTemps, i, triSections[i], amount, 3);
    }

    printf("%s \n", titre);
    headerFinale();
    entryFinale(triTemps[0], NULL, triSections, triBestLap, amount, minTime);
    for (int i=1;i<length;i++) {
        entryFinale(triTemps[i],triTemps[i-1],triSections, triBestLap, i < amount, minTime);
    }

    printf("\n\033[30m\033[103m 1er \033[47m 2eme \033[0m\033[48;5;172m 3eme \033[0m\n");
}

void headerFinale() {
    printf(" VOITURE ");
    for (int i=0;i<QTE_SECTIONS;i++) {
        printf("   S%i    ", i + 1);
    }
    printf("   TOUR   ");
    printf(" TOURS ");
    printf("   TEMPS   ");
    printf("    GAP   ");
    printf(" STAND \n");

    for (int i=0;i < 54 + QTE_SECTIONS*9;i++){
        printf("━");
    }
    printf("\n");
}

void entryFinale
    (Voiture* voiture, Voiture* precedent,
     Voiture* triSections[QTE_SECTIONS][3], Voiture* triBestLap[],
     int participe, double tempsMin)
{
    int tours = voiture->qteSections / QTE_SECTIONS;
    double tMin;
    double gap = 0;
    if  (tempsMin < 0 || tempsMin > voiture->TotalTime) {
        tMin = voiture->TotalTime;
    } else {
        tMin = tempsMin;
    }
    if (precedent) {
        gap = tMin * (1 - voiture->speed / precedent->speed);
    }

    if (!participe) {
        printf("\033[100m");
    }
    switch (strlen(voiture->nomVoiture)) {
        case 1:
            printf("    %s    ", voiture->nomVoiture);
            break;
        case 2:
            printf("   %s    ", voiture->nomVoiture);
    }
    for (int i=0;i<QTE_SECTIONS;i++) {
        if (participe) {
            if (voiture == triSections[i][0]) {
                printf("\033[30m\033[103m");
            } else if (voiture == triSections[i][1]) {
                printf("\033[30m\033[47m");
            } else if (voiture == triSections[i][2]) {
                printf("\033[48;5;172m");
            }
            printf(" %.3fs \033[0m", voiture->sections[i]);
        } else {
            printf(" %.3fs ", voiture->sections[i]);
        }
    }
    if (participe) {
        if (voiture == triBestLap[0]) {
            printf("\033[30m\033[103m");
        } else if (voiture == triBestLap[1]) {
            printf("\033[30m\033[47m");
        } else if (voiture == triBestLap[2]) {
            printf("\033[48;5;172m");
        }
        printf(" %.3fs \033[0m", voiture->bestLap);
    } else {
        printf(" %.3fs ", voiture->bestLap);
    }
    if (participe) {
        if (tours<10) {
            printf("   %i   ", tours);
        } else {
            printf("  %i   ", tours);
        }
    } else {
        printf("  ---  ");
    }
    if (participe) {
        printf(" %.3fs ", tMin);
    } else {
        printf("    ---    ");
    }
    if (precedent && participe) {
        if (gap < 10) {
            printf("  %+.3fs ", gap);
        } else {
            printf(" %+.3fs ", gap);
        }
    } else {
        printf("   ----   ");
    }
    if (participe && voiture->pit) {
        printf(" %.0fs ", voiture->pitTime);
    } else {
        printf("  ---  ");
    }
    printf("\033[0m\n");
}
