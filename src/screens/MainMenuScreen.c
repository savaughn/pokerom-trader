#include "raylibhelper.h"
#include "filehelper.h"

void draw_main_menu(struct SaveFileData *save_file_data, GameScreen *current_screen, bool *should_close_window, Texture2D *textures)
{
    BeginDrawing();
    ClearBackground(COLOR_PKMN_RED);
    DrawCircle(SCREEN_WIDTH * 1.15, SCREEN_HEIGHT * 1.725, 800, BLACK);
    DrawCircle(SCREEN_WIDTH * 1.15, SCREEN_HEIGHT * 1.725, 730, WHITE);

    enum main_menu_buttons
    {
        BUTTON_NONE = -1,
        BUTTON_TRADE,
        BUTTON_EVOLVE,
        BUTTON_SETTINGS,
        BUTTON_QUIT
    };

    const int start_y = 200;
    const int start_x = 75;
    const int rec_height_offset = 50;
    const int text_size = 30;
    const uint8_t anim_speed = 45;
    int selected_main_menu_index = BUTTON_NONE;
    static int rand_console_index_1 = -1;
    static int rand_console_index_2 = -1;
    static int rand_pokeball_index = -1;
    static int anim_from_right[4] = {300, 300, 300, 300};
    static int active_anim_index = -1;
    static int active_hover_index = -1;

    const Rectangle details_rec = (Rectangle){SCREEN_WIDTH * 0.55, SCREEN_HEIGHT * 0.35, 200, 200};
    const Vector2 details_text = (Vector2){details_rec.x - 100, SCREEN_HEIGHT - 30};
    const uint8_t console_x_offset = 70;
    static uint8_t anim_index = 0;

    // if (SHOW_BILLS_PC && CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 300, 200, 20}))
    //          get_save_files(save_file_data);
    //          *current_screen = SCREEN_BILLS_PC_FILE_SELECT;

    // Draw image pkrom_trader_logo
    DrawTextureEx(textures[T_LOGO], (Vector2){50, 50}, 0, 0.62, WHITE);
    if (draw_menu_button(start_x, start_y, "Trade", text_size))
    {
        active_anim_index = 0;
        active_hover_index = BUTTON_TRADE;
        selected_main_menu_index = BUTTON_TRADE;
    }
    else
    {
        active_hover_index = BUTTON_NONE;
    }
    if (active_anim_index == BUTTON_TRADE)
    {
        static int frame_counter = 0;
        if (rand_console_index_1 == -1)
            rand_console_index_1 = GetRandomValue(T_CONSOLE_0, T_CONSOLE_9);
        if (rand_console_index_2 == -1)
            rand_console_index_2 = GetRandomValue(T_CONSOLE_0, T_CONSOLE_9);

        DrawTextureEx(textures[T_TRADE], (Vector2){details_rec.x + anim_from_right[0], details_rec.y + 25}, 0, 0.5, WHITE);
        DrawTextureEx(textures[rand_console_index_1], (Vector2){details_rec.x + anim_from_right[0] - 40, details_rec.y + 150}, 0, 3, WHITE);

        // Draw arrow to right of consoles[0]
        if (anim_index > 0 && anim_index < 5)
            DrawRectangle(details_rec.x + anim_from_right[0] + console_x_offset, details_rec.y + 170, 10, 65, BLACK);
        if (anim_index > 1 && anim_index < 5)
            DrawRectangle(details_rec.x + anim_from_right[0] + console_x_offset + 15, details_rec.y + 170, 20, 65, BLACK);
        if (anim_index > 2 && anim_index < 5)
            DrawRectangle(details_rec.x + anim_from_right[0] + console_x_offset + 40, details_rec.y + 170, 30, 65, BLACK);
        if (anim_index > 3 && anim_index < 5)
            DrawTriangle(
                (Vector2){details_rec.x + anim_from_right[0] + console_x_offset + 75, details_rec.y + 170},
                (Vector2){details_rec.x + anim_from_right[0] + console_x_offset + 75, details_rec.y + 235},
                (Vector2){details_rec.x + anim_from_right[0] + console_x_offset + 110, details_rec.y + 202}, BLACK);
        if (anim_index > 4)
            DrawRectangle(details_rec.x + anim_from_right[0] + console_x_offset + 100, details_rec.y + 170, 10, 65, BLACK);
        if (anim_index > 5)
            DrawRectangle(details_rec.x + anim_from_right[0] + console_x_offset + 75, details_rec.y + 170, 20, 65, BLACK);
        if (anim_index > 6)
            DrawRectangle(details_rec.x + anim_from_right[0] + console_x_offset + 40, details_rec.y + 170, 30, 65, BLACK);
        if (anim_index > 7)
            DrawTriangle(
                (Vector2){details_rec.x + anim_from_right[0] + console_x_offset + 35, details_rec.y + 170},
                (Vector2){details_rec.x + anim_from_right[0] + console_x_offset + 0, details_rec.y + 202},
                (Vector2){details_rec.x + anim_from_right[0] + console_x_offset + 35, details_rec.y + 235},
                BLACK);

        if (frame_counter % 10 == 0)
            anim_index++;
        if (active_hover_index == BUTTON_TRADE && anim_from_right[0] >= 0)
            anim_from_right[0] -= anim_speed;
        if (active_hover_index == BUTTON_NONE)
            anim_from_right[0] += anim_speed;
        if (anim_from_right[0] > 300)
        {
            active_anim_index = -1;
            anim_from_right[0] = 300;
        }
        anim_index %= 9;
        frame_counter++;

        DrawTextureEx(textures[rand_console_index_2], (Vector2){details_rec.x + anim_from_right[0] + console_x_offset + 120, details_rec.y + 150}, 0, 3, WHITE);
        DrawText("Trade pokemon between save files", details_text.x + anim_from_right[0] + 40, details_text.y, 20, BLACK);
    }
    else
    {
        rand_console_index_1 = -1;
        rand_console_index_2 = -1;
        anim_from_right[0] = 300;
    }
    // if (SHOW_BILLS_PC)
    //     DrawText("Bill's PC", 190, 300, 20, BLACK);
    if (draw_menu_button(start_x + 15, start_y + rec_height_offset, "Evolve", text_size))
    {
        active_anim_index = 1;
        active_hover_index = BUTTON_EVOLVE;
        selected_main_menu_index = BUTTON_EVOLVE;
    }
    else
    {
        active_hover_index = BUTTON_NONE;
    }
    if (active_anim_index == BUTTON_EVOLVE)
    {
        static int frame_counter = 0;
        if (rand_pokeball_index == -1)
            rand_pokeball_index = GetRandomValue(T_POKEBALL_0, T_POKEBALL_3);

        DrawTextureEx(textures[T_EVOLVE], (Vector2){details_rec.x + anim_from_right[1], details_rec.y + 25}, 0, 0.5, WHITE);
        DrawTextureEx(textures[rand_pokeball_index], (Vector2){details_rec.x + anim_from_right[1] + 15, details_rec.y + 80}, 0, 0.25, WHITE);
        if (anim_index > 0)
            DrawRectangle(details_rec.x + anim_from_right[1] + 200, details_rec.y + 190, 50, 10, BLACK);
        if (anim_index > 1)
            DrawRectangle(details_rec.x + anim_from_right[1] + 200, details_rec.y + 170, 50, 15, BLACK);
        if (anim_index > 2)
            DrawTriangle(
                (Vector2){details_rec.x + anim_from_right[1] + 200, details_rec.y + 165},
                (Vector2){details_rec.x + anim_from_right[1] + 250, details_rec.y + 165},
                (Vector2){details_rec.x + anim_from_right[1] + 225, details_rec.y + 140}, BLACK);
        DrawText("Evolve pokemon with trade evolutions", details_text.x + anim_from_right[1] + 40, details_text.y, 20, BLACK);

        if (frame_counter % 15 == 0)
            anim_index++;
        if (active_hover_index == BUTTON_EVOLVE && anim_from_right[1] >= 0)
            anim_from_right[1] -= anim_speed;
        if (active_hover_index == BUTTON_NONE)
            anim_from_right[1] += anim_speed;
        if (anim_from_right[1] > 300)
        {
            active_anim_index = -1;
            anim_from_right[1] = 300;
        }
        anim_index %= 4;
        frame_counter++;
    }
    else
    {
        rand_pokeball_index = -1;
        anim_from_right[1] = 300;
    }
    if (draw_menu_button(start_x + 30, start_y + (rec_height_offset * 2), "Settings", text_size))
    {
        active_anim_index = 2;
        active_hover_index = BUTTON_SETTINGS;
        selected_main_menu_index = BUTTON_SETTINGS;
    }
    else
    {
        active_hover_index = BUTTON_NONE;
    }
    if (active_anim_index == BUTTON_SETTINGS)
    {
        DrawTextureEx(textures[T_SETTINGS], (Vector2){details_rec.x + anim_from_right[2] - 25, details_rec.y + 100}, 0, 0.5, WHITE);
        DrawText("Change trade and evolution settings", details_text.x + anim_from_right[2] + 40, details_text.y, 20, BLACK);
        if (active_hover_index == BUTTON_SETTINGS && anim_from_right[2] >= 0)
            anim_from_right[2] -= anim_speed;
        if (active_hover_index == BUTTON_NONE)
            anim_from_right[2] += anim_speed;
        if (anim_from_right[2] > 300)
        {
            active_anim_index = -1;
            anim_from_right[2] = 300;
        }
    }
    else
    {
        anim_from_right[2] = 300;
    }
    if (draw_menu_button(start_x + 45, start_y + (rec_height_offset * 3), "Quit", text_size))
    {
        active_anim_index = 3;
        active_hover_index = BUTTON_QUIT;
        selected_main_menu_index = BUTTON_QUIT;
    }
    else
    {
        active_hover_index = BUTTON_NONE;
    }
    if (active_anim_index == 3)
    {
        DrawTextureEx(textures[T_QUIT], (Vector2){details_rec.x + anim_from_right[3], details_rec.y + 100}, 0, 0.5, WHITE);
        DrawText("Quit Pokerom Trader", details_text.x + anim_from_right[3] + 80, details_text.y, 20, BLACK);
        if (active_hover_index == BUTTON_QUIT && anim_from_right[3] >= 0)
            anim_from_right[3] -= anim_speed;
        if (active_hover_index == BUTTON_NONE)
            anim_from_right[3] += anim_speed;
        if (anim_from_right[3] > 300)
        {
            active_anim_index = -1;
            anim_from_right[3] = 300;
        }
    }
    else
    {
        anim_from_right[3] = 300;
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        switch (selected_main_menu_index)
        {
        case BUTTON_TRADE:
        {
            int err = get_save_files(save_file_data);
            no_dir_err = err;
            *current_screen = SCREEN_FILE_SELECT;
            anim_from_right[0] = 300;
            break;
        }
        case BUTTON_EVOLVE:
        {
            get_save_files(save_file_data);
            *current_screen = SCREEN_EVOLVE_FILE_SELECT;
            anim_from_right[1] = 300;
            break;
        }
        case BUTTON_SETTINGS:
        {
            *current_screen = SCREEN_SETTINGS;
            anim_from_right[2] = 300;
            break;
        }
        case BUTTON_QUIT:
        {
            *should_close_window = true;
            anim_from_right[3] = 300;
            break;
        }
        default:
            break;
        }
    }

    EndDrawing();
}
