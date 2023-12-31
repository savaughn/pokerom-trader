#include "common.h"
#include "filehelper.h"
#include "raylibhelper.h"
#include "pksavfilehelper.h"

int main(int argc, char *argv[])
{
    char player1_save_path[MAX_FILE_PATH_CHAR];
    char player2_save_path[MAX_FILE_PATH_CHAR];
    PokemonSave pkmn_save_player1;
    PokemonSave pkmn_save_player2;
    struct save_file_data save_file_data = {
        .num_saves = 0,
    };

    init_settings_from_config(&save_file_data);

    // create trainers
    struct trainer_info trainer1 = {
        .trainer_id = 0,
    };
    struct trainer_info trainer2 = {
        .trainer_id = 0,
    };
    // A struct to hold the trainer selection data
    struct TrainerSelection trainer_selection[2] = {
        [0] = {.trainer_id = trainer1.trainer_id, .pkmn_party_index = -1, .trainer_index = 0},
        [1] = {.trainer_id = trainer2.trainer_id, .pkmn_party_index = -1, .trainer_index = 1}};

    // Draw
    draw_raylib_screen_loop(
        &save_file_data,
        &trainer1,
        &trainer2,
        trainer_selection,
        player1_save_path,
        player2_save_path,
        &pkmn_save_player1,
        &pkmn_save_player2);

    free_filehelper_pointers();
    // free allocated save file data
    free_trade_saves();
    free_evolve_saves();

    return 0;
}
