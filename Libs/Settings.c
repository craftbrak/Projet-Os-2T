#include "Settings.h"
#include <stdlib.h>
#include <string.h>

/* djb2 hash algorithm, rapide avec peu de collision */
unsigned long hash(unsigned char* str) {
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = hash * 33 ^ c; // alternative du "hash = ((hash << 5) + hash) + c"

	return hash;
}

Node* CreateNode(unsigned long hash, char* key, enum SettingsTypes type, void* value) {
	Node* node = malloc(sizeof(Node));
	if (node == NULL)
		return NULL;

	node->hash = hash;
	node->key = key;
	node->type = type;
	node->value = value;
	node->next = NULL;
	return node;
}

void DestroyNode(Node* node) {
	free(node->key);
	free(node->value);
	free(node);
}

Settings SettingsCreate() {
	Settings settings = malloc(sizeof(Node*) * 256);
	if (settings == NULL)
		return NULL;

	for (int i = 0; i < 256; ++i)
		settings[i] = (Node*) NULL;

	return settings;
}

void SettingsDestroy(Settings settings) {
	for (int i = 0; i < 256; ++i) {
		Node* node = settings[i];
		while (node != NULL) {
			Node* next = node->next;
			free(node);
			node = next;
		}
	}

	free(settings);
}

void SettingsInsert(Settings settings, char* key, enum SettingsTypes type, void* value) {
	unsigned long id = hash((unsigned char*) key);
	Node* node = settings[id % 256];

	if (node == NULL) {
		settings[id % 256] = CreateNode(id, key, type, value);
		return;
	}

	while (node->next != NULL) {
		if (node->hash == id) {
			node->value = value;
			return;
		}
		node = node->next;
	}

	node->next = CreateNode(id, key, type, value);
}

void* SettingsGet(Settings settings, char* key) {
	unsigned long id = hash((unsigned char*) key);
	Node* node = settings[id % 256];

	if (node == NULL)
		return NULL;

	while (node->hash != id && node->next != NULL)
		node = node->next;

	if (node->hash != id)
		return NULL;

	return node->value;
}

bool SettingsHasKey(Settings settings, char* key) {
	return SettingsGet(settings, key) != NULL;
}

char* SettingsGetDefault(Settings settings, char* key, char* default_) {
	char* value = SettingsGet(settings, key);
	return value == NULL ? default_: value;
}

void SettingsRemove(Settings settings, char* key) {
	unsigned long id = hash((unsigned char*) key);
	Node* node = settings[id % 256];
	Node* prev = node;

	if (node == NULL)
		return;

	while (node->hash != id && node->next != NULL) {
		prev = node;
		node = node->next;
	}

	if (node->hash != id)
		return;

	if (prev != node)
		prev->next = node->next;
	else
		settings[id % 256] = NULL;

	DestroyNode(node);
}