#include <stdio.h>
#include "../config.h"
#include "voiture.h"
#include "randomLib.h"
#include "course.h"

void resetVoiture(Voiture*);
double tempsRandom(double, double);
void validerTempsSection (Voiture*, int, double);

// Essais et qualifs
void essai (Voiture* voiture, int tempsTotalMax, int vitesseMoyenne, double longueurSections[])
{
    int sectionActuelle = 0;
    double lapTime = 0.0;
    double tempsSection;

    resetVoiture(voiture);

    while(1) {
        tempsSection = tempsRandom(longueurSections[sectionActuelle], vitesseMoyenne);
    
        validerTempsSection(voiture, sectionActuelle, tempsSection);

        lapTime += tempsSection;
        voiture->TotalTime += tempsSection;
        if (voiture->TotalTime >= tempsTotalMax) {
            break;
        }

        //Test de crash et test d'abandon à ajouter ici

        sectionActuelle++;
        if (sectionActuelle == QTE_SECTIONS) {
            sectionActuelle = 0;
            if (voiture->bestLap < 0 || lapTime < voiture->bestLap) {
                voiture->bestLap = lapTime;
            }
            lapTime = 0;
        }
    }
}

// Course finale
void finale (Voiture* voiture, int toursMax, int vitesseMoyenne, double longueurSections[]) {
    int sectionActuelle = 0;
    double lapTime = 0.0;
    double tempsSection;
    
    resetVoiture(voiture);

    while (voiture->qteSections < toursMax * QTE_SECTIONS) {
        tempsSection = tempsRandom(longueurSections[sectionActuelle], vitesseMoyenne);

        validerTempsSection(voiture, sectionActuelle, tempsSection);
        
        lapTime += tempsSection;
        voiture->TotalTime += tempsSection;
       
        //Test crash ou abandon ici.

        sectionActuelle++;
        voiture->qteSections++;
        if (sectionActuelle == QTE_SECTIONS) {
            sectionActuelle = 0;
            if (voiture->bestLap < 0 || lapTime < voiture->bestLap) {
                voiture->bestLap = lapTime;
            }
            lapTime = 0;
        }
    }
}

void validerTempsSection (Voiture* voiture, int sectionActuelle, double temps) {
    if (voiture->sections[sectionActuelle] < 0 || temps < voiture->sections[sectionActuelle]) {
        voiture->sections[sectionActuelle] = temps;
        //printf("%s | S%i : %lf s \n", voiture->nomVoiture ,section    Actuelle + 1, tempsSection);
    }
}

double tempsRandom (double longueur, double vitesseMoyenne) {
    double tempsMoyen = (longueur * 3600) / vitesseMoyenne;
    double tempsDelta = tempsMoyen / 5;

    return randomRange( tempsMoyen - tempsDelta, tempsMoyen + tempsDelta);
}

void resetVoiture (Voiture* voiture) {
  for (int i=0;i<QTE_SECTIONS; i++){
    voiture->sections[i] = -1;
  }
  voiture->qteSections = 0;
  voiture->bestLap = -1;
  voiture->TotalTime = 0.0;
  voiture->pit = 0;
  voiture->out = 0;
}
