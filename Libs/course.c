#include <stdlib.h>
#include "voiture.h"
#include "randomLib.h"
#include "course.h"

// Essais et qualifs
void essai (Voiture* voiture, int tempsTotalMax, double vitesseMoyenne, double longueurSections[], int qte_sections)
{
    int sectionActuelle = 0;
    double lapTime = 0.0;
    double tempsSection;

    resetVoiture(voiture, qte_sections);

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
        if (sectionActuelle == qte_sections) {
            sectionActuelle = 0;
            if (voiture->bestLap < 0 || lapTime < voiture->bestLap) {
                voiture->bestLap = lapTime;
            }
            lapTime = 0;
        }
    }
}

// Course finale
void finale (Voiture* voiture, int maxSections, double vitesseMoyenne, double longueurSections[], int qte_sections) {
    int sectionActuelle = 0;
    double distance = 0;
    double lapTime = 0.0;
    double tempsSection;
    
    resetVoiture(voiture, qte_sections);

    while (voiture->qteSections < maxSections) {
        tempsSection = tempsRandom(longueurSections[sectionActuelle], vitesseMoyenne);

        validerTempsSection(voiture, sectionActuelle, tempsSection);
        
        lapTime += tempsSection;
        distance += longueurSections[sectionActuelle];
        voiture->TotalTime += tempsSection;
        voiture->speed = distance / voiture->TotalTime;
       
        //Test crash ici.

        sectionActuelle++;
        voiture->qteSections++;
        if (sectionActuelle == qte_sections) {
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

void resetVoiture (Voiture* voiture, int qte_sections) {
  for (int i=0;i<qte_sections; i++){
    voiture->sections[i] = -1;
  }
  voiture->qteSections = 0;
  voiture->bestLap = -1;
  voiture->TotalTime = 0.0;
  voiture->pit = 0;
  voiture->out = 0;
}