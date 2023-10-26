#include "filehelper.h"
#include <string.h>
#ifdef _WIN32
#include <errno.h>
#else
#include <unistd.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#ifdef __APPLE__
#define USR_DATA_DIR "/Library/PokeromTrader"
#elif _WIN32
#define USR_DATA_DIR "\%APPDATA\%/PokeromTrader"
#else
#define USR_DATA_DIR "/.pokeromtrader"
#endif

char *resolved_path = NULL;
char *absolute_path = NULL;
#ifdef _WIN32
int get_save_files(struct SaveFileData *save_data)
{
    return 0;
}
int write_key_to_config(const char *key, const char *value)
{
    return 0;
}
void create_default_config(bool overwrite)
{
    return;
}
char *read_key_from_config(const char *key)
{
    return NULL;
}
void init_settings_from_config(struct SaveFileData *save_file_data)
{
    return;
}
void write_to_log(const char *msg, const uint8_t message_type)
{
    return;
}
int delete_app_data(void)
{
    return 0;
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
    strcat(config_path, USR_DATA_DIR);
    strcat(config_path, "/config.ini");
    // Open the INI file for reading and writing.
    FILE *file = fopen(config_path, "r+");
    if (file == NULL)
    {
        perror("Error opening INI file");
        return 1;
    }

    // Create a temporary file to store the modified contents.
    FILE *temp_file = tmpfile();
    if (temp_file == NULL)
    {
        perror("Error creating temporary file");
        fclose(file);
        return 1;
    }

    char line[1024];
    int key_found = 0;

    // Read the INI file line by line.
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Check if the line starts with the desired key.
        if (strncmp(line, key, strlen(key)) == 0)
        {
            // Key found, update the value.
            fprintf(temp_file, "%s=%s\n", key, value);
            key_found = 1;
        }
        else
        {
            // Copy the line as-is to the temporary file.
            fprintf(temp_file, "%s", line);
        }
    }

    // If the key was not found, add it to the end of the file.
    if (!key_found)
    {
        fprintf(temp_file, "%s=%s\n", key, value);
    }

    // Close both files.
    fclose(file);
    rewind(temp_file);

    // Reopen the INI file for writing and copy the contents from the temporary file.
    file = fopen(config_path, "w");
    if (file == NULL)
    {
        perror("Error reopening INI file");
        fclose(temp_file);
        return 1;
    }

    while (fgets(line, sizeof(line), temp_file) != NULL)
    {
        fprintf(file, "%s", line);
    }

    // Close the temporary file.
    fclose(temp_file);

    // Close the INI file.
    fclose(file);

    return 0;
}

void create_default_config(bool overwrite)
{
    FILE *fp;
    char cwd[MAX_FILE_PATH_CHAR];

    // Get current working dir
    strcpy(cwd, getenv("HOME"));

    // create directory PokeromTrader
    const char *dir_path = USR_DATA_DIR;
    strcat(cwd, dir_path);

    // create saves folder
    char saves_dir[MAX_FILE_PATH_CHAR];
    strcpy(saves_dir, cwd);
    strcat(saves_dir, "/saves");

    // create logs folder
    char logs_dir[MAX_FILE_PATH_CHAR];
    strcpy(logs_dir, cwd);
    strcat(logs_dir, "/logs");

    if (!overwrite)
    {

        int status = mkdir(cwd, 0777);
        if (status == -1)
        {
            printf("Error %d creating directory!\n", errno);
            if (errno != 17)
                exit(errno);
        }

        status = mkdir(saves_dir, 0777);
        if (status == -1)
        {
            puts("Error creating saves directory");
            if (errno != 17)
                exit(errno);
        }

        status = mkdir(logs_dir, 0777);
        if (status == -1)
        {
            puts("Error creating logs directory");
            if (errno != 17)
                exit(errno);
        }
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
    strcat(config_path, USR_DATA_DIR);
    strcat(config_path, "/config.ini");

    fp = fopen(config_path, "r");

    // If missing, then create
    if (fp == NULL)
    {
        create_default_config(false);

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

int delete_app_data(void)
{
    char config_path[MAX_FILE_PATH_CHAR];
    strcpy(config_path, getenv("HOME"));
    strcat(config_path, USR_DATA_DIR);

    DIR *dir;
    struct dirent *entry;

    // Delete config.ini
    char config_file[MAX_FILE_PATH_CHAR];
    strcpy(config_file, config_path);
    strcat(config_file, "/config.ini");
    remove(config_file);

    // Delete all .sav files in saves/
    char saves_dir[MAX_FILE_PATH_CHAR];
    strcpy(saves_dir, config_path);
    strcat(saves_dir, "/saves");

    dir = opendir(saves_dir);
    if (dir == NULL)
    {
        perror("Error opening directory: ");
        printf("Path: %s\n", saves_dir);
        return 1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Check if the entry is a regular file with a .sav extension
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".sav"))
        {
            // Combine the base path and file name
            char full_path[strlen(saves_dir) + 1 + strlen(entry->d_name) + 1];
            sprintf(full_path, "%s/%s", saves_dir, entry->d_name);

            // Delete file
            remove(full_path);
            unlink(full_path);
        }
    }

    closedir(dir);

    // delete logs
    char logs_dir[MAX_FILE_PATH_CHAR];
    strcpy(logs_dir, config_path);
    strcat(logs_dir, "/logs");

    dir = opendir(logs_dir);
    if (dir == NULL)
    {
        perror("Error opening directory: ");
        printf("Path: %s\n", logs_dir);
        return 1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Check if the entry is a regular file with a .sav extension
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".txt"))
        {
            // Combine the base path and file name
            char full_path[strlen(logs_dir) + 1 + strlen(entry->d_name) + 1];
            sprintf(full_path, "%s/%s", logs_dir, entry->d_name);

            // Delete file
            remove(full_path);
            unlink(full_path);
        }
    }

    closedir(dir);

    // Delete saves/ directory
    int status = rmdir(saves_dir);
    if (status == -1)
    {
        perror("Error deleting saves directory: ");
        printf("Path: %s\n", saves_dir);
    }

    // Delete logs/ directory
    status = rmdir(logs_dir);
    if (status == -1)
    {
        perror("Error deleting logs directory: ");
        printf("Path: %s\n", logs_dir);
    }

    // Delete PokeromTrader/ directory
    status = rmdir(config_path);
    if (status == -1)
    {
        perror("Error deleting PokeromTrader directory: ");
        printf("Path: %s\n", config_path);
    }

    return 0;
}

void init_settings_from_config(struct SaveFileData *save_file_data)
{
    // Read and save the saves file directory from config.ini
    char *config_save_path = read_key_from_config("SAVE_FILE_DIR");
    
    if (config_save_path != NULL)
    {
        strcpy((char *)save_file_data->save_dir, config_save_path);
    } else {
        strcpy((char *)save_file_data->save_dir, "DIR_NOT_SET");
    }

    // Read and save the disable random setting from config.ini
    set_is_random_DVs_disabled(strcmp(read_key_from_config("DISABLE_RANDOM_IVS_ON_TRADE"), "false"));
    // Read and save the item required evolutions setting from config.ini
    set_is_item_required(strcmp(read_key_from_config("ITEM_REQUIRED_EVOLUTIONS"), "false"));

    // malloc'd from read_key_from_config
    free(config_save_path);
}

void write_to_log(const char *msg, const uint8_t message_type)
{
    FILE *fp;
    char cwd[MAX_FILE_PATH_CHAR];

    strcpy(cwd, getenv("HOME"));
    strcat(cwd, USR_DATA_DIR);
    strcat(cwd, "/logs");

    char date[20];
    char time_str[20];
    time_t now = time(NULL);
    const struct tm *t = localtime(&now);

    strftime(date, sizeof(date) - 1, "%Y-%m-%d", t);
    strftime(time_str, sizeof(time_str) - 1, "%H:%M:%S", t);

    switch (message_type)
    {
    case E_LOG_MESSAGE_TYPE_ERROR:
        strcat(cwd, "/error-log@");
        break;
    case E_LOG_MESSAGE_TYPE_INFO:
        strcat(cwd, "/info-log@");
        break;
    default:
        strcat(cwd, "/log@");
        break;
    }
    strcat(cwd, date);
    strcat(cwd, ".txt");
    
    fp = fopen(cwd, "a+");

    if (fp == NULL)
    {
        printf("Error %d creating file!\n", errno);
        exit(errno);
    }

    fputs(time_str, fp);
    fputs(": ", fp);
    fputs(msg, fp);
    fputs("\n", fp);

    fclose(fp);
}

#endif

void free_filehelper_pointers(void)
{
    free(resolved_path);
    free(absolute_path);
}
