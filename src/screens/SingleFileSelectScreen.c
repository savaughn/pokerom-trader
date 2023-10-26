#include "raylibhelper.h"
#include "pksavfilehelper.h"
#include "filehelper.h"

void draw_file_select_single(struct SaveFileData *save_file_data, PokemonSave *save_player1, char *player1_save_path, struct TrainerInfo *trainer1, struct TrainerSelection *trainerSelection, enum single_player_menu_types menu_type, GameScreen *current_screen)
{

    BeginDrawing();
    ClearBackground(RAYWHITE);

    // select a save file
    static int selected_saves_index = -1;
    bool hasSelectedSave = selected_saves_index != -1 && save_player1->save_generation_type != SAVE_GENERATION_CORRUPTED;

    if (save_file_data->num_saves == 0)
    {
        get_save_files(save_file_data);
        char *save_dir = save_file_data->save_dir;
        DrawText("No save files found in save folder", SCREEN_WIDTH_TEXT_CENTER("No save files found in save folder", 20), 200, 20, BLACK);
        DrawText(TextFormat("%s/", save_dir), SCREEN_WIDTH_TEXT_CENTER(save_dir, 20), 250, 20, BLACK);
    }
    if (menu_type == SINGLE_PLAYER_MENU_TYPE_BILLS_PC)
        DrawText("Select a save file to access Bill's PC", 190, 25, 20, BLACK);
    if (menu_type == SINGLE_PLAYER_MENU_TYPE_EVOLVE)
        DrawText("Select a save file to access your party", 190, 25, 20, BLACK);
    for (int i = 0; i < save_file_data->num_saves; i++)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){100, 75 + 25 * i, SCREEN_WIDTH / 2, 25}))
            {
                if (selected_saves_index == i)
                {
                    selected_saves_index = -1;
                }
                else
                {
                    selected_saves_index = i;
                }
                // Reset when selecting a file after seeing error message
                save_player1->save_generation_type = SAVE_GENERATION_NONE;
            }
        }
        char *save_name = strrchr(save_file_data->saves_file_path[i], '/');
        save_name++;

        DrawText(save_name, 100, 75 + 25 * i, 20, (selected_saves_index == i) ? LIGHTGRAY : BLACK);
        if (menu_type == SINGLE_PLAYER_MENU_TYPE_BILLS_PC)
            DrawText("Open Bill's PC >", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, hasSelectedSave ? BLACK : LIGHTGRAY);
        if (menu_type == SINGLE_PLAYER_MENU_TYPE_EVOLVE)
            DrawText("Open Party >", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, hasSelectedSave ? BLACK : LIGHTGRAY);

        // On selected file next button press
        if (hasSelectedSave)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
                {
                    // load selection to player1_save
                    *save_player1 = load_savefile_from_path(save_file_data->saves_file_path[selected_saves_index]);
                    // save the selected path name
                    strcpy(player1_save_path, save_file_data->saves_file_path[selected_saves_index]);
                    // generate trainer info from save
                    create_trainer(save_player1, trainer1);
                    // save trainer id to trainerSelection
                    trainerSelection->trainer_id = trainer1->trainer_id;

                    if (menu_type == SINGLE_PLAYER_MENU_TYPE_BILLS_PC)
                        *current_screen = SCREEN_BILLS_PC;
                    if (menu_type == SINGLE_PLAYER_MENU_TYPE_EVOLVE && save_player1->save_generation_type != SAVE_GENERATION_CORRUPTED)
                        *current_screen = SCREEN_EVOLVE;
                }
            }
        }
    }
    if (save_player1->save_generation_type == SAVE_GENERATION_CORRUPTED)
    {
        DrawText("save file invalid", NEXT_BUTTON_X + 5, NEXT_BUTTON_Y + 25, 15, RED);
    }
    if (menu_type == SINGLE_PLAYER_MENU_TYPE_BILLS_PC)
        DrawText("Bill's PC", 25, 25, 20, BLACK);
    if (menu_type == SINGLE_PLAYER_MENU_TYPE_EVOLVE)
        DrawText("Trade Evolve", 25, 25, 20, BLACK);
    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            *current_screen = SCREEN_MAIN_MENU;
            selected_saves_index = -1;
        }
    }

    // change directory button
    int button_width = MeasureText("Change Save Directory", 20) + 20;
    DrawText("Change Save Directory", SCREEN_WIDTH / 2 - button_width / 2, BACK_BUTTON_Y, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){SCREEN_WIDTH / 2 - button_width / 2 - 10, BACK_BUTTON_Y - 30, button_width, BUTTON_HEIGHT}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            trainer1->trainer_id = 0;
            trainerSelection->pkmn_party_index = -1;
            *current_screen = SCREEN_FILE_EDIT;
        }
    }

    EndDrawing();
    if (*current_screen != SCREEN_EVOLVE_FILE_SELECT && *current_screen != SCREEN_BILLS_PC_FILE_SELECT)
        selected_saves_index = -1;
}
