#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "filehelper.h"
#include <libgen.h> // For dirname function
#include <mach-o/dyld.h> // For _NSGetExecutablePath function

char *resolvedPath = NULL;
char *absolutePath = NULL;

// This will only work on macOS
void get_save_files(struct SaveFileData *save_data)
{
    DIR *dir;
    struct dirent *entry;
    char *saveDir = save_data->saveDir;
    int numSaves = 0;

    char *executablePath = NULL;
    uint32_t size = 0;
    int result = 0;

    // Get the path of the executable
    result = _NSGetExecutablePath(NULL, &size);
    if (result == -1)
    {
        executablePath = malloc(size);
        result = _NSGetExecutablePath(executablePath, &size);
    }

    // Get the directory of the executable
    char *executableDir = dirname(executablePath);

    // Combine the executable directory and save directory
    char *saveDirPath = malloc(strlen(executableDir) + 1 + strlen(saveDir) + 1);
    sprintf(saveDirPath, "%s/%s", executableDir, saveDir);

    // Get the absolute path
    absolutePath = realpath(saveDirPath, NULL);
    if (absolutePath) {
        saveDir = absolutePath;
    }

    dir = opendir(saveDir);
    if (dir == NULL)
    {
        perror("Error opening directory: ");
        printf("Path: %s\n", saveDir);
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Check if the entry is a regular file with a .sav extension
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".sav"))
        {
            // Combine the base path and file name
            char fullPath[strlen(saveDir) + 1 + strlen(entry->d_name) + 1];
            sprintf(fullPath, "%s/%s", saveDir, entry->d_name);

            // Get the absolute path
            absolutePath = realpath(fullPath, NULL);
            if (absolutePath) {
                save_data->saves_file_path[numSaves] = absolutePath;
                numSaves++;
            }
        }
    }

    closedir(dir);
    save_data->numSaves = numSaves;
    free(saveDirPath);
}

void free_filehelper_pointers(void) {
    free(resolvedPath);
    free(absolutePath);
}
