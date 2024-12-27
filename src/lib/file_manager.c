#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_manager.h"
#include "error_messages.h"

FilePath filePaths[MAX_FILE_PATHS];
int filePathCount = 0;

int LoadFilePaths(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (!file)
    {
        return ERR_FILE_PATH_OPEN;
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
    return EXIT_SUCCESS;
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

#ifdef DEBUG_FILE_PATHS
void printFilePaths()
{
    for (int i = 0; i < filePathCount; i++)
    {
        printf("Key: %s, Value: %s\n", filePaths[i].key, filePaths[i].value);
    }
}
#endif