#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "file_manager.h"

#define MAX_STRING_LENGTH 256
#define MAX_STRINGS 100
#define MAX_NUMERICS 100

typedef struct {
    char key[MAX_STRING_LENGTH];
    char value[MAX_STRING_LENGTH];
} ResourceString;

typedef struct {
    char key[MAX_STRING_LENGTH];
    int value;
} ResourceNumeric;

void LoadResources(FilePath filePaths[], int filePathCount);
const char* GetResourceString(const char *key);
int GetResourceNumeric(const char *key);
void PrintResources();

#endif // RESOURCE_MANAGER_H