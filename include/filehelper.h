#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !defined(_WIN32)
#include <dirent.h>
#endif
#include "common.h"
#include "pksavhelper.h"

// Global pointers for resolved_path and absolute_path
extern char *resolved_path;
extern char *absolute_path;

// Function to get save files in a directory
int get_save_files(struct save_file_data *save_data);
// free global pointers
void free_filehelper_pointers(void);
int write_key_to_config(const char *key, const char *value);
int delete_app_data(void);
void init_settings_from_config(struct save_file_data *save_file_data);
void write_to_log(const char *msg, const uint8_t message_type);
#ifdef _WIN32
struct config_data
{
    char *save_file_dir;
    char *disable_random_ivs_on_trade;
    char *item_required_evolutions;
};

struct config_data read_key_from_config(void);
void create_default_config(void);
#else
char *read_key_from_config(const char *key);
void create_default_config(bool overwrite);
#endif

#endif /* FILEHELPER_H */
