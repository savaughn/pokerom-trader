#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "common.h"

// Global pointers for resolvedPath and absolutePath
extern char *resolvedPath;
extern char *absolutePath;

// Function to get save files in a directory
int get_save_files(struct SaveFileData *save_data);

// free global pointers
void free_filehelper_pointers(void);

void write_key_to_config(char *key, char *value);
char *read_key_from_config(char *key);
char *get_absolute_path(char *path);

#endif /* FILEHELPER_H */
