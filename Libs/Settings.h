#pragma once
#include <stdbool.h>

enum SettingsTypes {
    Unknown, // Internal purpose
    Number,
    String,
    List
};

typedef struct Node {
	unsigned long hash;
	char* key;
	enum SettingsTypes type;
	void* value;
	struct Node* next;
} Node;

typedef Node** Settings;

/* Crée une nouvelle structure Settings. */
Settings SettingsCreate();
/* Détruit une structure Settings. */
void SettingsDestroy(Settings settings);
/* Insère une clé-valeur dans des Settings. */
void SettingsInsert(Settings settings, char* key, enum SettingsTypes type, void* value);
/* Récupère la valeur d'une clé dans des Settings. Retourne NULL si celle-ci n'a pas été trouvée. */
void* SettingsGet(Settings settings, char* key);
/* Vérifie si la clé existe. */
bool SettingsHasKey(Settings settings, char* key);
/* Récupère la valeur d'une clé dans des Settings. Retourne la valeur par défaut si la clé n'a pas été trouvée. */
char* SettingsGetDefault(Settings settings, char* key, char* default_);
/* Supprime une clé des Settings. */
void SettingsRemove(Settings settings, char* key);