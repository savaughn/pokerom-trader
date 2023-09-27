#include "filehelper.h"

char *resolvedPath = NULL;
char *absolutePath = NULL;

void getAbsolutePath(const char* basePath, const char* fileName) {
    // Combine the base path and file name
    char fullPath[strlen(basePath) + 1 + strlen(fileName) + 1];
    sprintf(fullPath, "%s/%s", basePath, fileName);

    // Get the absolute path
    resolvedPath = realpath(fullPath, NULL);
    if (resolvedPath) {
        // Allocate memory for the absolute path
        absolutePath = (char*)malloc(strlen(resolvedPath) + 1);
        if (absolutePath) {
            strcpy(absolutePath, resolvedPath);
        }
    }
}
void get_save_files(struct SaveFileData *save_data)
{
    DIR *dir;
    struct dirent *entry;
    const char *saveDir = save_data->saveDir;
    char **saves_file_path = save_data->saves_file_path;
    int *numSaves = &(save_data->numSaves);

    dir = opendir(saveDir);
    if (dir == NULL)
    {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Check if the entry is a regular file with a .sav extension
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".sav"))
        {
            getAbsolutePath(saveDir, entry->d_name);
            if (absolutePath) {
                saves_file_path[*numSaves] = absolutePath;
                (*numSaves)++;
            }
        }
    }

    closedir(dir);
}

void free_filehelper_pointers(void) {
    free(resolvedPath);
    free(absolutePath);
}
