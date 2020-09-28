#pragma once
#include <stdbool.h>

typedef struct Node {
	unsigned long hash;
	char* key;
	char* value;
	struct Node* next;
} Node;

typedef Node** Settings;

/* Cr�e une nouvelle structure Settings. */
Settings SettingsCreate();
/* D�truit une structure Settings. */
void SettingsDestroy(Settings settings);
/* Ins�re une cl�-valeur dans des Settings. */
void SettingsInsert(Settings settings, char* key, char* value);
/* R�cup�re la valeur d'une cl� dans des Settings. Retourne NULL si celle-ci n'a pas �t� trouv�e. */
char* SettingsGet(Settings settings, char* key);
/* V�rifie si la cl� existe. */
bool SettingsHasKey(Settings settings, char* key);
/* R�cup�re la valeur d'une cl� dans des Settings. Retourne la valeur par d�faut si la cl� n'a pas �t� trouv�e. */
char* SettingsGetDefault(Settings settings, char* key, char* default_);
/* Supprime une cl� des Settings. */
void SettingsRemove(Settings settings, char* key);