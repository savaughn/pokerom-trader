#include "raylibhelper.h"
#include "pksavfilehelper.h"

#define new_max(x, y) (((x) >= (y)) ? (x) : (y))
#define new_min(x, y) (((x) <= (y)) ? (x) : (y))

static PokemonSave pkmn_saves[MAX_FILE_PATH_COUNT] = {
    [0 ... MAX_FILE_PATH_COUNT - 1] = {
        .save_generation_type = SAVE_GENERATION_NONE,
    }};

void DrawSaveFileContainer(PokemonSave *pkmn_save, char *save_name, Rectangle container_rec, bool is_selected)
{
    char trainer_name[7] = "\0";
    uint16_t trainer_id;
    uint16_t hours = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;

    if (pkmn_save->save_generation_type == SAVE_GENERATION_1)
    {
        pksav_gen1_import_text(pkmn_save->save.gen1_save.trainer_info.p_name, trainer_name, 7);
        trainer_id = pksav_bigendian16(*pkmn_save->save.gen1_save.trainer_info.p_id);
        struct pksav_gen1_time *save_time = pkmn_save->save.gen1_save.p_time_played;
        hours = pksav_littleendian16(save_time->hours);
        minutes = save_time->minutes;
        seconds = save_time->seconds;
    }
    else if (pkmn_save->save_generation_type == SAVE_GENERATION_2)
    {
        pksav_gen2_import_text(pkmn_save->save.gen2_save.trainer_info.p_name, trainer_name, 7);
        trainer_id = pksav_bigendian16(*pkmn_save->save.gen2_save.trainer_info.p_id);
        struct pksav_gen2_time *save_time = pkmn_save->save.gen2_save.save_time.p_time_played;
        hours = save_time->hours;
        minutes = save_time->minutes;
        seconds = save_time->seconds;
    }
    else
    {
        return;
    }

    DrawRectangleRec(container_rec, (Color){62, 60, 153, is_selected ? 150 : 255});

    // draw trainer info box
    DrawRectangle(container_rec.x, container_rec.y, 125 + 5, container_rec.height, (Color){100, 101, 193, is_selected ? 150 : 255});
    DrawRectangleLines(container_rec.x, container_rec.y, 125 + 5, container_rec.height, BLACK);

    // draw trainer name
    DrawText(trainer_name, container_rec.x + 6, container_rec.y + 6, 20, BLACK);
    DrawText(trainer_name, container_rec.x + 5, container_rec.y + 5, 20, WHITE);

    // draw trainer id
    DrawText(TextFormat("ID No.%05u", trainer_id), container_rec.x + 6, container_rec.y + 6 + 25, 20, BLACK);
    DrawText(TextFormat("ID No.%05u", trainer_id), container_rec.x + 5, container_rec.y + 5 + 25, 20, WHITE);

    DrawText(TextFormat("%dh%02dm%02ds", hours, minutes, seconds), container_rec.x + 6, container_rec.y + 6 + 50, 20, BLACK);
    DrawText(TextFormat("%dh%02dm%02ds", hours, minutes, seconds), container_rec.x + 5, container_rec.y + 5 + 50, 20, WHITE);

    // pokemon party grid
    DrawLine(container_rec.x + 130, container_rec.y + (container_rec.height - 30) / 2, container_rec.x + container_rec.width, container_rec.y + (container_rec.height - 30) / 2, BLACK);
    DrawLine(container_rec.x + 130 + (container_rec.width - 135) / 3, container_rec.y, container_rec.x + 130 + (container_rec.width - 135) / 3, container_rec.y + container_rec.height - 30, BLACK);
    DrawLine(container_rec.x + 130 + (container_rec.width - 135) / 3 * 2, container_rec.y, container_rec.x + 130 + (container_rec.width - 135) / 3 * 2, container_rec.y + container_rec.height - 30, BLACK);

    Rectangle name_slots[6] = {
        (Rectangle){container_rec.x + 135, container_rec.y + 5, (container_rec.width - 135) / 3, (container_rec.height - 30) / 2},
        (Rectangle){container_rec.x + 135 + (container_rec.width - 135) / 3, container_rec.y + 5, (container_rec.width - 135) / 3, (container_rec.height - 30) / 2},
        (Rectangle){container_rec.x + 135 + (container_rec.width - 135) / 3 * 2, container_rec.y + 5, (container_rec.width - 135) / 3, (container_rec.height - 30) / 2},
        (Rectangle){container_rec.x + 135, container_rec.y + 5 + (container_rec.height - 30) / 2, (container_rec.width - 135) / 3, (container_rec.height - 30) / 2},
        (Rectangle){container_rec.x + 135 + (container_rec.width - 135) / 3, container_rec.y + 5 + (container_rec.height - 30) / 2, (container_rec.width - 135) / 3, (container_rec.height - 30) / 2},
        (Rectangle){container_rec.x + 135 + (container_rec.width - 135) / 3 * 2, container_rec.y + 5 + (container_rec.height - 30) / 2, (container_rec.width - 135) / 3, (container_rec.height - 30) / 2},
    };

    // draw pokemon party
    if (pkmn_save->save_generation_type == SAVE_GENERATION_1)
    {
        for (int i = 0; i < pkmn_save->save.gen1_save.pokemon_storage.p_party->count; i++)
        {
            char pokemon_name[11];
            pksav_gen1_import_text(pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[i], pokemon_name, 10);
            DrawText(pokemon_name, name_slots[i].x + 1, name_slots[i].y + 1, 20, BLACK);
            DrawText(pokemon_name, name_slots[i].x, name_slots[i].y, 20, WHITE);
            DrawText(TextFormat(" L%d", pkmn_save->save.gen1_save.pokemon_storage.p_party->party->party_data.level), (name_slots[i].x + ((container_rec.width - 135) / 3)) - 60 + 1, name_slots[i].y + 1, 20, BLACK);
            DrawText(TextFormat(" L%d", pkmn_save->save.gen1_save.pokemon_storage.p_party->party->party_data.level), (name_slots[i].x + ((container_rec.width - 135) / 3)) - 60, name_slots[i].y, 20, WHITE);
        }
    }
    else if (pkmn_save->save_generation_type == SAVE_GENERATION_2)
    {
        for (int i = 0; i < pkmn_save->save.gen2_save.pokemon_storage.p_party->count; i++)
        {
            char pokemon_name[11] = "\0";
            pksav_gen2_import_text(pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[i], pokemon_name, 10);
            DrawText(pokemon_name, name_slots[i].x + 1, name_slots[i].y + 1, 20, BLACK);
            DrawText(pokemon_name, name_slots[i].x, name_slots[i].y, 20, WHITE);
            DrawText(TextFormat(" L%d", pkmn_save->save.gen2_save.pokemon_storage.p_party->party[i].pc_data.level), (name_slots[i].x + ((container_rec.width - 135) / 3)) - 60 + 1, name_slots[i].y + 1, 20, BLACK);
            DrawText(TextFormat(" L%d", pkmn_save->save.gen2_save.pokemon_storage.p_party->party[i].pc_data.level), (name_slots[i].x + ((container_rec.width - 135) / 3)) - 60, name_slots[i].y, 20, WHITE);
        }
    }
    else
    {
        return;
    }

    // draw save name
    DrawLine(container_rec.x + 130, container_rec.y + container_rec.height - 30, container_rec.width + container_rec.x, container_rec.y + container_rec.height - 30, BLACK);
    DrawText(save_name, (container_rec.width - 135) / 2 + container_rec.x + 135 - MeasureText(save_name, 20) / 2 + 1, container_rec.y + container_rec.height - 25 + 1, 20, BLACK);
    DrawText(save_name, (container_rec.width - 135) / 2 + container_rec.x + 135 - MeasureText(save_name, 20) / 2, container_rec.y + container_rec.height - 25, 20, WHITE);

    // draw border
    DrawRectangleLinesEx((Rectangle){container_rec.x - 4, container_rec.y - 4, container_rec.width + 8, container_rec.height + 8}, 1, BLACK);
    DrawRectangleLinesEx((Rectangle){container_rec.x - 3, container_rec.y - 3, container_rec.width + 6, container_rec.height + 6}, 3, WHITE);
    DrawRectangleLinesEx((Rectangle){container_rec.x, container_rec.y, container_rec.width, container_rec.height}, 1, BLACK);

    if (is_selected)
    {
        DrawRectangleGradientH(container_rec.x, container_rec.y, 200, container_rec.height, (Color){0, 0, 0, 175}, (Color){0, 0, 0, 0});
        DrawRectangleGradientH(container_rec.x + container_rec.width - 200, container_rec.y, 200, container_rec.height, (Color){0, 0, 0, 0}, (Color){0, 0, 0, 175});
    }
}

void draw_corrupted_save(const char *save_name, const int y_offset, const int index, const int corrupted_count)
{
    DrawRectangle(30 + (SCREEN_WIDTH - 50) / 12, y_offset + (93 * index) - (60 * corrupted_count) - 2, SCREEN_WIDTH - 60 - (2 * (SCREEN_WIDTH - 50) / 12), 25, (Color){255, 255, 255, 125});
    DrawRectangleGradientH(30, y_offset + (93 * index) - (60 * corrupted_count) - 2, (SCREEN_WIDTH - 50) / 12, 25, (Color){255, 255, 255, 0}, (Color){255, 255, 255, 125});
    DrawRectangleGradientH(SCREEN_WIDTH - 31 - (SCREEN_WIDTH - 50) / 12, y_offset + (93 * index) - (60 * corrupted_count) - 2, (SCREEN_WIDTH - 50) / 12, 25, (Color){255, 255, 255, 125}, (Color){255, 255, 255, 0});
    DrawText(save_name, 100, y_offset + (93 * index) - (60 * corrupted_count) + 2, 20, DARKGRAY);
    DrawText("save file invalid", 100 + MeasureText(save_name, 20) + 10, y_offset + (93 * index) - (60 * corrupted_count), 15, BLACK);
}

void draw_no_save_files(char *save_path)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (no_dir_err)
    {
        Vector2 text_center = SCREEN_CENTER("Save folder doesn't exist!", 20);
        DrawText("Save folder doesn't exist!", text_center.x, text_center.y, 20, BLACK);
    }
    else
    {
        DrawText("No save files found in save folder", 190, 200, 20, BLACK);
    }
    DrawText(TextFormat("%s", save_path), SCREEN_CENTER(save_path, 20).x, 275, 20, BLACK);
    EndDrawing();
}

void load_display_files(struct SaveFileData *save_file_data, PokemonSave *pkmn_saves)
{
    // Load save files once
    for (int i = 0; i < save_file_data->num_saves; i++)
    {
        // if not initialized
        if (pkmn_saves[i].save_generation_type == SAVE_GENERATION_NONE)
        {
            pkmn_saves[i] = load_savefile_from_path(save_file_data->saves_file_path[i]);
        }
    }
}

void draw_pokeball_scroll(float scroll_position, float transparency)
{
    DrawCircleGradient(SCREEN_WIDTH - 10, 75 + (scroll_position * (SCREEN_HEIGHT - 150)), 10, (Color){0, 0, 0, transparency}, (Color){0, 0, 0, 0});
    DrawCircle(SCREEN_WIDTH - 10, 75 + (scroll_position * (SCREEN_HEIGHT - 150)), 5, (Color){255, 255, 255, transparency});
    DrawCircleSector((Vector2){SCREEN_WIDTH - 10, 74 + (scroll_position * (SCREEN_HEIGHT - 150))}, 5, 0, -180, 6, (Color){230, 41, 55, transparency});
    DrawCircle(SCREEN_WIDTH - 10, 75 + (scroll_position * (SCREEN_HEIGHT - 150)), 2, (Color){0, 0, 0, transparency});
    DrawLine(SCREEN_WIDTH - 10 - 5, 75 + (scroll_position * (SCREEN_HEIGHT - 150)), SCREEN_WIDTH - 10 + 5, 75 + (scroll_position * (SCREEN_HEIGHT - 150)), (Color){0, 0, 0, transparency});
    DrawCircle(SCREEN_WIDTH - 10, 75 + (scroll_position * (SCREEN_HEIGHT - 150)), 1, (Color){255, 255, 255, transparency});
}

void handle_list_scroll(int *y_offset, const int num_saves, const int corrupted_count, int *mouses_down_index, bool *is_moving_scroll, int *banner_position_offset)
{
    const uint8_t box_height = 93;
    const uint8_t num_visible = 4;
    const int height = num_saves * box_height - 60 * corrupted_count;

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN))
    {
        float mouse_delta = GetMouseDelta().y;

        if (mouse_delta >= 0.5 || mouse_delta <= -0.5)
        {
            *y_offset += GetMouseDelta().y;
            *y_offset = *y_offset < -height + (num_visible * box_height) + (corrupted_count * 25) ? -height + (num_visible * box_height) + (corrupted_count * 25) : *y_offset;
            *y_offset = *y_offset > 75 ? 75 : *y_offset;
            *mouses_down_index = -1;
            *is_moving_scroll = true;
            if (*y_offset < 50)
            {
                *banner_position_offset = *y_offset - 50;
            }
            else
            {
                *banner_position_offset = 0;
            }
        }
        else if (IsKeyDown(KEY_UP))
        {
            *y_offset += 1;
            *mouses_down_index = -1;
            *is_moving_scroll = true;
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            *y_offset -= 1;
            *mouses_down_index = -1;
            *is_moving_scroll = true;
        }
    }

    const int min_y = (height + (num_visible * box_height) + (corrupted_count * 25)) - 75;
    const int max_y = min_y + 75 - (-height + (num_visible * box_height) + (corrupted_count * 25));
    const uint8_t t_max = 50;
    const uint8_t t_min = 0;
    const uint8_t t_rate = 5;
    static uint8_t transparency = 0;

    // scroll position indicator
    float scroll_position = min_y + 75 - *y_offset;
    scroll_position = (scroll_position - min_y) / (max_y - min_y);
    scroll_position = scroll_position < 0 ? 0 : scroll_position;
    scroll_position = scroll_position > 1 ? 1 : scroll_position;
    if (*is_moving_scroll)
    {
        transparency += t_rate;
        if (transparency > t_max)
        {
            transparency = t_max;
        }
    }
    else if (transparency > t_min)
    {
        transparency -= t_rate;
        if (transparency < t_min)
        {
            transparency = t_min;
        }
    }
    if (height > SCREEN_HEIGHT - 100 - 2 * (SCREEN_HEIGHT / 16))
    {
        DrawRectangleGradientV(SCREEN_WIDTH - 20, 50, 20, SCREEN_HEIGHT / 16, (Color){255, 255, 255, 0}, (Color){255, 255, 255, transparency});
        DrawRectangle(SCREEN_WIDTH - 20, 50 + SCREEN_HEIGHT / 16, 20, SCREEN_HEIGHT - 100 - 2 * (SCREEN_HEIGHT / 16), (Color){255, 255, 255, transparency});
        DrawRectangleGradientV(SCREEN_WIDTH - 20, SCREEN_HEIGHT - 50 - SCREEN_HEIGHT / 16, 20, SCREEN_HEIGHT / 16, (Color){255, 255, 255, transparency}, (Color){255, 255, 255, 0});
        draw_pokeball_scroll(scroll_position, (float)transparency * 5.1f);
    }
}

void update_selected_indexes_with_selection(int *selected_saves_index, int *mouses_down_index, bool *is_moving_scroll)
{
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        if (*mouses_down_index != -1 && !*is_moving_scroll)
        {
            if (selected_saves_index[0] == *mouses_down_index)
            {
                selected_saves_index[0] = -1;
            }
            else if (selected_saves_index[1] == *mouses_down_index)
            {
                selected_saves_index[1] = -1;
            }
            else if (selected_saves_index[0] == -1)
            {
                selected_saves_index[0] = *mouses_down_index;
            }
            else if (selected_saves_index[1] == -1)
            {
                selected_saves_index[1] = *mouses_down_index;
            }
        }
        *mouses_down_index = -1;
        *is_moving_scroll = false;
    }
}

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

    BeginDrawing();
    ClearBackground(COLOR_PKMN_RED);

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
                DrawSaveFileContainer(&pkmn_saves[i], save_name, save_file_rec, (selected_saves_index[0] == i || selected_saves_index[1] == i));
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
        int text_width = MeasureText("Select two save files to trade between", 20);
        DrawRectangle(0, banner_position_offset - 10, SCREEN_WIDTH, 50, WHITE);
        DrawLineEx((Vector2){0, banner_position_offset + 45}, (Vector2){SCREEN_WIDTH, banner_position_offset + 45}, 15, BLACK);
        DrawText("Select two save files to trade between", SCREEN_WIDTH / 2 - text_width / 2, 10 + banner_position_offset, 20, BLACK);

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
                *is_same_generation = pkmn_save_player1->save_generation_type == pkmn_save_player2->save_generation_type;
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

    EndDrawing();
}
