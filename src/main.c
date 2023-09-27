#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <pksav.h>
#include "raylib.h"
#include "pksavhelper.h"
#include "tradescreen.h"
#include "fileselectscreen.h"
#include "filehelper.h"
#include "common.h"

int main(int argc, char *argv[])
{
    InitWindow(1280, 720, "Pokerom Trader");

    char player1_save_path[100];
    char player2_save_path[100];
    struct pksav_gen2_save save_player1;
    struct pksav_gen2_save save_player2;
    struct SaveFileData save_file_data = {
        .saveDir = "saves",
        .numSaves = 0
    };
    get_save_files(&save_file_data);

    // create trainers
    struct TrainerInfo trainer1 = {
        .trainer_id = 0};
    struct TrainerInfo trainer2 = {
        .trainer_id = 0};
    struct TrainerSelection trainerSelection[2] = {
        [0] = {.trainer_id = trainer1.trainer_id, .pokemon_index = -1, .trainer_index = 0},
        [1] = {.trainer_id = trainer2.trainer_id, .pokemon_index = -1, .trainer_index = 1}};
    static GameScreen current_screen = SCREEN_FILE_SELECT;

    while (!WindowShouldClose())
    {
        // Update
        int selected_index_trainer1 = trainerSelection[0].pokemon_index;
        int selected_index_trainer2 = trainerSelection[1].pokemon_index;
        static bool should_trade = false;

        if (should_trade && selected_index_trainer1 != -1 && selected_index_trainer2 != -1)
        {
            // Reset Trading state
            should_trade = false;
            trainerSelection[0].pokemon_index = -1;
            trainerSelection[1].pokemon_index = -1;

            swapPokemonAtIndexBetweenSaves(&save_player1, &save_player2, selected_index_trainer1, selected_index_trainer2);
            saveToFile(&save_player1, player1_save_path, &error_handler);
            saveToFile(&save_player2, player2_save_path, &error_handler);
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (current_screen)
        {
        case SCREEN_FILE_SELECT:
            DrawFileSelectScreen(&save_player1, &save_player2, player1_save_path, player2_save_path, &trainer1, &trainer2, trainerSelection, &save_file_data, &current_screen, error_handler);
            break;

        case SCREEN_TRADE:
            DrawTradeScreen(trainerSelection, &trainer1, &trainer2, &should_trade);
        default:
            break;
        }
        EndDrawing();
    }

    free_filehelper_pointers();

    return 0;
}
