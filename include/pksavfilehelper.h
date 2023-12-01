#include <stdio.h>
#include <string.h>
#include "pksavhelper.h"

static enum pksav_error SAVE_FILE_ERROR = PKSAV_ERROR_NONE;

enum pksav_error detect_savefile_generation(const char *path, SaveGenerationType *save_generation_type);
void load_savefile_from_path(const char *path, PokemonSave *pkmn_save);
pksavhelper_error save_savefile_to_path(PokemonSave *pkmn_save, char *path);
void load_display_files(const struct save_file_data *save_file_data, PokemonSave *pkmn_saves, uint8_t *num_saves);
void free_pkmn_saves(PokemonSave *pkmn_saves, uint8_t *save_file_count);
void free_trade_saves(void);
void free_evolve_saves(void);
void create_backup_save(PokemonSave *pkmn_save, char* save_path);
