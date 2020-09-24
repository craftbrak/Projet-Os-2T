#include <stdio.h>
#include "voiture.h"
#include "randomRange.h"
#define QTE_SECTIONS = 3

// Essais
void essai (Voiture *voiture, int tempsTotalMax, int tempsMin, int tempsMax) 
{
  double tempsSection;
  int sectionActuelle = 1;  

  //Reset les propriétés avant la course
  voiture->reset();
  
  while(1) {
    tempsSection = randomRange(tempsMin, tempsMax);

    if (voiture->sections[sectionActuelle] == 0 || tempsSection < voiture->sections[sectionActuelle]) {
      voiture->sections[sectionActuelle] = tempsSection;
    }
    voiture->TotalTime += tempsSection;
    if (voiture->TotalTime >= tempsTotalMax) {
      break;
    }

    //Test de crash et test d'abandon à ajouter ici

    if (sectionActuelle == QTE_SECTIONS) {
      sectionActuelle = 0;
    }
    sectionActuelle++;
  }
}
