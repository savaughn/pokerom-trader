#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <pksav.h>
#include "raylib.h"
#include "pksavhelper.h"
#include "tradescreen.h"
#include "common.h"

int error_handler(enum pksav_error error, const char *message)
{
    printf("%s\n", message);
    exit(1);
}

int main(int argc, char *argv[])
{

    InitWindow(800, 450, "Pokerom Trader");

    char filePath[100];
    strcpy(filePath, GetWorkingDirectory());

    char player1_savefile[100];
    strcpy(player1_savefile, filePath);
    strcat(player1_savefile, "/rom/pk-crystal.sav");

    char player2_savefile[100];
    strcpy(player2_savefile, filePath);
    strcat(player2_savefile, "/rom/pk-crystal_player2.sav");

    printf("Player 1 save file: %s\n", player1_savefile);

    struct pksav_gen2_save save_player1 = loadSaveFromFile(player1_savefile, &error_handler);
    struct pksav_gen2_save save_player2 = loadSaveFromFile(player2_savefile, &error_handler);

    const char *saveDir = "rom";
    char *saves[100];
    int numSaves = 0;

    DIR *dir;
    struct dirent *entry;

    dir = opendir(saveDir);
    if (dir == NULL)
    {
        perror("Error opening directory");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Check if the entry is a regular file with a .sav extension
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".sav"))
        {
            saves[numSaves] = (char *)malloc(strlen(saveDir) + 1 + strlen(entry->d_name) + 1);
            strcpy(saves[numSaves], saveDir);
            strcat(saves[numSaves], "/");
            strcat(saves[numSaves], entry->d_name);
            numSaves++;
        }
    }

    closedir(dir);

    // Now, you have the paths to all .sav files in the 'saves' array
    for (int i = 0; i < numSaves; i++)
    {
        printf("Reading save file: %s\n", saves[i]);
        // Load and process each save file as needed
        // Example: struct pksav_gen2_save save_data = loadSaveFromFile(saves[i], &error_handler);
    }

    // Free memory allocated for saves array
    for (int i = 0; i < numSaves; i++)
    {
        free(saves[i]);
    }

    printf("Application directory: %s\n", filePath);

    // create trainers
    struct TrainerInfo trainer1;
    struct TrainerInfo trainer2;
    create_trainer(&save_player1, &trainer1);
    create_trainer(&save_player2, &trainer2);

    struct TrainerSelection trainerSelection[2] = {
    [0] = {.trainer_id = -1, .pokemon_index = -1, .trainer_index = -1},
    [1] = {.trainer_id = -1, .pokemon_index = -1, .trainer_index = -1}};
    trainerSelection[0].trainer_id = trainer1.trainer_id;
    trainerSelection[0].trainer_index = 0;
    trainerSelection[1].trainer_id = trainer2.trainer_id;
    trainerSelection[1].trainer_index = 1;

    static GameScreen current_screen = SCREEN_TRADE;

    // raylib while loop
    while (!WindowShouldClose())
    {
        // Update
        int selected_index_trainer1 = trainerSelection[0].pokemon_index;
        int selected_index_trainer2 = trainerSelection[1].pokemon_index;
        static bool should_trade = false;

        if (should_trade && selected_index_trainer1 != -1 && selected_index_trainer2 != -1)
        {
            should_trade = false;
            trainerSelection[0].pokemon_index = -1;
            trainerSelection[1].pokemon_index = -1;

            swapPokemonAtIndexBetweenSaves(&save_player1, &save_player2, selected_index_trainer1, selected_index_trainer2);
            saveToFile(&save_player1, player1_savefile, &error_handler);
            saveToFile(&save_player2, player2_savefile, &error_handler);
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (current_screen)
        {
        case SCREEN_FILE_SELECT:
            /* code */
            break;

        case SCREEN_TRADE: DrawTradeScreen(&trainerSelection, trainer1, trainer2, &should_trade);
        default:
            break;
        }
        EndDrawing();
    }

    pksav_gen2_free_save(&save_player1);
    pksav_gen2_free_save(&save_player2);

    return 0;
}
