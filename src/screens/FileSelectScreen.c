#include "raylibhelper.h"
#include "pksavfilehelper.h"

static PokemonSave pkmn_saves[MAX_FILE_PATH_COUNT] = {
    [0 ... MAX_FILE_PATH_COUNT - 1] = {
        .save_generation_type = SAVE_GENERATION_NONE,
    }};

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
    bool has_selected_two_saves = selected_saves_index[0] != -1 && selected_saves_index[1] != -1;
    const Rectangle bottom_bar_rec = (Rectangle){0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, 100};
    const Rectangle trade_button_rec = (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y + 8, BUTTON_WIDTH, BUTTON_HEIGHT};
    static enum ui_selections {
        E_UI_NONE = -1,
        E_UI_BACK,
        E_UI_CHANGE_DIR,
        E_UI_TRADE
    } ui_selection = E_UI_NONE;
    static bool is_moving_scroll = false;
    static int y_offset = 75;
    static int banner_position_offset = 0;
    static bool show_duplicate_toast = false;

    BeginDrawing();
    ClearBackground(RED);

    draw_background_grid();

    if (save_file_data->num_saves == 0)
    {
        draw_no_save_files(save_file_data->save_dir);
    }
    else
    {
        int corrupted_count = 0;
        static int mouses_down_index = -1;

        // Load save files once
        load_display_files(save_file_data, pkmn_saves);

        // Update and draw save files
        for (int i = 0; i < save_file_data->num_saves; i++)
        {
            bool is_corrupted = pkmn_saves[i].save_generation_type == SAVE_GENERATION_CORRUPTED;
            const Rectangle save_file_rec = (Rectangle){SCREEN_WIDTH / 2 - (SCREEN_WIDTH - 50) / 2, y_offset + (93 * i) - (60 * corrupted_count), SCREEN_WIDTH - 50, 80};

            // Update selected save files index
            update_selected_indexes_with_selection(selected_saves_index, &mouses_down_index, &is_moving_scroll);

            // Extract save file name from path
            char *save_name = strrchr(save_file_data->saves_file_path[i], '/') + 1;

            // Draw save file container
            if (is_corrupted)
            {
                draw_corrupted_save(save_name, y_offset, i, corrupted_count);
                corrupted_count++;
            }
            else
            {
                draw_save_file_container(&pkmn_saves[i], save_name, save_file_rec, (selected_saves_index[0] == i || selected_saves_index[1] == i));
            }

            // If we are not hovering over the bottom bar
            if (!CheckCollisionPointRec(GetMousePosition(), bottom_bar_rec))
            {
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !is_moving_scroll)
                {
                    // Select a save file
                    if (CheckCollisionPointRec(GetMousePosition(), save_file_rec) && !is_corrupted)
                    {
                        mouses_down_index = i;
                        pkmn_save_player1->save_generation_type = SAVE_GENERATION_NONE;
                        pkmn_save_player2->save_generation_type = SAVE_GENERATION_NONE;
                    }
                }
            }
        }

        handle_list_scroll(&y_offset, save_file_data->num_saves, corrupted_count, &mouses_down_index, &is_moving_scroll, &banner_position_offset);

        // Top Banner
        draw_top_banner("Select two save files to trade between", &banner_position_offset);

        // Bottom bar
        DrawRectangleRec(bottom_bar_rec, WHITE);
        DrawLineEx((Vector2){bottom_bar_rec.x, bottom_bar_rec.y}, (Vector2){bottom_bar_rec.width, bottom_bar_rec.y}, 15, BLACK);
        DrawText("Trade >", trade_button_rec.x + 15, trade_button_rec.y + 10, 20, has_selected_two_saves ? ui_selection == E_UI_TRADE ? LIGHTGRAY : BLACK : LIGHTGRAY);
    }

    // Back button
    const Rectangle back_button_rec = (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y + 8, BUTTON_WIDTH, BUTTON_HEIGHT};
    DrawText("< Back", back_button_rec.x + 15, back_button_rec.y + 10, 20, ui_selection == E_UI_BACK ? LIGHTGRAY : BLACK);

    // change directory button
    int button_width = MeasureText("Change Save Directory", 20) + 20;
    const Rectangle change_dir_button = (Rectangle){SCREEN_WIDTH / 2 - button_width / 2, BACK_BUTTON_Y + 8, button_width, BUTTON_HEIGHT};
    DrawText("Change Save Directory", change_dir_button.x + 10, change_dir_button.y + 10, 20, ui_selection == E_UI_CHANGE_DIR ? LIGHTGRAY : BLACK);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !is_moving_scroll)
    {
        if (has_selected_two_saves && CheckCollisionPointRec(GetMousePosition(), trade_button_rec))
        {
            ui_selection = E_UI_TRADE;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), back_button_rec))
        {
            ui_selection = E_UI_BACK;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), change_dir_button))
        {
            ui_selection = E_UI_CHANGE_DIR;
        }
        else
        {
            ui_selection = E_UI_NONE;
        }
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && ui_selection != E_UI_NONE)
    {
        switch (ui_selection)
        {
        case E_UI_TRADE:
            if (CheckCollisionPointRec(GetMousePosition(), trade_button_rec))
            {
                static bool is_duplicate_save_file = false;

                // load selection to player1_save
                *pkmn_save_player1 = pkmn_saves[selected_saves_index[0]];

                // save the selected path name
                strcpy(player1_save_path, save_file_data->saves_file_path[selected_saves_index[0]]);
                // generate trainer info from save
                create_trainer(pkmn_save_player1, trainer1);
                // save trainer id to trainerSelection
                trainerSelection[0].trainer_id = trainer1->trainer_id;

                // load selection to player2_save
                *pkmn_save_player2 = pkmn_saves[selected_saves_index[1]];
                strcpy(player2_save_path, save_file_data->saves_file_path[selected_saves_index[1]]);
                create_trainer(pkmn_save_player2, trainer2);
                trainerSelection[1].trainer_id = trainer2->trainer_id;
                is_duplicate_save_file = trainer1->trainer_id == trainer2->trainer_id;

                *is_same_generation = pkmn_save_player1->save_generation_type == pkmn_save_player2->save_generation_type;
                if (!is_duplicate_save_file)
                {
                    *current_screen = SCREEN_TRADE;
                    selected_saves_index[0] = -1;
                    selected_saves_index[1] = -1;
                    // reset save files
                    for (int i = 0; i < MAX_FILE_PATH_COUNT; i++)
                    {
                        pkmn_saves[i].save_generation_type = SAVE_GENERATION_NONE;
                    }
                    ui_selection = E_UI_NONE;
                    y_offset = 75;
                    banner_position_offset = 0;

                    reset_toast_message();
                    show_duplicate_toast = false;
                }
                else
                {
                    show_duplicate_toast = true;
                }
            }
            else
            {
                ui_selection = E_UI_NONE;
            }
            break;
        case E_UI_BACK:
            if (CheckCollisionPointRec(GetMousePosition(), back_button_rec))
            {
                *current_screen = SCREEN_MAIN_MENU;
                selected_saves_index[0] = -1;
                selected_saves_index[1] = -1;
                trainer1->trainer_id = 0;
                trainer2->trainer_id = 0;

                reset_toast_message();
                show_duplicate_toast = false;

                // reset save files
                for (int i = 0; i < MAX_FILE_PATH_COUNT; i++)
                {
                    pkmn_saves[i].save_generation_type = SAVE_GENERATION_NONE;
                }
                ui_selection = E_UI_NONE;
                y_offset = 75;
                banner_position_offset = 0;
            }
            else
            {
                ui_selection = E_UI_NONE;
            }
            break;
        case E_UI_CHANGE_DIR:
            if (CheckCollisionPointRec(GetMousePosition(), change_dir_button))
            {
                *current_screen = SCREEN_FILE_EDIT;
                selected_saves_index[0] = -1;
                selected_saves_index[1] = -1;
                trainer1->trainer_id = 0;
                trainer2->trainer_id = 0;

                // reset save files
                for (int i = 0; i < MAX_FILE_PATH_COUNT; i++)
                {
                    pkmn_saves[i].save_generation_type = SAVE_GENERATION_NONE;
                }
                ui_selection = E_UI_NONE;
                y_offset = 75;
                banner_position_offset = 0;
            }
            else
            {
                ui_selection = E_UI_NONE;
            }
            break;
        default:
            break;
        }
    }

    if (show_duplicate_toast)
    {
        show_duplicate_toast = !draw_toast_message("Duplicate save files cannot trade with each other!", TOAST_LONG, TOAST_ERROR);
    }

    EndDrawing();
}
