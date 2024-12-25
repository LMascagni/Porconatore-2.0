#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_manager.h"

void LoadFilePaths(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (!file)
    {
        fprintf(stderr, "Errore nell'apertura del file dei percorsi\n");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "\n");

        if (key && value)
        {
            strcpy(filePaths[filePathCount].key, key);
            strcpy(filePaths[filePathCount].value, value);
            filePathCount++;
        }
    }

    fclose(file);
}

const char* GetFilePath(const char *key)
{
    for (int i = 0; i < filePathCount; i++)
    {
        if (strcmp(filePaths[i].key, key) == 0)
        {
            return filePaths[i].value;
        }
    }
    return NULL;
}