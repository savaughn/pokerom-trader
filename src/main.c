#include <string.h>
#include <pksav.h>
#include "common.h"
#include "filehelper.h"
#include "raylibhelper.h"
#include "pksavhelper.h"

int main(int argc, char *argv[])
{
    char player1_save_path[MAX_FILE_PATH_CHAR];
    char player2_save_path[MAX_FILE_PATH_CHAR];
    PokemonSave pkmn_save_player1;
    PokemonSave pkmn_save_player2;
    struct SaveFileData save_file_data;
    // Read and save the saves file directory from config.ini
    char *config_save_path = read_key_from_config("SAVE_FILE_DIR");
    if (config_save_path != NULL)
    {
        strcpy((char *)save_file_data.save_dir, config_save_path);
    } else {
        strcpy((char *)save_file_data.save_dir, "DIR_NOT_SET");
    }

    // Read and save the disable random setting from config.ini
    set_is_random_DVs_disabled(strcmp(read_key_from_config("DISABLE_RANDOM_IVS_ON_TRADE"), "false"));
    // Read and save the item required evolutions setting from config.ini
    set_is_item_required(strcmp(read_key_from_config("ITEM_REQUIRED_EVOLUTIONS"), "false"));
    // Read and save the language setting from config.ini
    const char *lang = read_key_from_config("LANGUAGE");
    if (lang != NULL)
    {
        if (strcmp(lang, "EN") == 0)
        {
            set_language(EN);
        }
        else if (strcmp(lang, "FR") == 0)
        {
            set_language(FR);
        }
        else if (strcmp(lang, "DE") == 0)
        {
            set_language(DE);
        }
        else if (strcmp(lang, "IT") == 0)
        {
            set_language(IT);
        }
        else if (strcmp(lang, "ES") == 0)
        {
            set_language(ES);
        }
        else if (strcmp(lang, "JA") == 0)
        {
            set_language(JA);
        }
        else if (strcmp(lang, "KO") == 0)
        {
            set_language(KO);
        }
        else if (strcmp(lang, "ZH") == 0)
        {
            set_language(ZH);
        }
        else if (strcmp(lang, "ZH_TW") == 0)
        {
            set_language(ZH_TW);
        }
        else
        {
            set_language(EN);
        }
    } else {
        set_language(EN);
    }

    // malloc'd from read_key_from_config
    free(config_save_path);

    // create trainers
    struct TrainerInfo trainer_1 = {
        .trainer_id = 0,
    };
    struct TrainerInfo trainer_2 = {
        .trainer_id = 0,
    };
    // A struct to hold the trainer selection data
    struct TrainerSelection trainer_selection[2] = {
        [0] = {.trainer_id = trainer_1.trainer_id, .pkmn_party_index = -1, .trainer_index = 0},
        [1] = {.trainer_id = trainer_2.trainer_id, .pkmn_party_index = -1, .trainer_index = 1}};

    // Draw
    draw_raylib_screen_loop(
        &save_file_data,
        &trainer_1,
        &trainer_2,
        trainer_selection,
        player1_save_path,
        player2_save_path,
        &pkmn_save_player1,
        &pkmn_save_player2);

    free_filehelper_pointers();

    return 0;
}
