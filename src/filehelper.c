#include "filehelper.h"
#ifdef _WIN32
#else
#include <unistd.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

char *resolved_path = NULL;
char *absolute_path = NULL;
#ifdef _WIN32
void get_save_files(struct SaveFileData *save_data)
{
}

#else

int get_save_files(struct SaveFileData *save_data)
{
    DIR *dir;
    struct dirent *entry;
    char save_dir[MAX_FILE_PATH_CHAR];
    strcpy(save_dir, (char *)save_data->save_dir);
    int num_saves = 0;

    dir = opendir(save_dir);
    if (dir == NULL)
    {
        perror("Error opening directory: ");
        printf("Path: %s\n", save_dir);
        save_data->num_saves = 0;
        return 1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Check if the entry is a regular file with a .sav extension
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".sav"))
        {
            // Combine the base path and file name
            char full_path[strlen(save_dir) + 1 + strlen(entry->d_name) + 1];
            sprintf(full_path, "%s/%s", save_dir, entry->d_name);

            // Get the absolute path
            absolute_path = realpath(full_path, NULL);
            if (absolute_path)
            {
                save_data->saves_file_path[num_saves] = absolute_path;
                num_saves++;
            }
        }
    }

    closedir(dir);
    save_data->num_saves = num_saves;
    return 0;
}

int write_key_to_config(const char *key, const char *value)
{
    // Get config.ini path
    char config_path[MAX_FILE_PATH_CHAR];
    strcpy(config_path, getenv("HOME"));
    strcat(config_path, "/Library/PokeromTrader/config.ini");

    // Open the INI file for reading and writing.
    FILE *file = fopen(config_path, "r+");
    if (file == NULL) {
        perror("Error opening INI file");
        return 1;
    }

    // Create a temporary file to store the modified contents.
    FILE *temp_file = tmpfile();
    if (temp_file == NULL) {
        perror("Error creating temporary file");
        fclose(file);
        return 1;
    }

    char line[1024];
    int key_found = 0;

    // Read the INI file line by line.
    while (fgets(line, sizeof(line), file) != NULL) {
        // Check if the line starts with the desired key.
        if (strncmp(line, key, strlen(key)) == 0) {
            // Key found, update the value.
            fprintf(temp_file, "%s=%s\n", key, value);
            key_found = 1;
        } else {
            // Copy the line as-is to the temporary file.
            fprintf(temp_file, "%s", line);
        }
    }

    // If the key was not found, add it to the end of the file.
    if (!key_found) {
        fprintf(temp_file, "%s=%s\n", key, value);
    }

    // Close both files.
    fclose(file);
    rewind(temp_file);

    // Reopen the INI file for writing and copy the contents from the temporary file.
    file = fopen(config_path, "w");
    if (file == NULL) {
        perror("Error reopening INI file");
        fclose(temp_file);
        return 1;
    }

    while (fgets(line, sizeof(line), temp_file) != NULL) {
        fprintf(file, "%s", line);
    }

    // Close the temporary file.
    fclose(temp_file);

    // Close the INI file.
    fclose(file);

    return 0;
}

void create_default_config(void)
{
    FILE *fp;
    char cwd[MAX_FILE_PATH_CHAR];

    // Get current working dir
    strcpy(cwd, getenv("HOME"));

    // create directory PokeromTrader
    const char *dir_path = "/Library/PokeromTrader";
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
    fp = fopen(cwd, "w+");

    if (fp == NULL)
    {
        printf("Error %d creating file!\n", errno);
        exit(errno);
    }

    fputs("# This is a generated file. Only modify values not keys.\n", fp);

    // Write default key values to new config.ini
    char default_key[MAX_FILE_PATH_CHAR];
    strcpy(default_key, "SAVE_FILE_DIR=");
    strcat(default_key, saves_dir);
    fputs(default_key, fp);
    fputs("\n", fp);
    fputs("DISABLE_RANDOM_IVS_ON_TRADE=false", fp);
    fputs("\n", fp);
    fputs("ITEM_REQUIRED_EVOLUTIONS=true", fp);

    fclose(fp);
}

char *read_key_from_config(const char *key)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    char *value = NULL;

    char config_path[MAX_FILE_PATH_CHAR];
    strcpy(config_path, getenv("HOME"));
    strcat(config_path, "/Library/PokeromTrader/config.ini");

    fp = fopen(config_path, "r");

    // If missing, then create
    if (fp == NULL)
    {
        create_default_config();

        // open after creation
        fp = fopen(config_path, "r");
    }

    while (getline(&line, &len, fp) != -1)
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
    free(resolved_path);
    free(absolute_path);
}
