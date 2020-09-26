#ifndef QTE_SECTIONS
#define QTE_SECTIONS 3
#endif

#ifndef VOITURE_H
#define VOITURE_H

typedef struct state {
    double KmParcouruPneu;
    double usurePneu;
    double totalKmParcouru;
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

#endif
