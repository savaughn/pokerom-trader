#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <pksav.h>
#include "common.h"
#include "raylib.h"
#include "pksavhelper.h"
#include "tradescreen.h"
#include "fileselectscreen.h"
#include "filehelper.h"

#define MAX_INPUT_CHARS 40

int main(int argc, char *argv[])
{
    InitWindow(800, 480, "Pokerom Trader");

    char player1_save_path[100];
    char player2_save_path[100];
    struct pksav_gen2_save save_player1;
    struct pksav_gen2_save save_player2;
    struct SaveFileData save_file_data = {
        .saveDir = "saves",
    };
    get_save_files(&save_file_data);

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
    static GameScreen current_screen = SCREEN_MAIN_MENU;

    bool should_close_window = false;

    char inputText[MAX_INPUT_CHARS + 1] = "\0"; // Input text buffer
    int textSize = 0;                           // Current text size

    Rectangle inputBox = { 50, SCREEN_HEIGHT / 2 - 20, SCREEN_WIDTH - 100, 40};
    bool editingText = false; // Flag to indicate if the text is being edited

    while (!should_close_window && !WindowShouldClose())
    {
        // Escape key to close window
        if (IsKeyPressed(KEY_ESCAPE))
        {
            should_close_window = true;
        }

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
            updateSeenOwnedPokemon(&save_player1, selected_index_trainer1);
            updateSeenOwnedPokemon(&save_player2, selected_index_trainer2);
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
            DrawTradeScreen(trainerSelection, &trainer1, &trainer2, &should_trade, &current_screen);
            break;
        case SCREEN_MAIN_MENU:
            DrawText("Main Menu", 190, 100, 20, BLACK);
            DrawText("Trade", 190, 200, 20, BLACK);
            DrawText("Settings", 190, 225, 20, BLACK);
            DrawText("Quit", 190, 250, 20, BLACK);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 200, 200, 20}))
                {
                    current_screen = SCREEN_FILE_SELECT;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 225, 200, 20}))
                {
                    current_screen = SCREEN_SETTINGS;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 250, 200, 20}))
                {
                    should_close_window = true;
                }
            }
            break;
        case SCREEN_SETTINGS:
            DrawText("Settings", 190, 100, 20, BLACK);
            DrawText("Change Save Directory", 190, 200, 20, BLACK);
            DrawText("Back", 190, 225, 20, BLACK);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 200, 200, 20}))
                {
                    current_screen = SCREEN_FILE_EDIT;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 220, 200, 20}))
                {
                    current_screen = SCREEN_MAIN_MENU;
                }
            }
            break;
        case SCREEN_FILE_EDIT:
            // Placeholder Text
            if (!editingText && textSize == 0) {
                strcpy(inputText, save_file_data.saveDir);
                textSize = strlen(inputText);
            }
        
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Check if the mouse is clicked within the input box
                if (CheckCollisionPointRec(GetMousePosition(), inputBox))
                {
                    editingText = true;
                }
                else
                {
                    editingText = false;
                }
            }

            if (editingText)
            {
                int key = GetCharPressed();
                int backspace = GetKeyPressed();
                if (key >= 32 && key <= 125 && textSize < MAX_INPUT_CHARS)
                {
                    // Append character to inputText
                    inputText[textSize] = (char)key;
                    textSize++;
                }
                else if ((key == KEY_BACKSPACE || backspace == KEY_BACKSPACE) && textSize > 0)
                {
                    // Remove last character
                    textSize--;
                    inputText[textSize] = '\0';
                }

                // Finish editing by pressing Enter
                if (IsKeyPressed(KEY_ENTER))
                {
                    editingText = false;
                }
            }
            DrawText("Specify folder name containing saves", 50, SCREEN_HEIGHT / 2 - 75, 20, BLACK);
            DrawText("relative to executable (e.g. \"../my_saves\" or \"saves\")", 50, SCREEN_HEIGHT / 2 - 50, 20, BLACK);

            // Draw the input box
            DrawRectangleRec(inputBox, WHITE);
            DrawRectangleLinesEx(inputBox, 2, editingText ? BLACK : DARKGRAY);

            // Draw the text inside the input box
            DrawText(inputText, inputBox.x + 10, inputBox.y + 10, 20, BLACK);

            // Draw the blinking cursor
            if (editingText)
            {
                DrawLine(inputBox.x + 8 + MeasureText(inputText, 20), inputBox.y + 15,
                         inputBox.x + 8 + MeasureText(inputText, 20), inputBox.y + 25, BLACK);
            }

            // Draw the save button
            DrawText("Save!", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, textSize ? BLACK : LIGHTGRAY);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}) && textSize > 0)
                {
                    save_file_data.saveDir = inputText;
                    save_file_data.numSaves = 0;
                    *save_file_data.saves_file_path = NULL;
                    get_save_files(&save_file_data);
                    current_screen = SCREEN_SETTINGS;
                }
            }

            // add a back button
            DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, BLACK);
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    current_screen = SCREEN_SETTINGS;
                }
            }
            break;
        default:
            break;
        }
        EndDrawing();
    }

    free_filehelper_pointers();

    return 0;
}
