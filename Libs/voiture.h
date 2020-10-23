#pragma once

#include "../config.h"

typedef struct state {
    double KmParcouruPneu;
    double usurePneu;
    double totalKmParcouru;
    double tauxUsurePneu = 0.5
} State;

typedef struct voiture {
    char* nomVoiture;
    double sections[QTE_SECTIONS];
    double GAP;
    double TotalTime;
    int pit,out;
    int pitCounteur;
    State state;
} Voiture;
