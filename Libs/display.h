#pragma once

void displayEssai(SharedInfo, int [], int, char *, int, int);

void headerEssai(int);

void entryEssai(Voiture [], int [], int, int, int *, int, int);

void displayFinale(SharedInfo, int [], int, char *, int, int, double);

void headerFinale(int);

void entryFinale(Voiture [], int[], int, int, int *, int [], int, double, int, double);

int courseFinie(Voiture [], int [], int);

double calcLongueur(Settings);

int puissance(int, int);

void printTime(int, double, int);
