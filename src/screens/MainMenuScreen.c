#include "raylibhelper.h"
#include "filehelper.h"

enum main_menu_buttons
{
    BUTTON_NONE = -1,
    BUTTON_TRADE,
    BUTTON_EVOLVE,
    BUTTON_SETTINGS,
    BUTTON_QUIT,
    BUTTON_COUNT
};
const uint8_t anim_speed = 45;
// Entire details pane stays in place
const Rectangle details_rec = (Rectangle){SCREEN_WIDTH * 0.55, SCREEN_HEIGHT * 0.35, 200, 200};
// starting position of details pane content offscreen (offset from details_rec.x)
const uint16_t offscreen_x = 415;
// offset from details_rec.x
const uint8_t console_x_offset = 70;

// Details content animation position
static int16_t anim_from_right[4] = {offscreen_x};
// which button is currently being animated
static int8_t active_anim_index = BUTTON_NONE;
// which button is currently being hovered over
static int8_t active_hover_index = BUTTON_NONE;
// which button has been selected
static int8_t selected_main_menu_index = BUTTON_NONE;

// Update which details pane animation should be displayed
void set_active_animation(const uint8_t selection)
{
    if (selection >= BUTTON_COUNT)
        return;
    active_anim_index = selection;
    active_hover_index = selection;
    selected_main_menu_index = selection;
}

void clear_active_animation(void)
{
    active_hover_index = BUTTON_NONE;
}

void reset_anim_pos(uint8_t index)
{
    anim_from_right[index] = offscreen_x;
}

void reset_all_anim_pos(void)
{
    for (int i = BUTTON_TRADE; i < BUTTON_COUNT; i++)
    {
        reset_anim_pos(i);
    }
}

// move details pane content left/right
void slide_animate_details_pane(uint8_t button_selection)
{
    // Move left until at details_rec.x
    if (active_hover_index == button_selection && anim_from_right[button_selection] >= 0)
    {
        anim_from_right[button_selection] -= anim_speed;
    }
    // Move right until offscreen when not hovered
    if (active_hover_index == BUTTON_NONE)
    {
        anim_from_right[button_selection] += anim_speed;
    }
    // if offscreen clear active animation
    if (anim_from_right[button_selection] > offscreen_x)
    {
        active_anim_index = BUTTON_NONE;
        reset_anim_pos(button_selection);
    }
}

// Arrow animation on trade details content
void draw_trade_arrow_animation(void)
{
    static uint8_t frame_counter = 0;
    static uint8_t arrow_anim_index = 0;

    // Draw arrow to right of consoles[0]
    if (arrow_anim_index > 0 && arrow_anim_index < 5)
        DrawRectangle(details_rec.x + anim_from_right[BUTTON_TRADE] + console_x_offset, details_rec.y + 170, 10, 65, BLACK);
    if (arrow_anim_index > 1 && arrow_anim_index < 5)
        DrawRectangle(details_rec.x + anim_from_right[BUTTON_TRADE] + console_x_offset + 15, details_rec.y + 170, 20, 65, BLACK);
    if (arrow_anim_index > 2 && arrow_anim_index < 5)
        DrawRectangle(details_rec.x + anim_from_right[BUTTON_TRADE] + console_x_offset + 40, details_rec.y + 170, 30, 65, BLACK);
    if (arrow_anim_index > 3 && arrow_anim_index < 5)
        DrawTriangle(
            (Vector2){details_rec.x + anim_from_right[BUTTON_TRADE] + console_x_offset + 75, details_rec.y + 170},
            (Vector2){details_rec.x + anim_from_right[BUTTON_TRADE] + console_x_offset + 75, details_rec.y + 235},
            (Vector2){details_rec.x + anim_from_right[BUTTON_TRADE] + console_x_offset + 110, details_rec.y + 202}, BLACK);
    if (arrow_anim_index > 4)
        DrawRectangle(details_rec.x + anim_from_right[BUTTON_TRADE] + console_x_offset + 100, details_rec.y + 170, 10, 65, BLACK);
    if (arrow_anim_index > 5)
        DrawRectangle(details_rec.x + anim_from_right[BUTTON_TRADE] + console_x_offset + 75, details_rec.y + 170, 20, 65, BLACK);
    if (arrow_anim_index > 6)
        DrawRectangle(details_rec.x + anim_from_right[BUTTON_TRADE] + console_x_offset + 40, details_rec.y + 170, 30, 65, BLACK);
    if (arrow_anim_index > 7)
        DrawTriangle(
            (Vector2){details_rec.x + anim_from_right[BUTTON_TRADE] + console_x_offset + 35, details_rec.y + 170},
            (Vector2){details_rec.x + anim_from_right[BUTTON_TRADE] + console_x_offset + 0, details_rec.y + 202},
            (Vector2){details_rec.x + anim_from_right[BUTTON_TRADE] + console_x_offset + 35, details_rec.y + 235},
            BLACK);

    if (frame_counter % 10 == 0)
    {
        arrow_anim_index++;
        frame_counter = 0;
    }
    arrow_anim_index %= 9;
    frame_counter++;
}

// Arrow animation on evolve details content
void draw_evolution_arrow_animation(void)
{
    static uint8_t frame_counter = 0;
    // Controls when each animation frame is drawn
    static uint8_t arrow_anim_index = 0;

    // show bottom rectangle
    if (arrow_anim_index > 0)
        DrawRectangle(details_rec.x + anim_from_right[BUTTON_EVOLVE] + 200, details_rec.y + 190, 50, 10, BLACK);
    // show middle rectangle
    if (arrow_anim_index > 1)
        DrawRectangle(details_rec.x + anim_from_right[BUTTON_EVOLVE] + 200, details_rec.y + 170, 50, 15, BLACK);
    // show top triangle
    if (arrow_anim_index > 2)
        DrawTriangle(
            (Vector2){details_rec.x + anim_from_right[BUTTON_EVOLVE] + 200, details_rec.y + 165},
            (Vector2){details_rec.x + anim_from_right[BUTTON_EVOLVE] + 250, details_rec.y + 165},
            (Vector2){details_rec.x + anim_from_right[BUTTON_EVOLVE] + 225, details_rec.y + 140}, BLACK);

    // Control animation speed
    if (frame_counter % 15 == 0)
    {
        arrow_anim_index++;
        frame_counter = 0;
    }
    // clamp animation index
    arrow_anim_index %= 4;
    frame_counter++;
}

void draw_main_menu(struct save_file_data *save_file_data, GameScreen *current_screen, bool *should_close_window, Texture2D *textures)
{
    BeginDrawing();
    ClearBackground(RED);
    draw_background_grid();
    
    // Details Pane background
    DrawCircle(SCREEN_WIDTH * 1.15, SCREEN_HEIGHT * 1.725, 800, BLACK);
    DrawCircle(SCREEN_WIDTH * 1.15, SCREEN_HEIGHT * 1.725, 730, WHITE);

    // Start position of menu buttons
    const Vector2 text_position_start = (Vector2){75, 200};
    // Button height offset from start position
    const int rec_height_offset = 50;
    // Menu button text size
    const int text_size = 30;
    // Text at bottom of details pane
    const Vector2 details_text = (Vector2){details_rec.x - 100, SCREEN_HEIGHT - 30};

    static int rand_pokeball_index = -1;
    static enum selected_console_texture {
        CONSOLE_NONE = -1,
        CONSOLE_LEFT,
        CONSOLE_RIGHT,
        CONSOLE_COUNT,
    } rand_console[CONSOLE_COUNT] = {CONSOLE_NONE};

    // Draw image pkrom_trader_logo header
    DrawTextureEx(textures[T_LOGO], (Vector2){50, 50}, 0, 1, WHITE);

    // On Hover/Click Trade button
    if (draw_menu_button(text_position_start.x, text_position_start.y, "Trade", text_size))
    {
        set_active_animation(BUTTON_TRADE);
    }

    // Update/Draw animated Trade details pane
    if (active_anim_index == BUTTON_TRADE)
    {
        // Get two random console textures
        for (int i = 0; i < CONSOLE_COUNT; i++)
        {
            if (rand_console[i] == T_NONE)
                rand_console[i] = GetRandomValue(T_CONSOLE_0, T_CONSOLE_9);
        }

        // Move left/right
        slide_animate_details_pane(BUTTON_TRADE);
        // Trade texture logo
        DrawTextureEx(textures[T_TRADE], (Vector2){details_rec.x + anim_from_right[BUTTON_TRADE], details_rec.y + 25}, 0, 1, WHITE);
        // Left console texture
        DrawTextureEx(textures[rand_console[CONSOLE_LEFT]], (Vector2){details_rec.x + anim_from_right[BUTTON_TRADE] - 40, details_rec.y + 150}, 0, 3, WHITE);
        // Arrow animation
        draw_trade_arrow_animation();
        // Right console texture
        DrawTextureEx(textures[rand_console[CONSOLE_RIGHT]], (Vector2){details_rec.x + anim_from_right[BUTTON_TRADE] + console_x_offset + 120, details_rec.y + 150}, 0, 3, WHITE);
        // Bottom details text
        DrawText("Trade pokemon between save files", details_text.x + anim_from_right[BUTTON_TRADE] + 40, details_text.y, 20, BLACK);
    }
    else
    {
        // Reset random console textures to none
        for (int i = 0; i < CONSOLE_COUNT; i++)
        {
            rand_console[i] = CONSOLE_NONE;
        }
        // Reset animation position to offscreen_x
        reset_anim_pos(BUTTON_TRADE);
    }

    // On Hover/Click Evolve button
    if (draw_menu_button(text_position_start.x + 15, text_position_start.y + rec_height_offset, "Evolve", text_size))
    {
        set_active_animation(BUTTON_EVOLVE);
    }

    // Update/Draw animated Evolve details pane
    if (active_anim_index == BUTTON_EVOLVE)
    {
        // Get random pokeball texture
        if (rand_pokeball_index == T_NONE)
        {
            rand_pokeball_index = GetRandomValue(T_POKEBALL_0, T_POKEBALL_3);
        }

        // Move left/right
        slide_animate_details_pane(BUTTON_EVOLVE);
        // Evolve texture logo
        DrawTextureEx(textures[T_EVOLVE], (Vector2){details_rec.x + anim_from_right[BUTTON_EVOLVE], details_rec.y + 25}, 0, 1, WHITE);
        // Pokeball texture
        DrawTextureEx(textures[rand_pokeball_index], (Vector2){details_rec.x + anim_from_right[BUTTON_EVOLVE] + 15, details_rec.y + 80}, 0, 0.25, WHITE);
        // Arrow animation
        draw_evolution_arrow_animation();
        // Bottom details text
        DrawText("Evolve pokemon with trade evolutions", details_text.x + anim_from_right[BUTTON_EVOLVE] + 40, details_text.y, 20, BLACK);
    }
    else
    {
        // Reset random pokeball texture to none
        rand_pokeball_index = T_NONE;
        // Reset animation position to offscreen_x
        reset_anim_pos(BUTTON_EVOLVE);
    }

    // On Hover/Click Settings button
    if (draw_menu_button(text_position_start.x + 30, text_position_start.y + (rec_height_offset * 2), "Settings", text_size))
    {
        set_active_animation(BUTTON_SETTINGS);
    }

    // Update/Draw animated Settings details pane
    if (active_anim_index == BUTTON_SETTINGS)
    {
        // Move left/right
        slide_animate_details_pane(BUTTON_SETTINGS);
        // Draw settings texture logo
        DrawTextureEx(textures[T_SETTINGS], (Vector2){details_rec.x + anim_from_right[BUTTON_SETTINGS] - 25, details_rec.y + 100}, 0, 1, WHITE);
        // Bottom details text
        DrawText("Change trade and evolution settings", details_text.x + anim_from_right[BUTTON_SETTINGS] + 40, details_text.y, 20, BLACK);
    }
    else
    {
        // Reset animation position to offscreen_x
        reset_anim_pos(BUTTON_SETTINGS);
    }

    // On Hover/Click Quit button
    if (draw_menu_button(text_position_start.x + 45, text_position_start.y + (rec_height_offset * 3), "Quit", text_size))
    {
        set_active_animation(BUTTON_QUIT);
    }

    // Update/Draw animated Quit details pane
    if (active_anim_index == BUTTON_QUIT)
    {
        // Move left/right
        slide_animate_details_pane(BUTTON_QUIT);
        // Draw quit texture logo
        DrawTextureEx(textures[T_QUIT], (Vector2){details_rec.x + anim_from_right[BUTTON_QUIT], details_rec.y + 100}, 0, 1, WHITE);
        // Bottom details text
        DrawText("Quit Pokerom Trader", details_text.x + anim_from_right[BUTTON_QUIT] + 80, details_text.y, 20, BLACK);
    }
    else
    {
        // Reset animation position to offscreen_x
        reset_anim_pos(BUTTON_QUIT);
    }

    // On Click Release
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && selected_main_menu_index != BUTTON_NONE)
    {
        switch (selected_main_menu_index)
        {
        case BUTTON_TRADE:
        {
            // Selected Trade and still hovering over Trade button
            if (active_hover_index == BUTTON_TRADE)
            {
                no_dir_err = get_save_files(save_file_data);
                *current_screen = SCREEN_FILE_SELECT;
                reset_all_anim_pos();
                clear_active_animation();
            }
            break;
        }
        case BUTTON_EVOLVE:
        {
            // Selected Evolve and still hovering over Evolve button
            if (active_hover_index == BUTTON_EVOLVE)
            {
                no_dir_err = get_save_files(save_file_data);
                *current_screen = SCREEN_EVOLVE_FILE_SELECT;
                reset_all_anim_pos();
                clear_active_animation();
            }
            break;
        }
        case BUTTON_SETTINGS:
        {
            // Selected Settings and still hovering over Settings button
            if (active_hover_index == BUTTON_SETTINGS)
            {
                *current_screen = SCREEN_SETTINGS;
                reset_all_anim_pos();
                clear_active_animation();
            }
            break;
        }
        case BUTTON_QUIT:
        {
            // Selected Quit and still hovering over Quit button
            if (active_hover_index == BUTTON_QUIT)
            {
                *should_close_window = true;
                reset_all_anim_pos();
                clear_active_animation();
            }
            break;
        }
        default:
            break;
        }
    }

    EndDrawing();

    // Reset active hover index
    clear_active_animation();
}
