#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#define MAX_PATH_LENGTH 260
#define MAX_FILE_PATHS 100

typedef struct {
    char key[256];
    char value[MAX_PATH_LENGTH];
} FilePath;

static FilePath filePaths[MAX_FILE_PATHS];
static int filePathCount = 0;

void LoadFilePaths(const char *filePath);
const char* GetFilePath(const char *key);

#endif // FILE_MANAGER_H