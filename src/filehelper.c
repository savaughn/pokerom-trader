#include "filehelper.h"
#ifdef _WIN32
#else
#include <unistd.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

char *resolvedPath = NULL;
char *absolutePath = NULL;
#ifdef _WIN32
void get_save_files(struct SaveFileData *save_data)
{
}

#else

int get_save_files(struct SaveFileData *save_data)
{
    DIR *dir;
    struct dirent *entry;
    char saveDir[MAX_FILE_PATH_CHAR];
    strcpy(saveDir, (char *)save_data->saveDir);
    int numSaves = 0;

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

int write_key_to_config(char *key, char *value)
{
    // Open config.ini
    FILE *fp;
    char *config_path[MAX_FILE_PATH_CHAR];
    strcpy(config_path, getenv("HOME"));
    strcat(config_path, "/Library/PokeromTrader/config.ini");

    fp = fopen(config_path, "w");
    if (fp == NULL)
    {
        printf("Error opening file! %d\n", errno);
        return 1;
    }

    // if key already exists, overwrite it
    char *key_value = read_key_from_config(key);
    if (key_value != NULL)
    {
        // If there's no change in value
        if (strcmp(key_value, value) == 0)
        {
            printf("Key %s already exists with value %s, not overwriting...\n", key, value);
            fclose(fp);
            return 0;
        }

        // Overwriting with new key value
        printf("Key %s already exists, overwriting...\n", key);
        fputs(key_value, fp);

        fclose(fp);
    }
    else
    {
        fprintf(fp, "%s=%s", key, value);
        fclose(fp);
    }
    free(key_value);
    return 0;
}

void create_config()
{
    FILE *fp;
    char cwd[MAX_FILE_PATH_CHAR];

    // Get current working dir
    strcpy(cwd, getenv("HOME"));

    // create directory PokeromTrader
    char *dir_path = "/Library/PokeromTrader";
    strcat(cwd, dir_path);
    int status = mkdir(cwd, 0777);
    if (status == -1)
    {
        printf("Error %d creating directory!\n", errno);
        if (errno != 17) exit(errno);
    }

    // create saves folder
    char saves_dir[MAX_FILE_PATH_CHAR];
    strcpy(saves_dir, cwd);
    strcat(saves_dir, "/saves");
    status = mkdir(saves_dir, 0777);
    if (status == -1)
    {
        puts("Error creating saves directory");
        if (errno != 17) exit(errno);
    }

    // create config.ini file in cwd
    strcat(cwd, "/config.ini");
    fp = fopen(cwd, "a");

    if (fp == NULL)
    {
        printf("Error %d creating file!\n", errno);
        exit(errno);
    }

    // Write default key value to new config.ini
    char *default_key[MAX_FILE_PATH_CHAR];
    strcpy(default_key, "SAVE_FILE_DIR=");
    strcat(default_key, saves_dir);
    fputs(default_key, fp);

    fclose(fp);
}

char *read_key_from_config(char *key)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *value = NULL;

    char config_path[MAX_FILE_PATH_CHAR];
    strcpy(config_path, getenv("HOME"));
    strcat(config_path, "/Library/PokeromTrader/config.ini");

    fp = fopen(config_path, "r");

    // If missing, then create
    if (fp == NULL)
    {
        fclose(fp);
        create_config();

        // open after creation
        fp = fopen(config_path, "r");
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
