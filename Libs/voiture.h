#pragma once


typedef struct state {
    double KmParcouruPneu;
    double usurePneu;
    double totalKmParcouru;
} State;

typedef struct voiture {
    char *nomVoiture;
    double *sections;
    double bestLap;
    double GAP;
    double TotalTime;
    double speed;
    int pit, out;
    double pitTime;
    int qteSections;
    State state;
} Voiture;
