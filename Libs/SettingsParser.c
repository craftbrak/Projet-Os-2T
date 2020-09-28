﻿#include "SettingsParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

typedef struct Buffer {
	char* data;
	char* ptr;
	int size;
} Buffer;

bool bufend(Buffer* buf) {
	return (buf->ptr - buf->data) >= buf->size;
}

/* Lis entièrement un fichier en format binaire.
   Prend en argument le nom du fichier.
   Retourne un buffer avec le contenu du fichier. */
Buffer* readfile(char* filename) {
	FILE* fp;
	Buffer* buffer = malloc(sizeof(Buffer));
	if (buffer == NULL)
		return NULL;

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		seterror("Cannot open the file \"%s\".", filename);
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	buffer->data = malloc(buffer->size = ftell(fp));
	fseek(fp, 0, SEEK_SET);

	if (buffer->data == NULL)
		return NULL;

	fread(buffer->data, buffer->size, 1, fp);
	buffer->ptr = buffer->data;
	fclose(fp);
	return buffer;
}

int is_whitespace(char c) {
	// \t \n \v \f \r ' '
	return (c > 0x8 && c < 0xe) || c == 0x20;
}

void skip_whitespace(Buffer* buffer) {
	while (!bufend(buffer) && is_whitespace(*buffer->ptr)) ++buffer->ptr;
}
void skip_until_whitespace(Buffer* buffer) {
	while (!bufend(buffer) && !is_whitespace(*buffer->ptr)) ++buffer->ptr;
}
void skip_space(Buffer* buffer) {
	while (!bufend(buffer) && *buffer->ptr == ' ') ++buffer->ptr;
}
void skip_until_eol(Buffer* buffer) {
	while (!bufend(buffer) && *buffer->ptr != '\n' && *buffer->ptr != '\r') ++buffer->ptr;
}

char* parse_key(Buffer* buffer) {
	skip_whitespace(buffer);
	char* begin = buffer->ptr;
	skip_until_whitespace(buffer);
	int size = buffer->ptr - begin;

	if (bufend(buffer))
		return NULL;

	char* key = malloc((size_t) size + 1);
	if (key == NULL)
		return NULL;

	memcpy(key, begin, size);
	key[size] = '\0';
	return key;
}

bool parse_equal(Buffer* buffer) {
	skip_whitespace(buffer);

	if (bufend(buffer)) {
		seterror("Expected '=' at %d but reached end of file.", buffer->ptr - buffer->data);
		return false;
	}
	if (*buffer->ptr != '=') {
		seterror("Expected '=' at %d, got '%c' instead.", buffer->ptr - buffer->data, *buffer->ptr);
		return false;
	}
	++buffer->ptr;
	return true;
}

char* parse_value(Buffer* buffer) {
	skip_whitespace(buffer);
	char* begin = buffer->ptr;
	skip_until_eol(buffer);
	int size = buffer->ptr - begin;

	if (bufend(buffer)) {
		seterror("Expected value at %d but reached end of file.", buffer->ptr - buffer->data);
		return NULL;
	}

	char* value = malloc((size_t) size + 1);
	if (value == NULL)
		return NULL;

	memcpy(value, begin, size);
	value[size] = '\0';
	return value;
}

/* Supprime tous les espaces à droite du string. Ne modifie pas la taille. */
void strip_spaces(char* str) {
	int size = strlen(str);
	while (str[size - 1] == ' ') {
		--size;
		str[size] = '\0';
	}
}

static char* SETTINGS_ERROR = NULL;

void seterror(char* fmt, ...) {
	if (SETTINGS_ERROR != NULL)
		free(SETTINGS_ERROR);

	// C'est pas super, mais suffisant ?
	SETTINGS_ERROR = malloc(512);

	va_list args;
	va_start(args, fmt);
	vsnprintf(SETTINGS_ERROR, 512, fmt, args);
	va_end(args);
}

char* geterror() {
	return SETTINGS_ERROR;
}

Settings parse_config(char* filename) {
	int size, pos = 0;
	Buffer* buffer = readfile(filename);
	if (buffer == NULL)
		return NULL;

	Settings settings = SettingsCreate();
	while (!bufend(buffer)) {
		char* key = parse_key(buffer);
		if (key == NULL)
			break;

		if (!parse_equal(buffer))
			return NULL;

		char* value = parse_value(buffer);
		if (value == NULL)
			return NULL;

		strip_spaces(value);
		SettingsInsert(settings, key, value);
	}

	return settings;
}
