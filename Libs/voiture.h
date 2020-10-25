#pragma once


typedef struct state {
    double KmParcouruPneu;
    double usurePneu;
    double totalKmParcouru;
} State;

typedef struct voiture {
    char *nomVoiture;
    double sections[32];
    double bestLap;
    double TotalTime;
    double speed;
    int pit, out, done;
    double pitTime;
    int qteSections;
    State state;
} Voiture;
