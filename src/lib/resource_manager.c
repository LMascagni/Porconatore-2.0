#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "resource_manager.h"
#include "file_manager.h"
#include "error_messages.h"

static ResourceString resourceStrings[MAX_STRINGS];
static int resourceStringCount = 0;

static ResourceNumeric resourceNumerics[MAX_NUMERICS];
static int resourceNumericCount = 0;

void TrimEndWhitespace(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = '\0';
}

char *TrimStartWhitespace(char *str) {
    while (isspace((unsigned char)*str)) str++;
    return str;
}

int LoadResourceStrings(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return ERR_RESOURCE_STRING_OPEN;
    }

    char line[MAX_STRING_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Find the first equals sign
        char *equalsSign = strchr(line, '=');

        if (equalsSign) {
            *equalsSign = '\0';
            char *key = line;
            char *value = equalsSign + 1;

            // Trim whitespace
            TrimEndWhitespace(key);
            value = TrimStartWhitespace(value);

            //remove \n from value
            char *newline = strchr(value, '\n');
            if (newline) {
                *newline = '\0';
            }

            strncpy(resourceStrings[resourceStringCount].key, key, MAX_STRING_LENGTH);
            strncpy(resourceStrings[resourceStringCount].value, value, MAX_STRING_LENGTH);
            resourceStringCount++;
        }
    }

    fclose(file);
    return EXIT_SUCCESS;
}

int LoadResourceNumerics(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return ERR_RESOURCE_NUMERIC_OPEN;
    }

    char line[MAX_STRING_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Find the first equals sign
        char *equalsSign = strchr(line, '=');

        if (equalsSign) {
            *equalsSign = '\0';
            char *key = line;
            char *value = equalsSign + 1;

            // Trim whitespace
            TrimEndWhitespace(key);
            TrimStartWhitespace(value);

            strncpy(resourceNumerics[resourceNumericCount].key, key, MAX_STRING_LENGTH);
            resourceNumerics[resourceNumericCount].value = atoi(value);
            resourceNumericCount++;
        }
    }

    fclose(file);
    return EXIT_SUCCESS;
}

int LoadResources(FilePath filePaths[], int filePathCount)
{
    int result;
    const char *stringFilename = GetFilePath("STRING_RESOURCES");
    const char *numericFilename = GetFilePath("NUMERIC_RESOURCES");

    result = LoadResourceStrings(stringFilename);
    if (result != EXIT_SUCCESS) {
        return result;
    }
    
    result = LoadResourceNumerics(numericFilename);
    if (result != EXIT_SUCCESS) {
        return result;
    }
    return EXIT_SUCCESS;
}

const char* GetResourceString(const char *key) {
    for (int i = 0; i < resourceStringCount; i++) {
        if (strcmp(resourceStrings[i].key, key) == 0) {
            return resourceStrings[i].value;
        }
    }
    return NULL;
}

int GetResourceNumeric(const char *key) {
    for (int i = 0; i < resourceNumericCount; i++) {
        if (strcmp(resourceNumerics[i].key, key) == 0) {
            return resourceNumerics[i].value;
        }
    }
    return -1; // Return a default value or handle error appropriately
}

#ifdef DEBUG_RESOURCE_PARSING
void PrintResources() {
    printf("String resources:\n");
    for (int i = 0; i < resourceStringCount; i++) {
        printf("%s = %s\n", resourceStrings[i].key, resourceStrings[i].value);
    }

    printf("\nNumeric resources:\n");
    for (int i = 0; i < resourceNumericCount; i++) {
        printf("%s = %d\n", resourceNumerics[i].key, resourceNumerics[i].value);
    }
}
#endif