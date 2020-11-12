#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "Settings.h"
#include "voiture.h"
#include "sharedmem.h"
#include "sorting.h"
#include "display.h"

void displayEssai(SharedInfo shared, int tri[], int length, char *title, int amount, int qte_sections) {
    Voiture voitures[length];
    int triSections[qte_sections][3];
    int pos_before[length];

    for (int i = 0; i < length; ++i) {
        pos_before[i] = -1;
    }

    while (1) {
        system("clear");

        if (!(getAllVoituresCopy(voitures, shared))) {
            return;
        }

        sortLapTime(voitures, tri, amount);
        for (int i = 0; i < qte_sections; i++) {
            sortSection(voitures, tri, i, triSections[i], amount, 3);
        }

        printf("%s \n\n", title);
        headerEssai(qte_sections);

        for (int i = 0; i < length; i++) {
            entryEssai(voitures, tri, i, pos_before[tri[i]], (int *) triSections, i < amount, qte_sections);
        }

        for (int i = 0; i < length; ++i) {
            pos_before[tri[i]] = i;
        }

        printf("\n\033[30m\033[103m 1er \033[47m 2eme \033[0m\033[48;5;172m 3eme \033[0m\n");

        if (courseFinie(voitures, tri, amount)) {
            return;
        }

        sleep(1);
    }
}

void headerEssai(int qte_sections) {
    printf(" VOITURE ");
    for (int i = 0; i < qte_sections; i++) {
        printf("   S%i    ", i + 1);
    }
    printf("   TOUR   ");
    printf("    GAP   ");
    printf(" STAND \n");
    for (int i = 0; i < 35 + qte_sections * 9; i++) {
        printf("━");
    }
    printf("\n");
}

void entryEssai(Voiture voitures[], int tri[], int index, int pos_before, int *triSections, int participe, int qte_sections) {
    Voiture *voiture = voitures + tri[index];

    if (pos_before < 0 || pos_before == index) {
        printf("  ");
    } else {
        const char *color = pos_before < index ? "\033[31m" : "\033[32m";
        const char *place = pos_before < index ? "↓" : "↑";
        printf(" %s%s\033[39m", color, place);
    }

    if (!participe) {
        printf("\033[100m");
    } else {
        if (voiture->out) {
            printf("\033[31m");
        } else if (voiture->done) {
            printf("\033[2m");
        }
    }
    switch (strlen(voiture->nomVoiture)) {
        case 1:
            printf("  %s    ", voiture->nomVoiture);
            break;
        case 2:
            printf(" %s    ", voiture->nomVoiture);
    }
    if (participe) {
        printf("\033[0m");
    }
    for (int i = 0; i < qte_sections; i++) {
        if (participe) {
            if (tri[index] == triSections[3*i + 0]) {
                printf("\033[30m\033[103m");
            } else if (tri[index] == triSections[3*i + 1]) {
                printf("\033[30m\033[47m");
            } else if (tri[index] == triSections[3*i + 2]) {
                printf("\033[48;5;172m");
            }
        }
        printTime(2, voiture->sections[i], 0);
        if (participe) {
            printf("\033[0m");
        }
    }
    printTime(3, voiture->bestLap, 0);
    if (index > 0 && participe) {
        printTime(2, voiture->bestLap - (voitures + tri[index - 1])->bestLap, 1);
    } else {
        printf("    ---   ");
    }
    if (voiture->pit) {
        printf(" %.0fm ", voiture->pitTime);
    } else {
        printf("  ---  ");
    }
    printf("\033[0m\n");
}

void displayFinale(SharedInfo shared, int tri[], int length, char *title,
                   int amount, int qte_sections, double longueur) {
    Voiture voitures[length];
    int triSections[qte_sections][3];
    int triBestLap[amount];
    double minTime;
    int pos_before[length];

    for (int i = 0; i < length; ++i) {
        pos_before[i] = -1;
    }

    while (1) {
        system("clear");

        if (!(getAllVoituresCopy(voitures, shared))) {
            return;
        }

        minTime = getMinTime(voitures, tri, amount);

        copyArr(tri, triBestLap, amount);

        sortLapTime(voitures, triBestLap, amount);
        sortSpeed(voitures, tri, amount);
        for (int i = 0; i < qte_sections; i++) {
            sortSection(voitures, tri, i, triSections[i], amount, 3);
        }

        if (minTime >= 0) {
            printf("%s  TEMPS: %.3fs \n\n", title, minTime);
        } else {
            printf("%s  FINI \n\n", title);
        }

        headerFinale(qte_sections);

        for (int i = 0; i < length; i++) {
            entryFinale(voitures, tri, i, pos_before[tri[i]], (int *) triSections, triBestLap, i < amount, minTime, qte_sections, longueur);
        }

        for (int i = 0; i < length; ++i) {
            pos_before[tri[i]] = i;
        }

        printf("\n\033[30m\033[103m 1er \033[47m 2eme \033[0m\033[48;5;172m 3eme \033[0m\n");

        if (courseFinie(voitures, tri, amount)) {
            return;
        }

        sleep(1);
    }
}

void headerFinale(int qte_sections) {
    printf(" VOITURE ");
    for (int i = 0; i < qte_sections; i++) {
        printf("   S%i    ", i + 1);
    }
    printf("   TOUR   ");
    printf(" TOURS ");
    printf("   TEMPS   ");
    printf("    GAP   ");
    printf(" STAND \n");

    for (int i = 0; i < 54 + qte_sections * 9; i++) {
        printf("━");
    }
    printf("\n");
}

void entryFinale(Voiture voitures[], int tri[], int index, int pos_before, int *triSections, int triBestLap[],
                 int participe, double tempsMin, int qte_sections, double longueurTour) {
    Voiture *voiture = voitures + tri[index];
    double tMin = tempsMin;
    double gap = 0;
    double tours;
    if (tempsMin < 0 || tempsMin > voiture->TotalTime) {
        tMin = voiture->TotalTime;
    }
    if (index > 0) {
        gap = tMin * (1 - voiture->speed / (voitures + tri[index - 1])->speed);
    }

    tours = voiture->speed * tMin / longueurTour;

    if (pos_before < 0 || pos_before == index) {
        printf("  ");
    } else {
        const char *color = pos_before < index ? "\033[31m" : "\033[32m";
        const char *place = pos_before < index ? "↓" : "↑";
        printf(" %s%s\033[39m", color, place);
    }

    if (!participe) {
        printf("\033[100m");
    } else {
        if (voiture->out) {
            printf("\033[31m");
        } else if (voiture->done) {
            printf("\033[2m");
        }
    }
    switch (strlen(voiture->nomVoiture)) {
        case 1:
            printf("  %s    ", voiture->nomVoiture);
            break;
        case 2:
            printf(" %s    ", voiture->nomVoiture);
    }
    if (participe) {
        printf("\033[0m");
    }
    for (int i = 0; i < qte_sections; i++) {
        if (participe) {
            if (tri[index] == triSections[3*i + 0]) {
                printf("\033[30m\033[103m");
            } else if (tri[index] == triSections[3*i + 1]) {
                printf("\033[30m\033[47m");
            } else if (tri[index] == triSections[3*i + 2]) {
                printf("\033[48;5;172m");
            }
        }
        printTime(2, voiture->sections[i], 0);
        if (participe) {
            printf("\033[0m");
        }
    }
    if (participe) {
        if (tri[index] == triBestLap[0]) {
            printf("\033[30m\033[103m");
        } else if (tri[index] == triBestLap[1]) {
            printf("\033[30m\033[47m");
        } else if (tri[index] == triBestLap[2]) {
            printf("\033[48;5;172m");
        }
    }
    printTime(3, voiture->bestLap, 0);
    if (participe) {
        printf("\033[0m");
    }
    if (participe) {
        printf("  %2.0f   ", tours);
    } else {
        printf("  ---  ");
    }
    if (participe) {
        printTime(4, tMin, 0);
    } else {
        printf("    ---    ");
    }
    if (participe) {
        printTime(2, gap, 1);
    } else {
        printf("   ----   ");
    }
    if (participe && voiture->pit) {
        printf(" %.0fm ", voiture->pitTime);
    } else {
        printf("  ---  ");
    }
    printf("\033[0m\n");
}

int courseFinie(Voiture voitures[], int tri[], int size) {
    for (int i = 0; i < size; i++) {
        if (!voitures[tri[i]].done) {
            return 0;
        }
    }
    return 1;
}

double calcLongueur(Settings settings) {
    NbrVector *longueurSections = (NbrVector *) SettingsGet(settings, "longueur_sections");
    double sum = 0;

    for (int i = 0; i < longueurSections->length; i++) {
        sum += longueurSections->data[i];
    }

    return sum;
}

void printTime(int maxDigits, double time, int sign) {
    if (time < 0) {
        for (int i = 0; i < maxDigits / 2 + 2; i++) {
            printf(" ");
        }
        printf("---");
        for (int i = 0; i < maxDigits / 2 + 2; i++) {
            printf(" ");
        }
        if (maxDigits % 2) {
            printf(" ");
        }
        return;
    }

    int max = puissance(10, maxDigits);

    while (time < max) {
        printf(" ");
        max /= 10;
        if (max == 1) {
            break;
        }
    }
    if (sign) {
        printf("%+.3fs ", time);
    } else {
        printf("%.3fs ", time);
    }
}

int puissance(int base, int exponent) {
    int result = base;
    for (int i = 1; i < exponent; i++) {
        result *= base;
    }
    return result;
}
