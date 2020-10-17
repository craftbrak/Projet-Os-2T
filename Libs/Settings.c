#include "Settings.h"
#include <stdlib.h>
#include <string.h>

/* djb2 hash algorithm, rapide avec peu de collision */
unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = hash * 33 ^ c; // alternative du "hash = ((hash << 5) + hash) + c"

    return hash;
}

Node *CreateNode(unsigned long hash, char *key, enum EnumTypes type, void *value) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL)
        return NULL;

    node->hash = hash;
    node->key = key;
    node->type = type;
    node->value = value;
    node->next = NULL;
    return node;
}

void DestroyNode(Node *node) {
    if (node->type == NumberArray)
        NbrVectorDestroy(*(NbrVector *) node->value);
    else if (node->type == StringArray)
        StrVectorDestroy(*(StrVector *) node->value);
    free(node->key);
    free(node->value);
    free(node);
}

Settings SettingsCreate() {
    Settings settings = malloc(sizeof(Node *) * 256);
    if (settings == NULL)
        return NULL;

    for (int i = 0; i < 256; ++i)
        settings[i] = (Node *) NULL;

    return settings;
}

void SettingsDestroy(Settings settings) {
    for (int i = 0; i < 256; ++i) {
        Node *node = settings[i];
        while (node != NULL) {
            Node *next = node->next;
            DestroyNode(node);
            node = next;
        }
    }

    free(settings);
}

void SettingsInsert(Settings settings, char *key, enum EnumTypes type, void *value) {
    unsigned long id = hash((unsigned char *) key);
    Node *node = settings[id % 256];

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

enum EnumTypes SettingsGetType(Settings settings, char *key) {
    unsigned long id = hash((unsigned char *) key);
    Node *node = settings[id % 256];

    if (node == NULL)
        return Unknown;

    while (node->hash != id && node->next != NULL)
        node = node->next;

    if (node->hash != id)
        return Unknown;

    return node->type;
}

void *SettingsGet(Settings settings, char *key) {
    unsigned long id = hash((unsigned char *) key);
    Node *node = settings[id % 256];

    if (node == NULL)
        return NULL;

    while (node->hash != id && node->next != NULL)
        node = node->next;

    if (node->hash != id)
        return NULL;

    return node->value;
}

bool SettingsHasKey(Settings settings, char *key) {
    return SettingsGet(settings, key) != NULL;
}

char *SettingsGetDefault(Settings settings, char *key, char *default_) {
    char *value = SettingsGet(settings, key);
    return value == NULL ? default_ : value;
}

void SettingsRemove(Settings settings, char *key) {
    unsigned long id = hash((unsigned char *) key);
    Node *node = settings[id % 256];
    Node *prev = node;

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

NbrVector NbrVectorCreate() {
    NbrVector vec;
    vec.length = 0;
    vec.max_size = 16;
    vec.data = malloc(sizeof(double) * vec.max_size);
    return vec;
}

void NbrVectorDestroy(NbrVector vec) {
    free(vec.data);
}

void NbrVectorAppend(NbrVector *vec, double value) {
    // resize the vector if we don't have enough place.
    if (vec->length == vec->max_size) {
        vec->max_size *= 2; // double the size (non-linear allocation)
        vec->data = realloc(vec->data, vec->max_size); // resize the buffer with this new size
    }

    // Add the value to the vector ...
    vec->data[vec->length] = value;
    ++vec->length; // ... and increase the vector size
}

double NbrVectorPop(NbrVector *vec) {
    if (vec->length > 0)
        return vec->data[--vec->length];
    return 0x41414141; // debug value
}


StrVector StrVectorCreate() {
    StrVector vec;
    vec.length = 0;
    vec.max_size = 16;
    vec.data = malloc(sizeof(char *) * vec.max_size);
    return vec;
}

void StrVectorDestroy(StrVector vec) {
    for (int i=0; i<vec.length; i++) {
        free(vec.data[i]);
    }
    free(vec.data);
}

void StrVectorAppend(StrVector *vec, char *value) {
    // resize the vector if we don't have enough place.
    if (vec->length == vec->max_size) {
        vec->max_size *= 2; // double the size (non-linear allocation)
        vec->data = realloc(vec->data, vec->max_size); // resize the buffer with this new size
    }

    // Add the value to the vector ...
    vec->data[vec->length] = value;
    ++vec->length; // ... and increase the vector size
}

char *StrVectorPop(StrVector *vec) {
    if (vec->length > 0){
        free(vec->data[vec->length]);
        return vec->data[--vec->length];
    }
    return NULL; // debug value
}

