#include "raylibhelper.h"
#include "filehelper.h"
#include "pksavhelper.h"
#include "pksavfilehelper.h"
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

// Draws a button with the pokemon nickname
void draw_pkmn_button(Rectangle rect, int index, char *pokemon_nickname, bool selected)
{
    DrawText(pokemon_nickname, rect.x + 10, rect.y + 6, 20, selected ? LIGHTGRAY : BLACK);
}

// Concantenate the trainer's name and id into a string for Raylib to draw
void create_trainer_name_str(const struct TrainerInfo *trainer, char *trainer_name, bool show_gender)
{
    strcpy(trainer_name, "NAME/");
    strcat(trainer_name, trainer->trainer_name);
    if (trainer->trainer_generation == SAVE_GENERATION_2 && show_gender)
    {
        strcat(trainer_name, " ");
        strcat(trainer_name, trainer->trainer_gender == PKSAV_GEN2_GENDER_FEMALE ? "F" : "M");
    }
}

// Concantenate the trainer's id into a string for Raylib to draw
void create_trainer_id_str(const struct TrainerInfo *trainer, char *trainer_id)
{
    char id_str[6];
    strcpy(trainer_id, "IDNo ");
    snprintf(id_str, sizeof(id_str), "%05u", trainer->trainer_id); // "IDNo %u" loses 3 chars even with enough space?
    strcat(trainer_id, id_str);
}

#if defined(__APPLE__)
void get_mac_resource_images_path(void)
{
    // Get the path to the bundle's resources directory
    CFBundleRef bundle = CFBundleGetMainBundle();
    CFURLRef resources_url = CFBundleCopyResourcesDirectoryURL(bundle);
    char resources_path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resources_url, true, (UInt8 *)resources_path, PATH_MAX))
    {
        fprintf(stderr, "Error: could not get resources directory path\n");
        exit(EXIT_FAILURE);
    }
    CFRelease(resources_url);

    // Change the current working directory to the resources directory
    if (chdir(resources_path) != 0)
    {
        fprintf(stderr, "Error: could not change working directory to resources directory\n");
        exit(EXIT_FAILURE);
    }
}
#endif

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
}

void draw_top_banner(const char *text, int *banner_position_offset)
{
    int text_width = MeasureText(text, 20);
    DrawRectangle(0, *banner_position_offset - 10, SCREEN_WIDTH, 50, WHITE);
    DrawLineEx((Vector2){0, *banner_position_offset + 45}, (Vector2){SCREEN_WIDTH, *banner_position_offset + 45}, 15, BLACK);
    DrawText(text, SCREEN_WIDTH / 2 - text_width / 2, 10 + *banner_position_offset, 20, BLACK);
}

void draw_raylib_screen_loop(
    struct SaveFileData *save_file_data,
    struct TrainerInfo *trainer1,
    struct TrainerInfo *trainer2,
    struct TrainerSelection trainerSelection[2],
    char *player1_save_path,
    char *player2_save_path,
    PokemonSave *pkmn_save_player1,
    PokemonSave *pkmn_save_player2)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pokerom Trader");
    SetTargetFPS(60);
    GameScreen current_screen = SCREEN_MAIN_MENU;
    static bool is_same_generation = true;
    static bool should_close_window = false;
    bool is_build_prerelease = strcmp(PROJECT_VERSION_TYPE, "prerelease") == 0;
    Texture2D textures[19] = {
        [0 ... 18] = {
            .id = 0}};

    char error_message[100] = "Something went wrong";

#if defined(__APPLE__)
    if (CI_BUILD)
    {
        get_mac_resource_images_path();
    }
#endif

    // while textures are loading
    int texture_load_loop_limit = 3;
    int texture_loop_count = 0;
    while (texture_loop_count <= texture_load_loop_limit && (textures[0].id == 0 || textures[1].id == 0 || textures[2].id == 0 || textures[3].id == 0 || textures[4].id == 0))
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Loading Textures...", 20, 20, 20, WHITE);
        EndDrawing();

        if (textures[T_LOGO].id == 0)
        {
            textures[T_LOGO] = LoadTextureFromImage(LoadImage("assets/images/logo-text.png"));
        }
        if (textures[T_TRADE].id == 0)
        {
            textures[T_TRADE] = LoadTextureFromImage(LoadImage("assets/images/trade.png"));
        }
        if (textures[T_EVOLVE].id == 0)
        {
            textures[T_EVOLVE] = LoadTextureFromImage(LoadImage("assets/images/evolve.png"));
        }
        if (textures[T_SETTINGS].id == 0)
        {
            textures[T_SETTINGS] = LoadTextureFromImage(LoadImage("assets/images/settings.png"));
        }
        if (textures[T_QUIT].id == 0)
        {
            textures[T_QUIT] = LoadTextureFromImage(LoadImage("assets/images/quit.png"));
        }

        for (int i = T_CONSOLE_0; i < T_POKEBALL_0; i++)
        {
            if (textures[i].id == 0)
            {
                textures[i] = LoadTextureFromImage(LoadImage(TextFormat("assets/images/Pixel_Fantasy_Icons_Consoles/Consoles/console_%d.png", i - T_CONSOLE_0)));
            }
        }
        for (int i = T_POKEBALL_0; i < T_POKEBALL_3 + 1; i++)
        {
            if (textures[i].id == 0)
            {
                textures[i] = LoadTextureFromImage(LoadImage(TextFormat("assets/images/pokeballs_MPR/ball_%d.png", i - T_POKEBALL_0)));
            }
        }

        texture_loop_count++;
        if (texture_loop_count >= texture_load_loop_limit)
        {
            current_screen = SCREEN_ERROR;
            strcpy(error_message, "Failed to load textures. Is asset folder with exe?");
        }
    }

    while (!should_close_window && !WindowShouldClose())
    {
        switch (current_screen)
        {
        case SCREEN_FILE_SELECT:
            draw_file_select(save_file_data, player1_save_path, player2_save_path, trainer1, trainer2, trainerSelection, pkmn_save_player1, pkmn_save_player2, &current_screen, &is_same_generation);
            break;
        case SCREEN_TRADE:
            draw_trade(pkmn_save_player1, pkmn_save_player2, player1_save_path, player2_save_path, trainerSelection, trainer1, trainer2, &is_same_generation, &current_screen);
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
            draw_evolve(pkmn_save_player1, player1_save_path, trainer1, &current_screen);
            break;
        case SCREEN_ABOUT:
        {
            draw_about(&current_screen, is_build_prerelease);
            break;
        }
        case SCREEN_LEGAL:
        {
            draw_legal(&current_screen);
            break;
        }
        default:
            BeginDrawing();
            ClearBackground(BACKGROUND_COLOR);
            DrawText(error_message, SCREEN_CENTER(error_message, 20).x, SCREEN_CENTER(error_message, 20).y, 20, BLACK);
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
