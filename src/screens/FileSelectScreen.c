#include "raylibhelper.h"
#include "pksavfilehelper.h"

/**
 * @brief Draw the file select screen
 * @param save_file_data The save file data
 * @param player1_save_path The path to the first player's save file
 * @param player2_save_path The path to the second player's save file
 * @param trainer1 The first player's trainer info
 * @param trainer2 The second player's trainer info
 * @param trainerSelection The trainer selection
 * @param pkmn_save_player1 The first player's save file
 * @param pkmn_save_player2 The second player's save file
 * @param current_screen The current screen
 * @param is_same_generation Whether the save files are the same generation
 * @return void
*/
void draw_file_select(struct SaveFileData *save_file_data, char *player1_save_path, char *player2_save_path, struct TrainerInfo *trainer1, struct TrainerInfo *trainer2, struct TrainerSelection trainerSelection[2], PokemonSave *pkmn_save_player1, PokemonSave *pkmn_save_player2, GameScreen *current_screen, bool *is_same_generation)
{
    static int selected_saves_index[2] = {-1, -1};

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (save_file_data->num_saves == 0)
    {
        if (no_dir_err)
        {
            Vector2 text_center = SCREEN_CENTER("Save folder doesn't exist!", 20);
            DrawText("Save folder doesn't exist!", text_center.x, text_center.y, 20, BLACK);
        }
        else
        {
            DrawText("No save files found in save folder", 190, 200, 20, BLACK);
        }
        char *save_dir = save_file_data->save_dir;
        DrawText(TextFormat("%s", save_dir), SCREEN_CENTER(save_dir, 20).x, 275, 20, BLACK);
    }
    else
    {
        bool has_selected_two_saves = selected_saves_index[0] != -1 && selected_saves_index[1] != -1;
        static bool has_file_error = false;

        DrawText("Select two save files to trade between", SCREEN_WIDTH / 2 - MeasureText("Select two save files to trade between", 20) / 2, 25, 20, BLACK);
        for (int i = 0; i < save_file_data->num_saves; i++)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){100, 75 + 25 * i, SCREEN_WIDTH / 2, 25}))
                {
                    if (selected_saves_index[0] == i)
                    {
                        selected_saves_index[0] = -1;
                    }
                    else if (selected_saves_index[1] == i)
                    {
                        selected_saves_index[1] = -1;
                    }
                    else if (selected_saves_index[0] == -1)
                    {
                        selected_saves_index[0] = i;
                    }
                    else if (selected_saves_index[1] == -1)
                    {
                        selected_saves_index[1] = i;
                    }
                    pkmn_save_player1->save_generation_type = SAVE_GENERATION_NONE;
                    pkmn_save_player2->save_generation_type = SAVE_GENERATION_NONE;
                    has_file_error = false;
                }
            }
            char *save_name = strrchr(save_file_data->saves_file_path[i], '/');
            save_name++;
            DrawText(save_name, 100, 75 + 25 * i, 20, (selected_saves_index[0] == i || selected_saves_index[1] == i) ? LIGHTGRAY : BLACK);
        }
        // Reset generation check
        if (!*is_same_generation)
        {
            *is_same_generation = selected_saves_index[1] == -1;
        }

        DrawText("Trade >", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, has_selected_two_saves && *is_same_generation ? BLACK : LIGHTGRAY);
        if (has_selected_two_saves)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
                {
                    // load selection to player1_save
                    *pkmn_save_player1 = load_savefile_from_path(save_file_data->saves_file_path[selected_saves_index[0]]);

                    // save the selected path name
                    strcpy(player1_save_path, save_file_data->saves_file_path[selected_saves_index[0]]);
                    // generate trainer info from save
                    create_trainer(pkmn_save_player1, trainer1);
                    // save trainer id to trainerSelection
                    trainerSelection[0].trainer_id = trainer1->trainer_id;

                    // load selection to player2_save
                    *pkmn_save_player2 = load_savefile_from_path(save_file_data->saves_file_path[selected_saves_index[1]]);
                    strcpy(player2_save_path, save_file_data->saves_file_path[selected_saves_index[1]]);
                    create_trainer(pkmn_save_player2, trainer2);
                    trainerSelection[1].trainer_id = trainer2->trainer_id;

                    if (pkmn_save_player1->save_generation_type == SAVE_GENERATION_CORRUPTED || pkmn_save_player2->save_generation_type == SAVE_GENERATION_CORRUPTED)
                    {
                        has_file_error = true;
                    }
                    else
                    {
                        *is_same_generation = pkmn_save_player1->save_generation_type == pkmn_save_player2->save_generation_type;
                        *current_screen = SCREEN_TRADE;
                        selected_saves_index[0] = -1;
                        selected_saves_index[1] = -1;
                        has_file_error = false;
                    }
                }
            }
            if (has_file_error)
            {
                DrawText("save file invalid", NEXT_BUTTON_X - 15, NEXT_BUTTON_Y + 25, 15, RED);
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
        }
    }

    // change directory button between back and next buttons in line horizontally centered vertically
    int button_width = MeasureText("Change Save Directory", 20) + 20;
    DrawText("Change Save Directory", SCREEN_WIDTH / 2 - button_width / 2, BACK_BUTTON_Y, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){SCREEN_WIDTH / 2 - button_width / 2 - 10, BACK_BUTTON_Y - 30, button_width, BUTTON_HEIGHT}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            *current_screen = SCREEN_FILE_EDIT;
            selected_saves_index[0] = -1;
            selected_saves_index[1] = -1;
            trainer1->trainer_id = 0;
            trainer2->trainer_id = 0;
        }
    }

    EndDrawing();
}
