#include <stdio.h>
#include "../config.h"
#include "voiture.h"
#include "randomLib.h"
#include "course.h"

void resetVoiture(Voiture*);

// Essais
void essai (Voiture* voiture, int tempsTotalMax, int vitesseMoyenne, double longueurSections[])
{
  int sectionActuelle = 0;
  double lapTime = 0.0;
  double tempsMoyen, tempsDelta, tempsSection;

  resetVoiture(voiture);

  while(1) {
    tempsMoyen = (longueurSections[sectionActuelle] / vitesseMoyenne) * 3600.0; 
    tempsDelta = tempsMoyen * 0.2;

    tempsSection = randomRange(tempsMoyen - tempsDelta, tempsMoyen + tempsDelta);
    
    if (voiture->sections[sectionActuelle] < 0.1 || tempsSection < voiture->sections[sectionActuelle]) {
      voiture->sections[sectionActuelle] = tempsSection;
      //printf("%s | S%i : %lf s \n", voiture->nomVoiture ,sectionActuelle + 1, tempsSection);
    }

    lapTime += tempsSection;
    voiture->TotalTime += tempsSection;
    if (voiture->TotalTime >= tempsTotalMax) {
      break;
    }

    //Test de crash et test d'abandon à ajouter ici

    sectionActuelle++;
    if (sectionActuelle == QTE_SECTIONS) {
      sectionActuelle = 0;
      if (voiture->bestLap < 0.1 || lapTime < voiture->bestLap) {
        voiture->bestLap = lapTime;
      }
      lapTime = 0;
    }
  }
}

void resetVoiture (Voiture* voiture) {
  for (int i=1;i<QTE_SECTIONS; i++){
    voiture->sections[i] = 0.0;
  }
  voiture->bestLap = 0.0;
  voiture->TotalTime = 0.0;
  voiture->pit = 0;
  voiture->out = 0;
}
