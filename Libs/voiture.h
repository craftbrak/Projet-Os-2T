typedef struct Voiture {
    int numVoiture;
    int nomVoiture;
    double S1,S2,S3;
    double GAP;
    double TotalTime;
    int pit,out;
    int pitCounteur;
    struct state{
        int KmParcouruPneu;
        double usurePneu;
        int totalKmParcouru;
    };

} Voiture;