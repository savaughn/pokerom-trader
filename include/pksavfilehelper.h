#include <stdio.h>
#include "common.h"
#include "pksavhelper.h"

enum pksav_error detectSaveFileGeneration(const char *path, SaveGenerationType *save_generation_type);
PokemonSave loadSaveFromFile(const char *path);
void saveToFile(PokemonSave *pokemon_save, char *path);
