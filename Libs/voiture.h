#pragma once

#include "../config.h"

typedef struct state {
    double KmParcouruPneu;
    double usurePneu;
    double totalKmParcouru;
} State;

typedef struct voiture {
    char* nomVoiture;
    double sections[QTE_SECTIONS];
    double bestLap;
    double GAP;
    double TotalTime;
    int pit,out;
    int pitTime;
    State state;
} Voiture;
