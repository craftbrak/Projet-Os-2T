#include <stdio.h>
#include "voiture.h"
#include "randomLib.h"
#include "course.h"

void resetVoiture(Voiture*);

// Essais
void essai (Voiture* voiture, int tempsTotalMax, int tempsMin, int tempsMax)
{
  double tempsSection;
  int sectionActuelle = 1;

  resetVoiture(voiture);

  while(1) {
    tempsSection = randomRange(tempsMin, tempsMax);
    
    if (voiture->sections[sectionActuelle - 1] < tempsMin || tempsSection < voiture->sections[sectionActuelle - 1]) {
      voiture->sections[sectionActuelle - 1] = tempsSection;
      //printf("%s | S%i : %lf s \n", voiture->nomVoiture ,sectionActuelle, tempsSection);
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

void resetVoiture (Voiture* voiture) {
  int i;  

  //Je sais plus si la flèche retourne un pointeur ou l'objet, à tester
  for (i=1;i<QTE_SECTIONS; i++){
    voiture->sections[i] = 0.0;
  }
  voiture->TotalTime = 0.0;
}
