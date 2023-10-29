#include "raylibhelper.h"
#include "pksavfilehelper.h"

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

    if (is_selected) {
        DrawRectangleGradientH(container_rec.x, container_rec.y, 200, container_rec.height, (Color){0, 0, 0, 175}, (Color){0, 0, 0, 0});
        DrawRectangleGradientH(container_rec.x + container_rec.width - 200, container_rec.y, 200, container_rec.height, (Color){0, 0, 0, 0}, (Color){0, 0, 0, 175});
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

    BeginDrawing();
    ClearBackground(COLOR_PKMN_RED);

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
        int corrupted_count = 0;
        static int y_offset = 75;
        static int banner_position_offset = 0;

        // Load save files once
        for (int i = 0; i < save_file_data->num_saves; i++)
        {
            // if not initialized
            if (pkmn_saves[i].save_generation_type == SAVE_GENERATION_NONE)
            {
                PokemonSave pkmn_save_temp = load_savefile_from_path(save_file_data->saves_file_path[i]);
                pkmn_saves[i] = load_savefile_from_path(save_file_data->saves_file_path[i]);
            }
        }

        for (int i = 0; i < save_file_data->num_saves; i++)
        {
            static int mouses_down_index = -1;
            static bool is_moving_scroll = false;
            bool is_corrupted = pkmn_saves[i].save_generation_type == SAVE_GENERATION_CORRUPTED;
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !is_moving_scroll)
            {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){SCREEN_WIDTH / 2 - (SCREEN_WIDTH - 50) / 2, y_offset + (93 * i) - (60 * corrupted_count), SCREEN_WIDTH - 50, 80}) && !is_corrupted)
                {
                    mouses_down_index = i;

                    pkmn_save_player1->save_generation_type = SAVE_GENERATION_NONE;
                    pkmn_save_player2->save_generation_type = SAVE_GENERATION_NONE;
                    has_file_error = false;
                }
            }

            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN))
            {
                float mouse_delta = GetMouseDelta().y;
                int height = save_file_data->num_saves * 93 - 60 * corrupted_count;
                if (mouse_delta >= 0.5 || mouse_delta <= -0.5)
                {
                    y_offset += GetMouseDelta().y * 0.1;
                    y_offset = y_offset < -height * 0.25 ? -height * 0.25 : y_offset;
                    y_offset = y_offset > 75 ? 75 : y_offset;
                    mouses_down_index = -1;
                    is_moving_scroll = true;
                    if (y_offset < 50)
                    {
                        banner_position_offset = y_offset - 50;
                    }
                }
                else if (IsKeyDown(KEY_UP))
                {
                    y_offset += 1;
                    mouses_down_index = -1;
                    is_moving_scroll = true;
                }
                else if (IsKeyDown(KEY_DOWN))
                {
                    y_offset -= 1;
                    mouses_down_index = -1;
                    is_moving_scroll = true;
                }
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                if (mouses_down_index != -1 && !is_moving_scroll)
                {
                    if (selected_saves_index[0] == mouses_down_index)
                    {
                        selected_saves_index[0] = -1;
                    }
                    else if (selected_saves_index[1] == mouses_down_index)
                    {
                        selected_saves_index[1] = -1;
                    }
                    else if (selected_saves_index[0] == -1)
                    {
                        selected_saves_index[0] = mouses_down_index;
                    }
                    else if (selected_saves_index[1] == -1)
                    {
                        selected_saves_index[1] = mouses_down_index;
                    }
                }
                mouses_down_index = -1;
                is_moving_scroll = false;
            }
            // Extract save file name from path
            char *save_name = strrchr(save_file_data->saves_file_path[i], '/');
            save_name++;

            if (is_corrupted)
            {
                corrupted_count++;
                DrawRectangle(25 + (SCREEN_WIDTH - 50) / 12, y_offset + (93 * i) -2, SCREEN_WIDTH - 50 - (2*(SCREEN_WIDTH - 50) / 12), 25, (Color){255, 255, 255, 125});
                DrawRectangleGradientH(25, y_offset + (93 * i) -2, (SCREEN_WIDTH - 50) / 12, 25, (Color){255, 255, 255, 0}, (Color){255, 255, 255, 125});
                DrawRectangleGradientH(SCREEN_WIDTH - 26 - (SCREEN_WIDTH - 50) / 12, y_offset + (93 * i) -2, (SCREEN_WIDTH - 50) / 12, 25, (Color){255, 255, 255, 125}, (Color){255, 255, 255, 0});
                DrawText(save_name, 100, y_offset + (93 * i) + 2, 20, DARKGRAY);
                DrawText("save file invalid", 100 + MeasureText(save_name, 20) + 10, y_offset + (93 * i), 15, BLACK);
            }
            DrawSaveFileContainer(&pkmn_saves[i], save_name, (Rectangle){SCREEN_WIDTH / 2 - (SCREEN_WIDTH - 50) / 2, y_offset + (93 * i) - (60 * corrupted_count), SCREEN_WIDTH - 50, 80}, (selected_saves_index[0] == i || selected_saves_index[1] == i));
        }
        // Reset generation check
        if (!*is_same_generation)
        {
            *is_same_generation = selected_saves_index[1] == -1;
        }

        int text_width = MeasureText("Select two save files to trade between", 20);
        DrawRectangle(0, -10 + banner_position_offset, SCREEN_WIDTH, 50, WHITE);
        // DrawRectangle(SCREEN_WIDTH / 2 - text_width / 2 - 15, -10 + banner_position_offset, text_width + 30, 50, WHITE);
        // DrawRectangleLinesEx((Rectangle){SCREEN_WIDTH / 2 - text_width / 2 - 15, -10 + banner_position_offset, text_width + 30, 50}, 5, BLACK);
        DrawLineEx((Vector2){0, 45 + banner_position_offset}, (Vector2){SCREEN_WIDTH, 45 + banner_position_offset}, 15, BLACK);
        DrawText("Select two save files to trade between", SCREEN_WIDTH / 2 - text_width / 2, 10 + banner_position_offset, 20, BLACK);

        DrawRectangle(0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, 100, WHITE);
        DrawLineEx((Vector2){0, SCREEN_HEIGHT - 50}, (Vector2){SCREEN_WIDTH, SCREEN_HEIGHT - 50}, 15, BLACK);
        
        DrawText("Trade >", NEXT_BUTTON_X, NEXT_BUTTON_Y + 15, 20, has_selected_two_saves && *is_same_generation ? BLACK : LIGHTGRAY);
        if (has_selected_two_saves)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 15, BUTTON_WIDTH, BUTTON_HEIGHT}))
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
    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y + 15, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 15, BUTTON_WIDTH, BUTTON_HEIGHT}))
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
    DrawText("Change Save Directory", SCREEN_WIDTH / 2 - button_width / 2, BACK_BUTTON_Y + 15, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){SCREEN_WIDTH / 2 - button_width / 2 - 10, BACK_BUTTON_Y - 15, button_width, BUTTON_HEIGHT}))
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
