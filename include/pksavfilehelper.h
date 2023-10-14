#include <stdio.h>
#include "common.h"
#include "pksavhelper.h"

enum pksav_error detect_savefile_generation(const char *path, SaveGenerationType *save_generation_type);
PokemonSave *load_savefile_from_path(const char *path);
void save_savefile_to_path(PokemonSave *pokemon_save, char *path);
