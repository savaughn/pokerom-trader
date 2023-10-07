#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filehelper.h"
#ifdef _WIN32
#else
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>      // For dirname function
#include <mach-o/dyld.h> // For _NSGetExecutablePath function
#endif

char *resolvedPath = NULL;
char *absolutePath = NULL;
#ifdef _WIN32
void get_save_files(struct SaveFileData *save_data)
{
}

#else

char *get_absolute_path(char *path)
{
    char *_absolutePath = NULL;
    char *_executablePath = NULL;
    uint32_t size = 0;
    int result = 0;

    // Get the path of the executable
    result = _NSGetExecutablePath(NULL, &size);
    if (result == -1)
    {
        _executablePath = malloc(size);
        result = _NSGetExecutablePath(_executablePath, &size);
    }

    // Get the directory of the executable
    char *executableDir = dirname(_executablePath);

    // Combine the executable directory and save directory
    char *saveDirPath = malloc(strlen(executableDir) + 1 + strlen(path) + 1);
    sprintf(saveDirPath, "%s/%s", executableDir, path);

    // Get the absolute path
    _absolutePath = realpath(saveDirPath, NULL);
    free(saveDirPath);
    if (_absolutePath)
    {
        return _absolutePath;
    }
    else
    {
        return NULL;
    }
}

// This will only work on macOS
int get_save_files(struct SaveFileData *save_data)
{
    DIR *dir;
    struct dirent *entry;
    char saveDir[100];
    strcpy(saveDir, (char *)save_data->saveDir);
    int numSaves = 0;

    absolutePath = get_absolute_path(saveDir);
    if (absolutePath)
    {
        strcpy(saveDir, absolutePath);
    }

    dir = opendir(saveDir);
    if (dir == NULL)
    {
        perror("Error opening directory: ");
        printf("Path: %s\n", saveDir);
        save_data->numSaves = 0;
        return 1;
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
            if (absolutePath)
            {
                save_data->saves_file_path[numSaves] = absolutePath;
                numSaves++;
            }
        }
    }

    closedir(dir);
    save_data->numSaves = numSaves;
    return 0;
}

void write_key_to_config(char *key, char *value)
{
    FILE *fp;
    char *configPath = get_absolute_path("config.ini");
    printf("Writing %s=%s to %s\n", key, value, configPath);
    fp = fopen(configPath, "a");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    // if key already exists, overwrite it
    char *key_value = read_key_from_config(key);
    printf("key_value: %s\n", key_value);
    if (key_value != NULL)
    {
        if (strcmp(key_value, value) == 0)
        {
            printf("Key %s already exists with value %s, not overwriting...\n", key, value);
            fclose(fp);
            return;
        }
        printf("Key %s already exists, overwriting...\n", key);
        fclose(fp);
        fp = fopen(configPath, "r+");
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        char *new_file_contents = malloc(100);
        new_file_contents[0] = '\0';
        while ((read = getline(&line, &len, fp)) != -1)
        {
            char *token = strtok(line, "=");
            if (strcmp(token, key) == 0)
            {
                char *new_line = malloc(strlen(key) + strlen(value) + 2);
                sprintf(new_line, "%s=%s", key, value);
                strcat(new_file_contents, new_line);
            }
            else
            {
                strcat(new_file_contents, line);
            }
        }
        fclose(fp);
        fp = fopen(configPath, "w");
        fputs(new_file_contents, fp);

        fclose(fp);
        free(new_file_contents);
        free(line);
    }
    else
    {
        fprintf(fp, "%s=%s", key, value);
        fclose(fp);
    }
}

char *read_key_from_config(char *key)
{
    FILE *fp;
    char *configPath = get_absolute_path("config.ini");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *value = NULL;

    printf("Reading %s from %s\n", key, configPath);

    fp = fopen(configPath, "r");
    if (fp == NULL)
    {
        // create the file if it doesn't exist
        char *configPath = get_absolute_path(".");
        strcat(configPath, "/config.ini");
        fp = fopen(configPath, "w");
        if (fp == NULL)
        {
            printf("Error creating file!\n");
            exit(1);
        }
        char *default_save_dir = get_absolute_path(".");
        strcat(default_save_dir, "/saves");
        write_key_to_config("SAVE_FILE_DIR", default_save_dir);
        fclose(fp);
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        // if the line starts with a # or newline, skip it
        if (line[0] == '#' || line[0] == '\n')
        {
            continue;
        }

        char *token = strtok(line, "=");
        if (strcmp(token, key) == 0)
        {
            token = strtok(NULL, "=");
            value = malloc(strlen(token) + 1);
            // remove newline from end of token
            token[strcspn(token, "\n")] = 0;
            strcpy(value, token);
            break;
        }
    }

    fclose(fp);
    if (line)
    {
        free(line);
    }
    return value;
}

#endif

void free_filehelper_pointers(void)
{
    free(resolvedPath);
    free(absolutePath);
}
