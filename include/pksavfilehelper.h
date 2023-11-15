#include <stdio.h>
#include "pksavhelper.h"

static enum pksav_error SAVE_FILE_ERROR = PKSAV_ERROR_NONE;

enum pksav_error detect_savefile_generation(const char *path, SaveGenerationType *save_generation_type);
PokemonSave load_savefile_from_path(const char *path);
pksavhelper_error save_savefile_to_path(PokemonSave *pkmn_save, char *path);
void load_display_files(const struct save_file_data *save_file_data, PokemonSave *pkmn_saves);
