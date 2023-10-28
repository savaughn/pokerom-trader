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

#if defined(__APPLE__)
    if (CI_BUILD)
    {
        get_mac_resource_images_path();
    }
#endif

    // while textures are loading
    while (textures[0].id == 0 || textures[1].id == 0 || textures[2].id == 0 || textures[3].id == 0 || textures[4].id == 0)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Loading...", 20, 20, 20, WHITE);
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
