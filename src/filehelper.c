#include "filehelper.h"
#ifdef _WIN32
#include <errno.h>
#include <Windows.h>
#include <tchar.h>
#include <stdint.h>
#include <time.h>
#else
#include <unistd.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#ifdef __APPLE__
#define USR_DATA_DIR "/Library/PokeromTrader"
#elif _WIN32
#define USR_DATA_DIR "\\Documents\\PokeromTrader"
#else
#define USR_DATA_DIR "/.pokeromtrader"
#endif

char *resolved_path = NULL;
char *absolute_path = NULL;
#ifdef _WIN32
int get_save_files(struct SaveFileData *save_data)
{
    // Get .sav files from save folder
    char save_dir[MAX_FILE_PATH_CHAR];
    strcpy(save_dir, (char *)save_data->save_dir);

    WIN32_FIND_DATA find_file_data;
    HANDLE hFind;
    LPCSTR saves[MAX_FILE_PATH_CHAR];
    strcpy(saves, save_dir);
    strcat(saves, "\\*.sav");

    hFind = FindFirstFile(saves, &find_file_data);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("FindFirstFile failed (%d)\n", GetLastError());
        save_data->num_saves = 0;
        // return 1;
    }
    else
    {
        int num_saves = 0;
        do
        {
            // Combine the base path and file name
            char full_path[MAX_FILE_PATH_CHAR];
            sprintf(full_path, "%s/%s", save_dir, find_file_data.cFileName);
            printf("Found file: %s\n", full_path);
                save_data->saves_file_path[num_saves] = malloc(strlen(full_path) + 1);
            strcpy(save_data->saves_file_path[num_saves], full_path);
            printf("Saved file: %s\n", save_data->saves_file_path[num_saves]);
            num_saves++;
        } while (FindNextFile(hFind, &find_file_data) != 0);
        FindClose(hFind);
        save_data->num_saves = num_saves;
        printf("num_saves from load: %d\n", save_data->num_saves);
    }

    return 0;
}
int write_key_to_config(const char *key, const char *value)
{
    char config_path[MAX_FILE_PATH_CHAR];
    strcpy(config_path, getenv("USERPROFILE"));
    strcat(config_path, USR_DATA_DIR);
    strcat(config_path, "\\config.ini");

    LPCSTR ini = config_path;

    WINBOOL error = WritePrivateProfileStringA("app", key, value, ini);
    if (error == FILE_OP_FAILURE)
        error_handler(error, "Error writing to config.ini");
    return error;
}
void create_default_config(void)
{
    char config_path[MAX_PATH];

    // Get the current working directory
    strcpy(config_path, getenv("USERPROFILE"));
    strcat(config_path, USR_DATA_DIR);

    // Create the saves folder
    char saves_dir[MAX_PATH];
    strcpy(saves_dir, config_path);
    strcat(saves_dir, "\\saves");

    // Create the logs folder
    char logs_dir[MAX_PATH];
    strcpy(logs_dir, config_path);
    strcat(logs_dir, "\\logs");

    // Create directories
    if (!CreateDirectory(config_path, NULL))
    {
        if (GetLastError() != ERROR_ALREADY_EXISTS)
        {
            printf("Error creating directory: %d\n", GetLastError());
            printf("Path: %s\n", config_path);
            exit(GetLastError());
        }
    }

    if (!CreateDirectory(saves_dir, NULL))
    {
        if (GetLastError() != ERROR_ALREADY_EXISTS)
        {
            printf("Error creating saves directory: %d\n", GetLastError());
            exit(GetLastError());
        }
    }

    if (!CreateDirectory(logs_dir, NULL))
    {
        if (GetLastError() != ERROR_ALREADY_EXISTS)
        {
            printf("Error creating logs directory: %d\n", GetLastError());
            exit(GetLastError());
        }
    }

    // Create config.ini file in cwd
    strcat(config_path, "\\config.ini");
    FILE *fp = fopen(config_path, "w+");

    if (fp == NULL)
    {
        printf("Error creating file: %d\n", GetLastError());
        exit(GetLastError());
    }

    fputs("# This is a generated file. Only modify values, not keys.\n", fp);
    fclose(fp);

    LPCSTR ini = config_path;
    WritePrivateProfileStringA("app", "SAVE_FILE_DIR", saves_dir, ini);
    WritePrivateProfileStringA("app", "DISABLE_RANDOM_IVS_ON_TRADE", "false", ini);
    WritePrivateProfileStringA("app", "ITEM_REQUIRED_EVOLUTIONS", "true", ini);
}
struct config_data read_key_from_config(void)
{
    char config_path[MAX_FILE_PATH_CHAR];
    strcpy(config_path, getenv("USERPROFILE"));
    strcat(config_path, USR_DATA_DIR);

    // if USR_DATA_DIR does not exist, create it
    if (GetFileAttributes(config_path) == INVALID_FILE_ATTRIBUTES)
    {
        create_default_config();
    }

    strcat(config_path, "\\config.ini");

    LPCSTR ini = config_path;
    char save_file_str[MAX_FILE_PATH_CHAR];
    GetPrivateProfileString("app", "SAVE_FILE_DIR", 0, save_file_str, MAX_FILE_PATH_CHAR, ini);

    char disable_random_ivs_on_trade[MAX_FILE_PATH_CHAR];
    GetPrivateProfileString("app", "DISABLE_RANDOM_IVS_ON_TRADE", 0, disable_random_ivs_on_trade, MAX_FILE_PATH_CHAR, ini);

    char item_required_evolutions[MAX_FILE_PATH_CHAR];
    GetPrivateProfileString("app", "ITEM_REQUIRED_EVOLUTIONS", 0, item_required_evolutions, MAX_FILE_PATH_CHAR, ini);

    return (struct config_data){
        .save_file_dir = save_file_str,
        .disable_random_ivs_on_trade = disable_random_ivs_on_trade,
        .item_required_evolutions = item_required_evolutions,
    };
}
void write_to_log(const char *msg, const uint8_t message_type)
{
    FILE *fp;

    char config_path[MAX_FILE_PATH_CHAR];
    strcpy(config_path, getenv("USERPROFILE"));
    strcat(config_path, USR_DATA_DIR);
    strcat(config_path, "\\logs");

    char date[20];
    char time_str[20];
    time_t now = time(NULL);
    struct tm t = {0};
    localtime_s(&t, &now);

    strftime(date, sizeof(date) - 1, "%Y-%m-%d", &t);
    strftime(time_str, sizeof(time_str) - 1, "%H:%M:%S", &t);

    switch (message_type)
    {
    case E_LOG_MESSAGE_TYPE_ERROR:
        strcat(config_path, "\\error-log@");
        break;
    case E_LOG_MESSAGE_TYPE_INFO:
        strcat(config_path, "\\info-log@");
        break;
    default:
        strcat(config_path, "\\log@");
        break;
    }
    strcat(config_path, date);
    strcat(config_path, ".txt");

    fp = fopen(config_path, "a+");

    if (fp == NULL)
    {
        printf("Error %d creating file!\n", GetLastError());
        exit(GetLastError());
    }

    fputs(time_str, fp);
    fputs(": ", fp);
    fputs(msg, fp);
    fputs("\n", fp);

    fclose(fp);
}
int delete_app_data(void)
{
    char config_path[MAX_FILE_PATH_CHAR];
    strcpy(config_path, getenv("USERPROFILE"));
    strcat(config_path, USR_DATA_DIR);

    // Delete config.ini
    char config_file[MAX_FILE_PATH_CHAR];
    strcpy(config_file, config_path);
    strcat(config_file, "\\config.ini");
    remove(config_file);

    // Delete all .sav files in saves/
    char saves_dir[MAX_FILE_PATH_CHAR];
    strcpy(saves_dir, config_path);
    strcat(saves_dir, "\\saves");

    WIN32_FIND_DATA find_file_data;
    HANDLE hFind;
    LPCSTR saves[MAX_FILE_PATH_CHAR];
    strcpy(saves, saves_dir);
    strcat(saves, "\\*.sav");

    hFind = FindFirstFile(saves, &find_file_data);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Empty (saves) (%d)\n", GetLastError());
    }
    else
    {
        do
        {
            // hfind contains .sav extension
            if (strstr(find_file_data.cFileName, ".sav") == NULL)
            {
                continue;
            }

            // Combine the base path and file name
            char full_path[MAX_FILE_PATH_CHAR];
            sprintf(full_path, "%s/%s", saves_dir, find_file_data.cFileName);
            // Delete file
            remove(full_path);
            unlink(full_path);
        } while (FindNextFile(hFind, &find_file_data) != 0);
        FindClose(hFind);
    }

    // Delete saves/ directory
    int status = rmdir(saves_dir);
    if (status == -1)
    {
        printf("Error deleting saves directory: %d\n", GetLastError());
    }

    // Delete logs/ directory
    char logs_dir[MAX_FILE_PATH_CHAR];
    strcpy(logs_dir, config_path);
    strcat(logs_dir, "\\logs");

    LPCSTR logs[MAX_FILE_PATH_CHAR];
    strcpy(logs, logs_dir);
    strcat(logs, "\\*.txt");

    hFind = FindFirstFileA(logs, &find_file_data);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Empty (logs) (%d)\n", GetLastError());
    }
    else
    {
        do
        {
            // Combine the base path and file name
            char full_path[MAX_FILE_PATH_CHAR];
            sprintf(full_path, "%s/%s", logs_dir, find_file_data.cFileName);

            // Delete file
            remove(full_path);
            unlink(full_path);
        } while (FindNextFile(hFind, &find_file_data) != 0);
        FindClose(hFind);
    }

    // Delete logs/ directory
    status = rmdir(logs_dir);
    if (status == -1)
    {
        printf("Error deleting logs directory: %d\n", GetLastError());
    }

    // Delete PokeromTrader/ directory
    status = rmdir(config_path);
    if (status == -1)
    {
        printf("Error deleting PokeromTrader directory: %d\n", GetLastError());
    }

    return 0;
}

void init_settings_from_config(struct SaveFileData *save_file_data)
{
    // Read and save the saves file directory from config.ini
    struct config_data config_data = read_key_from_config();

    char *save_file_dir = malloc(strlen(config_data.save_file_dir) + 1);
    if (save_file_dir != NULL)
    {
        strcpy(save_file_dir, config_data.save_file_dir);
        strcpy((char *)save_file_data->save_dir, save_file_dir);
    }
    else
    {
        strcpy((char *)save_file_data->save_dir, "DIR_NOT_SET");
    }

    set_is_random_DVs_disabled(strcmp(config_data.disable_random_ivs_on_trade, "false"));
    set_is_item_required(strcmp(config_data.item_required_evolutions, "false"));

    free(save_file_dir);
}

#else

int get_save_files(struct save_file_data *save_data)
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

void init_settings_from_config(struct save_file_data *save_file_data)
{
    // Read and save the saves file directory from config.ini
    char *config_save_path = read_key_from_config("SAVE_FILE_DIR");

    if (config_save_path != NULL)
    {
        strcpy((char *)save_file_data->save_dir, config_save_path);
    }
    else
    {
        strcpy((char *)save_file_data->save_dir, "DIR_NOT_SET");
    }

    // Read and save the disable random setting from config.ini
    set_is_random_DVs_disabled(strcmp(read_key_from_config("DISABLE_RANDOM_IVS_ON_TRADE"), "false"));
    // Read and save the item required evolutions setting from config.ini
    set_is_item_required(strcmp(read_key_from_config("ITEM_REQUIRED_EVOLUTIONS"), "false"));

    // malloc'd from read_key_from_config
    free(config_save_path);
}

#endif

void free_filehelper_pointers(void)
{
    free(resolved_path);
    free(absolute_path);
}
