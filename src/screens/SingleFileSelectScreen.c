#include "raylibhelper.h"
#include "pksavfilehelper.h"
#include "filehelper.h"

static PokemonSave pkmn_saves[MAX_FILE_PATH_COUNT] = {
    [0 ... MAX_FILE_PATH_COUNT - 1] = {
        .save_generation_type = SAVE_GENERATION_NONE,
    }};

void draw_file_select_single(struct save_file_data *save_file_data, PokemonSave *pkmn_save, char *player_save_path, struct trainer_info *trainer, struct TrainerSelection *trainerSelection, enum single_player_menu_types menu_type, GameScreen *current_screen)
{
    static int selected_saves_index = -1;
    bool has_selected_save = selected_saves_index != -1;
    const Rectangle bottom_bar_rec = (Rectangle){0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, 100};
    const Rectangle evolve_button_rec = (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y + 8, BUTTON_WIDTH, BUTTON_HEIGHT};
    static enum ui_selections {
        E_UI_NONE = -1,
        E_UI_BACK,
        E_UI_CHANGE_DIR,
        E_UI_EVOLVE
    } ui_selection = E_UI_NONE;
    static bool is_moving_scroll = false;
    static int y_offset = 75;
    static int banner_position_offset = 0;

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

        // Load save files for selection display
        load_display_files(save_file_data, pkmn_saves);

        // Update and draw save files
        for (int i = 0; i < save_file_data->num_saves; i++)
        {
            bool is_corrupted = pkmn_saves[i].save_generation_type == SAVE_GENERATION_CORRUPTED;
            const Rectangle save_file_rec = (Rectangle){SCREEN_WIDTH / 2 - (SCREEN_WIDTH - 50) / 2, y_offset + (93 * i) - (60 * corrupted_count), SCREEN_WIDTH - 50, 80};

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                if (mouses_down_index != -1 && !is_moving_scroll)
                {
                    if (selected_saves_index == mouses_down_index)
                    {
                        selected_saves_index = -1;
                    }
                    else
                    {
                        selected_saves_index = mouses_down_index;
                    }
                    // Reset when selecting a file after seeing error message
                    pkmn_save->save_generation_type = SAVE_GENERATION_NONE;
                }
                mouses_down_index = -1;
                is_moving_scroll = false;
            }

            char *save_name = strrchr(save_file_data->saves_file_path[i], '/') + 1;

            if (is_corrupted)
            {
                draw_corrupted_save(save_name, y_offset, i, corrupted_count);
                corrupted_count++;
            }
            else
            {
                draw_save_file_container(&pkmn_saves[i], save_name, save_file_rec, (selected_saves_index == i));
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
                        pkmn_save->save_generation_type = SAVE_GENERATION_NONE;
                    }
                }
            }
        }

        handle_list_scroll(&y_offset, save_file_data->num_saves, corrupted_count, &mouses_down_index, &is_moving_scroll, &banner_position_offset);

        draw_top_banner("Select a save file to access your party", &banner_position_offset);
        // Bottom bar
        DrawRectangleRec(bottom_bar_rec, WHITE);
        DrawLineEx((Vector2){bottom_bar_rec.x, bottom_bar_rec.y}, (Vector2){bottom_bar_rec.width, bottom_bar_rec.y}, 15, BLACK);
        DrawText("Evolve >", evolve_button_rec.x + 15, evolve_button_rec.y + 10, 20, has_selected_save ? ui_selection == E_UI_EVOLVE ? LIGHTGRAY : BLACK : LIGHTGRAY);
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
        if (has_selected_save && CheckCollisionPointRec(GetMousePosition(), evolve_button_rec))
        {
            ui_selection = E_UI_EVOLVE;
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

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && ui_selection != E_UI_NONE)
    {
        switch (ui_selection)
        {
        case E_UI_BACK:
            if (CheckCollisionPointRec(GetMousePosition(), back_button_rec))
            {
                *current_screen = SCREEN_MAIN_MENU;
                selected_saves_index = -1;
            }
            ui_selection = E_UI_NONE;
            break;
        case E_UI_CHANGE_DIR:
            if (CheckCollisionPointRec(GetMousePosition(), change_dir_button))
            {
                trainer->trainer_id = 0;
                trainerSelection->pkmn_party_index = -1;
                *current_screen = SCREEN_FILE_EDIT;
                selected_saves_index = -1;
            }
            ui_selection = E_UI_NONE;
            break;
        case E_UI_EVOLVE:
            if (CheckCollisionPointRec(GetMousePosition(), evolve_button_rec))
            {
                // load selection to player1_save
                *pkmn_save = pkmn_saves[selected_saves_index];
                // save the selected path name
                strcpy(player_save_path, save_file_data->saves_file_path[selected_saves_index]);
                // generate trainer info from save
                create_trainer(pkmn_save, trainer);
                // save trainer id to trainerSelection
                trainerSelection->trainer_id = trainer->trainer_id;

                if (menu_type == SINGLE_PLAYER_MENU_TYPE_BILLS_PC)
                    *current_screen = SCREEN_BILLS_PC;
                if (menu_type == SINGLE_PLAYER_MENU_TYPE_EVOLVE && pkmn_save->save_generation_type != SAVE_GENERATION_CORRUPTED)
                    *current_screen = SCREEN_EVOLVE;

                // reset save files
                for (int i = 0; i < MAX_FILE_PATH_COUNT; i++)
                {
                    pkmn_saves[i].save_generation_type = SAVE_GENERATION_NONE;
                }
                ui_selection = E_UI_NONE;
                y_offset = 75;
                banner_position_offset = 0;
                selected_saves_index = -1;
            }
            ui_selection = E_UI_NONE;
            break;
        default:
            break;
        }
    }

    EndDrawing();
}
