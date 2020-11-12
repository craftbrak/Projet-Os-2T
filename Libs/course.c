#include <unistd.h>
#include <stdio.h>
#include "Settings.h"
#include "voiture.h"
#include "sharedmem.h"
#include "randomLib.h"
#include "course.h"

// Essais et qualifs
void essai(SharedInfo shared, int index, int tempsTotalMax, Settings settings) {
    Voiture voiture;
    double vitesseMoyenne = *((double *) SettingsGet(settings, "vitesse_moyenne"));
    NbrVector *longueurs = SettingsGet(settings, "longueur_sections");
    double *longueurSections = longueurs->data;
    int qte_sections = longueurs->length;
    int delay = (int) *((double *) SettingsGet(settings, "delay"));
    int sectionActuelle = 0;
    double timeToAdd;
    double lapTime = 0.0;
    double tempsSection;

    if (!(getVoitureCopy(index, &voiture, shared))) {
        return;
    }

    while (1) {
        tempsSection = tempsRandom(longueurSections[sectionActuelle], vitesseMoyenne);
        voiture.state.KmParcouruPneu+=longueurSections[sectionActuelle];
        voiture.state.totalKmParcouru+=longueurSections[sectionActuelle];
        validerTempsSection(&voiture, sectionActuelle, tempsSection);

        lapTime += tempsSection;
        voiture.TotalTime += tempsSection;
        if (voiture.TotalTime >= tempsTotalMax) {
            break;
        }

        //Test de crash et test d'abandon à ajouter ici
        if(crash_test(&voiture, settings)){
            break;
        }
        sectionActuelle++;
        if (sectionActuelle == qte_sections) {
            sectionActuelle = 0;
            if (voiture.bestLap < 0 || lapTime < voiture.bestLap) {
                voiture.bestLap = lapTime;
            }
            if((timeToAdd=go_to_pit(&voiture, settings))){
                voiture.TotalTime +=timeToAdd;
                if (!(setVoiture(index, &voiture, shared))) {
                    break;
                }
                sleep(delay);
                voiture.pit=0;
                if (!(setVoiture(index, &voiture, shared))) {
                    break;
                }
            }
            lapTime = 0;
            sleep(delay);
        }

        if (!(setVoiture(index, &voiture, shared))) {
            break;
        }
    }

    voiture.done = 1;
    setVoiture(index, &voiture, shared);
}

// Course finale
void finale(SharedInfo shared, int index, int maxSections, Settings settings) {
    Voiture voiture;
    double vitesseMoyenne = *((double *) SettingsGet(settings, "vitesse_moyenne"));
    NbrVector *longueurs = SettingsGet(settings, "longueur_sections");
    double *longueurSections = longueurs->data;
    int qte_sections = longueurs->length;
    int sectionActuelle = 0;
    double timeToAdd=0;
    int delay = (int) *((double *) SettingsGet(settings, "delay"));
    double distance = 0;
    double lapTime = 0.0;
    double tempsSection;

    if (!(getVoitureCopy(index, &voiture, shared))) {
        return;
    }

    while (voiture.qteSections < maxSections) {
        tempsSection = tempsRandom(longueurSections[sectionActuelle], vitesseMoyenne);
        voiture.state.KmParcouruPneu+=longueurSections[sectionActuelle];
        voiture.state.totalKmParcouru+=longueurSections[sectionActuelle];
        validerTempsSection(&voiture, sectionActuelle, tempsSection);

        lapTime += tempsSection;
        distance += longueurSections[sectionActuelle];
        voiture.TotalTime += tempsSection;
        voiture.speed = distance / voiture.TotalTime;

        //Test crash ici.
        if(crash_test(&voiture, settings)){
            break;
        }

        sectionActuelle++;
        voiture.qteSections++;
        if (sectionActuelle == qte_sections) {
            sectionActuelle = 0;
            if (voiture.bestLap < 0 || lapTime < voiture.bestLap) {
                voiture.bestLap = lapTime;
            }
            if((timeToAdd=go_to_pit(&voiture, settings))){
                voiture.TotalTime +=timeToAdd;
                if (!(setVoiture(index, &voiture, shared))) {
                    break;
                }
                sleep(delay);
                voiture.pit=0;
                if (!(setVoiture(index, &voiture, shared))) {
                    break;
                }
            }
            lapTime = 0;
            sleep(delay);
        }

        if (!(setVoiture(index, &voiture, shared))) {
            break;
        }
    }

    voiture.done = 1;
    setVoiture(index, &voiture, shared);
}

void validerTempsSection(Voiture *voiture, int sectionActuelle, double temps) {
    if (voiture->sections[sectionActuelle] < 0 || temps < voiture->sections[sectionActuelle]) {
        voiture->sections[sectionActuelle] = temps;
        //printf("%s | S%i : %lf s \n", voiture->nomVoiture ,sectionActuelle + 1, temps);
    }
}

double tempsRandom(double longueur, double vitesseMoyenne) {
    double tempsMoyen = (longueur * 3600) / vitesseMoyenne;
    double tempsDelta = tempsMoyen / 5;

    return randomRange(tempsMoyen - tempsDelta, tempsMoyen + tempsDelta);
}

void resetVoiture(Voiture *voiture, int qte_sections) {
    for (int i = 0; i < qte_sections; i++) {
        voiture->sections[i] = -1;
    }
    voiture->speed = 0;
    voiture->qteSections = 0;
    voiture->bestLap = -1;
    voiture->TotalTime = 0.0;
    voiture->pit = 0;
    voiture->state.usurePneu=0;
    voiture->state.KmParcouruPneu=0;
    voiture->out = 0;
    voiture->done = 0;
}
int crash_test(Voiture* voiture, Settings settings){
    double tauxUsurepneu = *(double *) SettingsGet(settings ,"taux_usure_pneu");
    double usureP ;
    double usureM;
    double probaCrash;
    double crash;
    usureP = (voiture->state.KmParcouruPneu * tauxUsurepneu)/100;
    voiture->state.usurePneu =usureP;

    usureM = (voiture->state.totalKmParcouru*0.001);
    probaCrash =usureP +usureM;
    crash =randomRange(0 ,100);

    if(crash <= probaCrash){
        voiture->state.totalKmParcouru=0;
        return 1;
    }
    return 0;
}
double go_to_pit(Voiture* voiture, Settings settings){
    double minPit_time =*(double *) SettingsGet(settings,"min_pit_time");
    double maxPit_time =*(double *) SettingsGet(settings,"max_pit_time");
    if(voiture->state.usurePneu>50){
        voiture->state.usurePneu=0;
        voiture->state.KmParcouruPneu=0;
        voiture->pit=1;
        voiture->pitTime= randomRange(minPit_time,maxPit_time);
        return voiture->pitTime;
    }
}