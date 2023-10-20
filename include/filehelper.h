#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "common.h"
#include "pksavhelper.h"

// Global pointers for resolved_path and absolute_path
extern char *resolved_path;
extern char *absolute_path;

// Function to get save files in a directory
int get_save_files(struct SaveFileData *save_data);

// free global pointers
void free_filehelper_pointers(void);
int write_key_to_config(const char *key, const char *value);
char *read_key_from_config(const char *key);
int delete_app_data(void);
void create_default_config(bool overwrite);
void init_settings_from_config(struct SaveFileData *save_file_data);

#endif /* FILEHELPER_H */
