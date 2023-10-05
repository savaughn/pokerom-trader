#include <string.h>
#include <pksav.h>
#include "common.h"
#include "filehelper.h"
#include "raylibhelper.h"
#include "pksavhelper.h"

#define MAX_INPUT_CHARS 40

int main(int argc, char *argv[])
{
    char player1_save_path[100];
    char player2_save_path[100];
    PokemonSave pokemon_save_player1;
    PokemonSave pokemon_save_player2;
    struct SaveFileData save_file_data;
    strcpy((char *)save_file_data.saveDir, "saves");

    // create trainers
    struct TrainerInfo trainer1 = {
        .trainer_id = 0,
    };
    struct TrainerInfo trainer2 = {
        .trainer_id = 0,
    };
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
