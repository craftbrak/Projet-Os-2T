#include "SettingsParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

typedef struct Buffer {
    char *data;
    char *ptr;
    int size;
} Buffer;

bool bufend(Buffer *buf) {
    return (buf->ptr - buf->data) >= buf->size;
}

/* Lis entièrement un fichier en format binaire.
   Prend en argument le nom du fichier.
   Retourne un buffer avec le contenu du fichier. */
Buffer *readfile(char *filename) {
    FILE *fp;
    Buffer *buffer = malloc(sizeof(Buffer));
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

void skip_until_eol(Buffer *buffer) {
    while (!bufend(buffer) && *buffer->ptr != '\n' && *buffer->ptr != '\r') ++buffer->ptr;
}

void skip_whitespace(Buffer *buffer) {
    do {
        while (!bufend(buffer) && is_whitespace(*buffer->ptr)) ++buffer->ptr;
        if (*buffer->ptr == '#')
            skip_until_eol(buffer);
    } while (is_whitespace(*buffer->ptr));
}

void skip_until_whitespace(Buffer *buffer) {
    while (!bufend(buffer) && !is_whitespace(*buffer->ptr)) ++buffer->ptr;
}

void skip_until_char(Buffer *buffer, char c) {
    while (!bufend(buffer) && *buffer->ptr != c) ++buffer->ptr;
}

void skip_space(Buffer *buffer) {
    while (!bufend(buffer) && *buffer->ptr == ' ') ++buffer->ptr;
}

char *parse_key(Buffer *buffer) {
    skip_whitespace(buffer);
    char *begin = buffer->ptr;
    skip_until_whitespace(buffer);
    int size = buffer->ptr - begin;

    if (bufend(buffer))
        return NULL;

    char *key = malloc((size_t) size + 1);
    if (key == NULL)
        return NULL;

    memcpy(key, begin, size);
    key[size] = '\0';
    return key;
}

bool parse_equal(Buffer *buffer) {
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

bool parse_number(Buffer *buffer, double *value) {
    char *begin = buffer->ptr;
    double sign = 1.0;
    double float_idx = 0.0;

    if (buffer->ptr[0] == '-') {
        sign = -1.0;
        ++buffer->ptr;
    }

    while (!bufend(buffer)) {
        switch (buffer->ptr[0]) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                *value = *value * 10 + (double) (buffer->ptr[0] - '0');
                if (float_idx > 0)
                    float_idx *= 10.0;
                break;
            case '.':
                float_idx = 1.0;
                break;
            default:
                *value *= sign;
                if (float_idx > 0)
                    *value /= float_idx;

                return buffer->ptr != begin;
        }
        ++buffer->ptr;
    }
    return false;
}

char *parse_string(Buffer *buffer) {
    char *begin = ++buffer->ptr;
    skip_until_char(buffer, '"');
    int size = buffer->ptr - begin;

    if (bufend(buffer)) {
        seterror("Expected \" at %d but reached end of file.", buffer->ptr - buffer->data);
        return NULL;
    }
    ++buffer->ptr; // Skip "

    char *value = malloc((size_t) size);
    memcpy(value, begin, size);
    value[size] = '\0';
    return value;
}

void *parse_value(Buffer *buffer, enum EnumTypes *type) {
    skip_whitespace(buffer);
    char *begin = buffer->ptr;

    switch (begin[0]) {
        case '"': {
            *type = String;
            return parse_string(buffer);
        }
        case '[': {
            skip_whitespace(buffer);

            if (buffer->ptr[1] == '"') {
                *type = StringArray;
                StrVector *vec = malloc(sizeof(StrVector));
                *vec = StrVectorCreate();
                char *start;

                do {
                    ++buffer->ptr;
                    skip_whitespace(buffer);
                    if (buffer->ptr[0] != '"') {
                        StrVectorDestroy(*vec);
                        free(vec);
                        seterror("Expected '\"' got '%c' instead.", buffer->ptr[0]);
                        return NULL;
                    }
                    char *str = parse_string(buffer);
                    if (str == NULL) {
                        StrVectorDestroy(*vec);
                        free(vec);
                        return NULL;
                    }

                    StrVectorAppend(vec, str);
                    skip_whitespace(buffer);
                } while (!bufend(buffer) && buffer->ptr[0] == ',');

                if (buffer->ptr[0] != ']') {
                    StrVectorDestroy(*vec);
                    free(vec);
                    seterror("List does not have a matching closing bracket.");
                    return NULL;
                }
                ++buffer->ptr;
                return vec;
            } else {
                *type = NumberArray;
                NbrVector *vec = malloc(sizeof(NbrVector));
                *vec = NbrVectorCreate();
                double value;

                do {
                    value = 0;
                    ++buffer->ptr;
                    skip_whitespace(buffer);
                    if (!parse_number(buffer, &value)) {
                        NbrVectorDestroy(*vec);
                        free(vec);
                        seterror("Not a valid number at %d.", begin - buffer->data);
                        return NULL;
                    }
                    NbrVectorAppend(vec, value);
                    skip_whitespace(buffer);
                } while (!bufend(buffer) && buffer->ptr[0] == ',');

                if (buffer->ptr[0] != ']') {
                    NbrVectorDestroy(*vec);
                    free(vec);
                    seterror("List does not have a matching closing bracket.");
                    return NULL;
                }
                ++buffer->ptr;
                return vec;
            }
        }
        default: {
            // *type = guesstype(begin, buffer->ptr);
            *type = Number;
            double *value = malloc(sizeof(double));
            *value = 0;
            if (!parse_number(buffer, value)) {
                skip_until_eol(buffer);
                size_t length = buffer->ptr - begin;

                char *string = malloc(length);
                memcpy(string, begin, length);
                string[length] = '\0';

                seterror("'%s' is not a valid number.", string);
                free(value);
                free(string);
                return NULL;
            }

            return value;
        }
    }
}

/* Supprime tous les espaces à droite du string. Ne modifie pas la taille. */
void strip_spaces(char *str, int *size) {
    while (str[*size - 1] == ' ')
        str[--*size] = '\0';
}

static char *SETTINGS_ERROR = NULL;

void seterror(char *fmt, ...) {
    if (SETTINGS_ERROR != NULL)
        free(SETTINGS_ERROR);

    // C'est pas super, mais suffisant ?
    SETTINGS_ERROR = malloc(512);

    va_list args;
    va_start(args, fmt);
    vsnprintf(SETTINGS_ERROR, 512, fmt, args);
    va_end(args);
}

char *geterror() {
    return SETTINGS_ERROR;
}

Settings parse_config(char *filename) {
    int size, pos = 0;
    Buffer *buffer = readfile(filename);
    if (buffer == NULL)
        return NULL;

    Settings settings = SettingsCreate();
    while (!bufend(buffer)) {
        char *key = parse_key(buffer);
        if (key == NULL)
            break;

        if (!parse_equal(buffer))
            return NULL;

        enum EnumTypes type;
        void *value = parse_value(buffer, &type);
        if (value == NULL || type == Unknown)
            return NULL;

        SettingsInsert(settings, key, type, value);
    }

    return settings;
}
