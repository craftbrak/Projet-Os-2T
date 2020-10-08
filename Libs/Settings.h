#pragma once

#include <stdlib.h>
#include <stdbool.h>

enum EnumTypes {
    Unknown, // Internal purpose
    Number,
    String,
    NumberArray,
    StringArray
};

typedef struct Node {
    unsigned long hash;
    char *key;
    enum EnumTypes type;
    void *value;
    struct Node *next;
} Node;

typedef Node **Settings;

typedef struct NbrVector {
    size_t max_size; // size allocated to `data`
    size_t length; // elements inside the array `data`
    double *data;
} NbrVector;
typedef struct StrVector {
    size_t max_size; // size allocated to `data`
    size_t length; // elements inside the array `data`
    char **data;
} StrVector;

/* Crée une nouvelle structure Settings. */
Settings SettingsCreate();

/* Détruit une structure Settings. */
void SettingsDestroy(Settings settings);

/* Insère une clé-valeur dans des Settings. */
void SettingsInsert(Settings settings, char *key, enum EnumTypes type, void *value);

/* Récupère la valeur d'une clé dans des Settings. Retourne NULL si celle-ci n'a pas été trouvée. */
void *SettingsGet(Settings settings, char *key);

/* Vérifie si la clé existe. */
bool SettingsHasKey(Settings settings, char *key);

/* Récupère la valeur d'une clé dans des Settings. Retourne la valeur par défaut si la clé n'a pas été trouvée. */
char *SettingsGetDefault(Settings settings, char *key, char *default_);

/* Supprime une clé des Settings. */
void SettingsRemove(Settings settings, char *key);

/* Crée et initialise un nouveau vector de nombres */
NbrVector NbrVectorCreate();

/* Ajoute à la fin du vector une valeur */
void NbrVectorAppend(NbrVector vec, double value);

/* Enlève et retourne une valeur du vecteur */
double NbrVectorPop(NbrVector vec);
