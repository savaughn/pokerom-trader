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
    PokemonSave pokemon_save_player1;
    PokemonSave pokemon_save_player2;
    struct SaveFileData save_file_data;
    // Read and save the saves file directory from config.ini
    char *config_save_path = read_key_from_config("SAVE_FILE_DIR");
    if (config_save_path != NULL)
    {
        strcpy((char *)save_file_data.saveDir, config_save_path);
    } else {
        strcpy((char *)save_file_data.saveDir, "DIR_NOT_SET");
    }

    // Read and save the disable random setting from config.ini
    set_is_random_DVs_disabled(strcmp(read_key_from_config("DISABLE_RANDOM_IVS_ON_TRADE"), "false"));

    // malloc'd from read_key_from_config
    free(config_save_path);

    // create trainers
    struct TrainerInfo trainer1 = {
        .trainer_id = 0,
    };
    struct TrainerInfo trainer2 = {
        .trainer_id = 0,
    };
    // A struct to hold the trainer selection data
    struct TrainerSelection trainerSelection[2] = {
        [0] = {.trainer_id = trainer1.trainer_id, .pokemon_index = -1, .trainer_index = 0},
        [1] = {.trainer_id = trainer2.trainer_id, .pokemon_index = -1, .trainer_index = 1}};

    // Draw
    DrawRaylibScreenLoop(
        &save_file_data,
        &trainer1,
        &trainer2,
        trainerSelection,
        player1_save_path,
        player2_save_path,
        &pokemon_save_player1,
        &pokemon_save_player2);

    free_filehelper_pointers();

    return 0;
}
