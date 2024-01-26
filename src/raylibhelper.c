#include "raylibhelper.h"
#include "filehelper.h"
#include "pksavhelper.h"
#include "pksavfilehelper.h"
#include "textures.h"
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

void draw_background_grid(void)
{
    int line_count_v = SCREEN_HEIGHT / 10;
    int line_count_h = SCREEN_WIDTH / 10;

    // draw line grid color_pkmn_red
    for (int i = 0; i < line_count_v; i++)
    {
        DrawRectangle(0, i * 10, SCREEN_WIDTH, 1, COLOR_PKMN_RED);
    }
    for (int i = 0; i < line_count_h; i++)
    {
        DrawRectangle(i * 10, 0, 1, SCREEN_HEIGHT, COLOR_PKMN_RED);
    }
}

// Draws a button with the pokemon nickname
void draw_pkmn_button(Rectangle rect, int index, char *pokemon_nickname, bool selected)
{
    shadow_text(pokemon_nickname, rect.x + 10, rect.y + 6, 20, selected ? LIGHTGRAY : WHITE);
}

// Concantenate the trainer's name and id into a string for Raylib to draw
void create_trainer_name_str(const struct trainer_info *trainer, char *trainer_name)
{
    strcpy(trainer_name, "NAME/");
    strcat(trainer_name, trainer->trainer_name);
}

// Concantenate the trainer's id into a string for Raylib to draw
void create_trainer_id_str(const struct trainer_info *trainer, char *trainer_id)
{
    char id_str[6];
    strcpy(trainer_id, "IDNo ");
    snprintf(id_str, sizeof(id_str), "%05u", trainer->trainer_id);
    strcat(trainer_id, id_str);
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

void draw_no_save_files(char *save_path)
{
    ClearBackground(RED);
    DrawRectangle(20, 150, SCREEN_WIDTH - 40, SCREEN_HEIGHT - 300, (Color){0, 0, 0, 50});
    if (no_dir_err)
    {
        Vector2 text_center = SCREEN_CENTER("Save folder doesn't exist!", 20);
        shadow_text("Save folder doesn't exist!", text_center.x, text_center.y, 20, WHITE);
    }
    else
    {
        shadow_text("No save files found in save folder", 190, 200, 20, WHITE);
    }
    shadow_text(TextFormat("%s", save_path), SCREEN_CENTER(save_path, 20).x, 230, 20, WHITE);

    char *text = "Add save files to the save folder or drag and drop save files here!";
    shadow_text(text, SCREEN_CENTER(text, 20).x, 300, 20, WHITE);
}

void draw_top_banner(const char *text, const int *banner_position_offset)
{
    int text_width = MeasureText(text, 20);
    DrawRectangle(0, *banner_position_offset - 10, SCREEN_WIDTH, 50, WHITE);
    DrawLineEx((Vector2){0, *banner_position_offset + 45}, (Vector2){SCREEN_WIDTH, *banner_position_offset + 45}, 15, BLACK);
    DrawText(text, SCREEN_WIDTH / 2 - text_width / 2, 10 + *banner_position_offset, 20, BLACK);
}

void draw_raylib_screen_loop(
    struct save_file_data *save_file_data,
    struct trainer_info *trainer1,
    struct trainer_info *trainer2,
    struct TrainerSelection trainerSelection[2],
    char *player1_save_path,
    char *player2_save_path,
    PokemonSave *pkmn_save_player1,
    PokemonSave *pkmn_save_player2)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pokerom Trader");
    SetTargetFPS(60);
    SetExitKey(0);
    GameScreen current_screen = SCREEN_MAIN_MENU;
    static bool is_same_generation = true;
    static bool should_close_window = false;
    bool is_build_prerelease = strcmp(PROJECT_VERSION_TYPE, "prerelease") == 0;
    Texture2D textures[19] = {
        [0 ... 18] = {
            .id = 0}};
    const struct texture_data *texture_data[5] = {
        &evolve_data,
        &logo_data,
        &quit_data,
        &settings_data,
        &trade_data};

    // while textures are loading
    int texture_load_loop_limit = 3;
    int texture_loop_count = 0;
    while (texture_loop_count <= texture_load_loop_limit && (textures[0].id == 0 || textures[1].id == 0 || textures[2].id == 0 || textures[3].id == 0 || textures[4].id == 0))
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Loading Textures...", 20, 20, 20, WHITE);
        EndDrawing();

        for (uint8_t i = T_EVOLVE; i < T_CONSOLE_0; i++)
        {
            if (textures[i].id == 0)
            {
                Image img = {0};
                img.format = FORMAT;
                img.height = texture_data[i]->height;
                img.width = texture_data[i]->width;
                img.data = texture_data[i]->data;
                img.mipmaps = 1;

                textures[i] = LoadTextureFromImage(img);
            }
        }

        for (int i = T_CONSOLE_0; i < T_POKEBALL_0; i++)
        {
            if (textures[i].id == 0)
            {
                Image console = {0};
                console.format = FORMAT;
                console.height = CONSOLE_HEIGHT;
                console.width = CONSOLE_WIDTH;
                console.data = console_data[i - T_CONSOLE_0];
                console.mipmaps = 1;

                textures[i] = LoadTextureFromImage(console);
            }
        }

        for (int i = T_POKEBALL_0; i < T_COUNT; i++)
        {
            if (textures[i].id == 0)
            {
                Image pokeball = {0};
                pokeball.format = FORMAT;
                pokeball.height = BALL_HEIGHT;
                pokeball.width = BALL_WIDTH;
                pokeball.data = ball_data[i - T_POKEBALL_0];
                pokeball.mipmaps = 1;

                textures[i] = LoadTextureFromImage(pokeball);
            }
        }

        texture_loop_count++;
    }

    while (!should_close_window && !WindowShouldClose())
    {
        switch (current_screen)
        {
        case SCREEN_FILE_SELECT:
            draw_file_select(save_file_data, player1_save_path, player2_save_path, trainer1, trainer2, trainerSelection, pkmn_save_player1, pkmn_save_player2, &current_screen, &is_same_generation);
            break;
        case SCREEN_TRADE:
            draw_trade(pkmn_save_player1, pkmn_save_player2, player1_save_path, player2_save_path, trainerSelection, trainer1, trainer2, &is_same_generation, &current_screen, &textures[T_TRADE]);
            break;
        case SCREEN_MAIN_MENU:
            draw_main_menu(save_file_data, &current_screen, &should_close_window, textures);
            break;
        case SCREEN_SETTINGS:
            draw_settings(save_file_data, &current_screen, &textures[T_SETTINGS]);
            break;
        case SCREEN_FILE_EDIT:
            draw_change_dir(save_file_data, &current_screen, &textures[T_SETTINGS]);
            break;
        case SCREEN_BILLS_PC_FILE_SELECT:
            draw_file_select_single(save_file_data, pkmn_save_player1, player1_save_path, trainer1, &trainerSelection[0], SINGLE_PLAYER_MENU_TYPE_BILLS_PC, &current_screen);
            break;
        case SCREEN_BILLS_PC:
            draw_bills_pc(pkmn_save_player1, player1_save_path, trainer1, &trainerSelection[0], &current_screen);
            break;
        case SCREEN_EVOLVE_FILE_SELECT:
            draw_file_select_single(save_file_data, pkmn_save_player1, player1_save_path, trainer1, &trainerSelection[0], SINGLE_PLAYER_MENU_TYPE_EVOLVE, &current_screen);
            break;
        case SCREEN_EVOLVE:
            draw_evolve(pkmn_save_player1, player1_save_path, trainer1, &current_screen, &textures[T_EVOLVE]);
            break;
        case SCREEN_ABOUT:
            draw_about(&current_screen, is_build_prerelease);
            break;
        case SCREEN_LEGAL:
            draw_legal(&current_screen);
            break;
        default:
            BeginDrawing();
            ClearBackground(BACKGROUND_COLOR);
            DrawText("Something went wrong", SCREEN_CENTER("Something went wrong", 20).x, SCREEN_CENTER("Something went wrong", 20).y, 20, BLACK);
            DrawText("Press ESC key to exit!", SCREEN_CENTER("Press ESC key to exit!", 20).x, SCREEN_CENTER("Press ESC key to exit!", 20).x + 50, 20, BLACK);
            EndDrawing();
            // Escape key to close window
            if (IsKeyPressed(KEY_ESCAPE))
            {
                should_close_window = true;
            }
            break;
        }
    }

    for (uint8_t i = 0; i < T_COUNT; i++)
    {
        UnloadTexture(textures[i]);
    }

    CloseWindow();
}
