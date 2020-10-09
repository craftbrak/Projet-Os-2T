#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "Libs/SettingsParser.h"

#define assert(expr) \
    if (!(expr)) { \
        fprintf(stderr, \
            "Assertation error: \"%s\" returned %d at line %d\n", \
            #expr,\
            (int) (expr), \
            __LINE__ \
        ); \
        exit(0); \
    }

#define EPSILON 0.000001

void main(int argc, char **argv) {
    char *filename = "config.txt";
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0) {
            printf("Usage: %s [config.txt]\n", argv[0]);
            return;
        } else if (access(argv[1], F_OK) == -1) {
            fprintf(stderr, "Cannot open file %s. Please provide an existing configuration file.\n", argv[1]);
            return;
        } else {
            filename = argv[1];
        }
    }
    Settings settings = parse_config(filename);

    if (settings == NULL) {
        printf("Error: %s\n", geterror());
    }

    assert(settings != NULL)
    assert(SettingsGetType(settings, "test") == String)
    assert(SettingsGetType(settings, "number") == Number)
    assert(SettingsGetType(settings, "negative") == Number)
    assert(SettingsGetType(settings, "decimal") == Number)
    assert(SettingsGetType(settings, "nbrlist") == NumberArray)
    assert(SettingsGetType(settings, "strlist") == StringArray)

    assert(strcmp(SettingsGet(settings, "test"), "Hi there") == 0)
    assert(fabs(*(double *) SettingsGet(settings, "number") - 42.0) < EPSILON)
    assert(fabs(*(double *) SettingsGet(settings, "negative") - -53.0) < EPSILON)
    assert(fabs(*(double *) SettingsGet(settings, "decimal") - 666.666) < EPSILON)

    NbrVector *nvec = SettingsGet(settings, "nbrlist");
    assert(nvec != NULL)
    assert(nvec->length == 3)
    assert(fabs(nvec->data[0] - 1.0) < EPSILON)
    assert(fabs(nvec->data[1] - -2.5) < EPSILON)
    assert(fabs(nvec->data[2] - 3.9) < EPSILON)

    StrVector *svec = SettingsGet(settings, "strlist");
    assert(svec != NULL)
    assert(svec->length == 5)
    assert(strcmp(svec->data[0], "where") == 0)
    assert(strcmp(svec->data[1], "is") == 0)
    assert(strcmp(svec->data[2], "the") == 0)
    assert(strcmp(svec->data[3], "H") == 0)
    assert(strcmp(svec->data[4], "?") == 0)

    SettingsDestroy(settings);

    printf("All tests passed.");
    exit(0);
}