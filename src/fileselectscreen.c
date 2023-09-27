#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "fileselectscreen.h"
#include "pksavhelper.h"
#include "common.h"

void DrawFileSelectScreen(struct pksav_gen2_save *save_player1, struct pksav_gen2_save *save_player2, char *player1_save_path, char *player2_save_path, struct TrainerInfo *trainer1, struct TrainerInfo *trainer2, struct TrainerSelection trainerSelection[2], struct SaveFileData *save_file_data, GameScreen *current_screen, Error_Handler error_handler)
{
    static int selected_saves_index[2] = {-1, -1};
    bool hasSelectedTwoSaves = selected_saves_index[0] != -1 && selected_saves_index[1] != -1;

    if (save_file_data->numSaves == 0)
    {
        DrawText(TextFormat("No save files found in save folder %s/", save_file_data->saveDir), 190, 250, 20, BLACK);
    }

    DrawText("Select two save files to trade between", 190, 100, 20, BLACK);
    for (int i = 0; i < save_file_data->numSaves; i++)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 200 + 25 * i, SCREEN_WIDTH / 2, 25}))
            {   
                if (selected_saves_index[0] == i) {
                    selected_saves_index[0] = -1;
                } else if (selected_saves_index[1] == i) {
                    selected_saves_index[1] = -1;
                } else if (selected_saves_index[0] == -1) {
                    selected_saves_index[0] = i;
                } else if (selected_saves_index[1] == -1) {
                    selected_saves_index[1] = i;
                }
            }
        }
        char *save_name = strrchr(save_file_data->saves_file_path[i], '/');
        save_name++;

        DrawText(save_name, 190, 200 + 25 * i, 20, (selected_saves_index[0] == i || selected_saves_index[1] == i) ? LIGHTGRAY : BLACK);
        DrawText("Trade >", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, hasSelectedTwoSaves ? BLACK : LIGHTGRAY);

        if (hasSelectedTwoSaves)
        {
            printf("selected_saves_index[0]: %d\n", selected_saves_index[0]);
            printf("selected_saves_index[1]: %d\n", selected_saves_index[1]);
            printf("selected_saves_path[0]: %s\n", save_file_data->saves_file_path[selected_saves_index[0]]);
            printf("selected_saves_path[1]: %s\n", save_file_data->saves_file_path[selected_saves_index[1]]);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
                {
                    *current_screen = SCREEN_TRADE;
                    // load selection to player1_save
                    *save_player1 = loadSaveFromFile(save_file_data->saves_file_path[selected_saves_index[0]], error_handler);
                    // save the selected path name
                    strcpy(player1_save_path, save_file_data->saves_file_path[selected_saves_index[0]]);
                    // generate trainer info from save
                    create_trainer(save_player1, trainer1);
                    // save trainer id to trainerSelection
                    trainerSelection[0].trainer_id = trainer1->trainer_id;

                    // load selection to player2_save
                    *save_player2 = loadSaveFromFile(save_file_data->saves_file_path[selected_saves_index[1]], error_handler);
                    strcpy(player2_save_path, save_file_data->saves_file_path[selected_saves_index[1]]);
                    create_trainer(save_player2, trainer2);
                    trainerSelection[1].trainer_id = trainer2->trainer_id;
                }
            }
        }
    }
    // Add back button
    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            *current_screen = SCREEN_MAIN_MENU;
            selected_saves_index[0] = -1;
            selected_saves_index[1] = -1;
            trainer1->trainer_id = 0;
            trainer2->trainer_id = 0;
            save_player1 = NULL;
            save_player2 = NULL;
        }
    }
}