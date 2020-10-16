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
    double speed;
    int pit,out;
    double pitTime;
    int qteSections;
    State state;
} Voiture;
