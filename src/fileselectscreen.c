#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "fileselectscreen.h"
#include "pksavhelper.h"
#include "common.h"

void DrawFileSelectScreen(struct pksav_gen2_save *save_player1, struct pksav_gen2_save *save_player2, char *player1_save_path, char *player2_save_path, struct TrainerInfo *trainer1, struct TrainerInfo *trainer2, struct TrainerSelection trainerSelection[2], struct SaveFileData *save_file_data, GameScreen *current_screen, Error_Handler error_handler)
{
    static int selected_saves_index[2] = {-1, -1};
    
    if (save_file_data->numSaves == 0 || save_file_data->saves_file_path == NULL)
    {
        DrawText(TextFormat("No save files found in save folder %s/", save_file_data->saveDir), 190, 250, 20, BLACK);
    }

    DrawText("Select a save file", 190, 100, 20, BLACK);
    for (int i = 0; i < save_file_data->numSaves; i++)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 200 + 20 * i, 200, 20}))
            {
                // First clicked file goes to trainer1
                if (trainer1->trainer_id == 0)
                {
                    // load selection to player1_save
                    *save_player1 = loadSaveFromFile(save_file_data->saves_file_path[i], error_handler);
                    // save the selected path name
                    strcpy(player1_save_path, save_file_data->saves_file_path[i]);
                    // generate trainer info from save
                    create_trainer(save_player1, trainer1);
                    // save trainer id to trainerSelection
                    trainerSelection[0].trainer_id = trainer1->trainer_id;
                    // save index of selected save file to block it from being selected again
                    selected_saves_index[0] = i;
                }
                else if (trainer2->trainer_id == 0)
                {
                    // load selection to player2_save
                   if (selected_saves_index[0] != i)
                   {
                        *save_player2 = loadSaveFromFile(save_file_data->saves_file_path[i], error_handler);
                        strcpy(player2_save_path, save_file_data->saves_file_path[i]);
                        create_trainer(save_player2, trainer2);
                        trainerSelection[1].trainer_id = trainer2->trainer_id;
                        selected_saves_index[1] = i;
                   }
                }
            }
        }
        char *save_name = strrchr(save_file_data->saves_file_path[i], '/'); 
        save_name++;

        DrawText(save_name, 190, 200 + 20 * i, 20, (selected_saves_index[0] == i || selected_saves_index[1] == i) ? LIGHTGRAY : BLACK);
        DrawText("Trade", GetScreenWidth() - 150, GetScreenHeight() - 100, 20, (trainer1->trainer_id != 0 && trainer2->trainer_id != 0) ? BLACK : LIGHTGRAY);

        if (trainer1->trainer_id != 0 && trainer2->trainer_id != 0)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){GetScreenWidth() - 150, GetScreenHeight() - 100, 200, 20}))
                {
                    *current_screen = SCREEN_TRADE;
                }
            }
        }
    }
}