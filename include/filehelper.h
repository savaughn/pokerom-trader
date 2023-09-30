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
void get_save_files(struct SaveFileData *save_data);

// free global pointers
void free_filehelper_pointers(void);

#endif /* FILEHELPER_H */
