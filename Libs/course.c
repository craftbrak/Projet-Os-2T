#include <stdio.h>
#include "../config.h"
#include "voiture.h"
#include "randomLib.h"
#include "course.h"

void resetVoiture(Voiture*);
void crash_test(Voiture*);
// Essais
void essai (Voiture* voiture, int tempsTotalMax, int vitesseMoyenne, double (*longueurSections)[])
{
  int sectionActuelle = 1;
  double tempsMoyen, tempsDelta, tempsSection;

  resetVoiture(voiture);

  while(1) {
    tempsMoyen = ((*longueurSections)[sectionActuelle - 1] / vitesseMoyenne) * 3600.0; 
    tempsDelta = tempsMoyen * 0.2;

    tempsSection = randomRange(tempsMoyen - tempsDelta, tempsMoyen + tempsDelta);
    
    if (voiture->sections[sectionActuelle - 1] < 0.1 || tempsSection < voiture->sections[sectionActuelle - 1]) {
      voiture->sections[sectionActuelle - 1] = tempsSection;
      //printf("%s | S%i : %lf s \n", voiture->nomVoiture ,sectionActuelle, tempsSection);
    }

    voiture->TotalTime += tempsSection;
    if (voiture->TotalTime >= tempsTotalMax) {
      break;
    }

    //Test de crash et test d'abandon à ajouter ici
    crash_test(voiture)
    

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
void crash_test(Voiture* voiture){
    double usureP ,usureM
    double probaCrash
    double crash
   usureP = (voiture->state.KmParcouruPneu * voiture->state.tauxUsurePneu)/100
   voiture->state.usurePneu =usureP
   usureM = (voiture->state->totalKmParcouru*0.001)
   probaCrash = 100 - usureP +usureM
   crash =randomRange(0 ,100)

   if(crash <= probaCrash){
       voiture->out =1
   }


}