#include <stdio.h>
<<<<<<< Updated upstream
#include <unistd.h>
#include <sys/types.h>
=======
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
>>>>>>> Stashed changes
#include "config.h"
#include "Libs/randomLib.h"
#include "Libs/voiture.h"
#include "Libs/course.h"

<<<<<<< Updated upstream

void initVoitures(Voiture (*voitures)[], char* (*noms)[], size_t);
=======
void pauseCourse();

void initVoitures(Voiture voitures[], char *noms[], size_t);
>>>>>>> Stashed changes

int main() {
    int i, j;
    pid_t pid;
    int vitesseMoyenne = VITESSE_MOYENNE;
    double longueurSections[] = LONGUEUR_SECTIONS;

    char *noms[] = NOMS_VOITURES;
    size_t qteVoitures = sizeof noms / sizeof noms[0];
    Voiture voitures[qteVoitures];
<<<<<<< Updated upstream
    pid_t pid[qteVoiture] ;
    initVoitures(&voitures, &noms, qteVoitures);
    setSeed(generateSeed());

    for (j=0;j<qteVoitures;j++) {
        pid[j]=fork()
        if (pid[j] != 0){
            for (i=1;i<=QTE_SECTIONS;i++) {
                printf("S%i : %lf s \n", i, voitures[j].sections[i-1]);
            }
        } else{
            essai(&voitures[j], 3000, vitesseMoyenne, &longueurSections);

            printf("Voiture %s: \n", voitures[j].nomVoiture);
        }

    }

}

void initVoitures(Voiture (*voitures)[], char* (*noms)[], size_t size) {
    int i;

    for (i=0;i<size;i++) {
        resetVoiture(&(*voitures)[i]);
        (*voitures)[i].nomVoiture = (*noms)[i];
=======
    Voiture *tri[qteVoitures];

    generateOrderedArr(voitures, tri, qteVoitures);

    initVoitures(voitures, noms, qteVoitures);
    setSeed(generateSeed());

    for (i = 0; i < qteVoitures; i++) {
        pid = fork();
        if (pid < 0) {
            printf("le fork Kc ");
            return -1;
        } else if (pid == 0) {
            printf("%d \n",i);
            essai(tri[i], 5400, vitesseMoyenne, longueurSections);
            printf("%d return f\n",i);
            return 0;
        }
        essai(tri[i], 5400, vitesseMoyenne, longueurSections);
    }
    while ((wait(NULL)) > 0);
    displayEssai(tri, qteVoitures, "P1:\n", qteVoitures);

    pauseCourse();
    for (i = 0; i < qteVoitures; i++) {
        essai(tri[i], 5400, vitesseMoyenne, longueurSections);
    }
    displayEssai(tri, qteVoitures, "P2:\n", qteVoitures);

    pauseCourse();
    for (i = 0; i < qteVoitures; i++) {
        essai(tri[i], 3600, vitesseMoyenne, longueurSections);
    }
    displayEssai(tri, qteVoitures, "P3:\n", qteVoitures);

    pauseCourse();
    for (i = 0; i < qteVoitures; i++) {
        essai(tri[i], 1080, vitesseMoyenne, longueurSections);
    }
    displayEssai(tri, qteVoitures, "Q1:\n", qteVoitures);

    pauseCourse();
    for (i = 0; i < qteVoitures - 5; i++) {
        essai(tri[i], 900, vitesseMoyenne, longueurSections);
    }
    displayEssai(tri, qteVoitures, "Q2:\n", qteVoitures - 5);

    pauseCourse();
    for (i = 0; i < qteVoitures - 10; i++) {
        essai(tri[i], 720, vitesseMoyenne, longueurSections);
    }
    displayEssai(tri, qteVoitures, "Q3:\n", qteVoitures - 10);

    pauseCourse();
    for (i = 0; i < qteVoitures - 10; i++) {

        finale(tri[i], QTE_SECTIONS * QTE_TOURS_FINALE, vitesseMoyenne, longueurSections);
    }
    displayFinale(tri, qteVoitures, "FINALE:\n", qteVoitures - 10, QTE_TOURS_FINALE * QTE_SECTIONS);
    return 0;
}

void pauseCourse() {
    printf("Press enter to start the next phase...\n");
    while (getchar() != '\n') {};
}

void initVoitures(Voiture voitures[], char *noms[], size_t size) {
    for (int i = 0; i < size; i++) {
        resetVoiture(&voitures[i]);
        voitures[i].nomVoiture = noms[i];
>>>>>>> Stashed changes
    }
}
