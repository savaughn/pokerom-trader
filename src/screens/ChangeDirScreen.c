#include "raylibhelper.h"
#include "filehelper.h"
#ifdef _WIN32
#include <errno.h>
#else
#include <sys/errno.h>
#endif

static struct save_file_data *_save_file_data = NULL;
static bool show_reset_modal = false;
static bool has_reset_config = false;

void on_reset_modal_cancel(void)
{
    show_reset_modal = false;
}

void on_reset_modal_submit(void)
{
#ifdef _WIN32
    create_default_config();
#else
    create_default_config(true);
#endif
    init_settings_from_config(_save_file_data);
    show_reset_modal = false;
    has_reset_config = true;
}

void draw_change_dir(struct save_file_data *save_file_data, GameScreen *current_screen, Texture2D *settings_texture)
{
    _save_file_data = save_file_data;
    static bool editing_text = false;
    static char input_text[MAX_FILE_PATH_CHAR];
    static int char_count = 0;
    static bool has_shown_placeholder = false;
    static enum file_op_results file_op_result = FILE_OP_FAILURE;
    char input_text_backup[MAX_FILE_PATH_CHAR];
    enum screen_buttons
    {
        SCREEN_BUTTON_NONE = -1,
        SCREEN_BUTTON_BACK,
        SCREEN_BUTTON_SAVE,
        SCREEN_BUTTON_CLEAR,
        BUTTON_RESET
    };
    static int selected_index = SCREEN_BUTTON_NONE;
    const Rectangle input_box_rec = (Rectangle){50, SCREEN_HEIGHT / 2 + 5, SCREEN_WIDTH - 100, 40};
    const uint8_t input_text_size = 15;
    const Color settings_text_color = BLACK;
    const Color settings_text_color_selected = LIGHTGRAY;

    char_count = strlen(input_text);
    strcpy(input_text_backup, (char *)save_file_data->save_dir);

    // Placeholder Text
    if (!editing_text && !has_shown_placeholder || has_reset_config)
    {
        strcpy(input_text, (char *)save_file_data->save_dir);
        char_count = strlen(input_text);
        has_shown_placeholder = true;
        has_reset_config = false;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Check if the mouse is clicked within the input box
        if (CheckCollisionPointRec(GetMousePosition(), input_box_rec))
        {
            editing_text = true;
        }
        else
        {
            editing_text = false;
        }
    }

    if (editing_text)
    {
        has_shown_placeholder = true;
        int key = GetCharPressed();
        int backspace = GetKeyPressed();
        if (key >= 32 && key <= 125 && char_count < MAX_FILE_PATH_CHAR)
        {
            // Append character to input_text
            input_text[char_count] = (char)key;
            char_count++;
        }
        else if ((key == KEY_BACKSPACE || backspace == KEY_BACKSPACE) && char_count > 0)
        {
            // Remove last character
            char_count--;
            input_text[char_count] = '\0';
        }

        // Finish editing by pressing Enter
        if (IsKeyPressed(KEY_ENTER))
        {
            editing_text = false;
        }
    }

    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);
    DrawCircle(SCREEN_WIDTH * -0.7, SCREEN_HEIGHT * -1, 1380, BLACK);
    DrawCircle(SCREEN_WIDTH * -0.7, SCREEN_HEIGHT * -1, 1350, COLOR_PKMN_RED);

    DrawTextureEx(*settings_texture, (Vector2){50, 35}, 0, 1, WHITE);
    DrawText("Specify folder name containing saves", 50, SCREEN_HEIGHT / 2 - 50, 25, BLACK);

    // Draw the input box
    DrawRectangleRec(input_box_rec, WHITE);
    DrawRectangleLinesEx(input_box_rec, 2, editing_text ? BLACK : DARKGRAY);

    // Draw the text inside the input box
    DrawText(input_text, input_box_rec.x + 10, input_box_rec.y + 12, input_text_size, BLACK);

    Rectangle clear_button_rec = (Rectangle){SCREEN_WIDTH - MeasureText("Clear input", 20) + 10 - 70, input_box_rec.y + 25 + input_box_rec.height - 5, MeasureText("Clear input", 20) + 10, 30};
    DrawRectangleRec(clear_button_rec, selected_index == SCREEN_BUTTON_CLEAR ? LIGHTGRAY : RED);
    DrawText("Clear input", clear_button_rec.x + 5, clear_button_rec.y + 5, 20, WHITE);

    // Draw the cursor
    if (editing_text)
    {
        DrawLine(input_box_rec.x + 12 + MeasureText(input_text, input_text_size), input_box_rec.y + 10,
                 input_box_rec.x + 12 + MeasureText(input_text, input_text_size), input_box_rec.y + 30, BLACK);
    }

    // Draw reset default config button
    const char *reset_config_text = "Reset to defaults";
    Rectangle reset_config_rec = (Rectangle){input_box_rec.x, input_box_rec.y + 25 + input_box_rec.height - 5, MeasureText(reset_config_text, 20) + 10, 30};
    DrawText(reset_config_text, reset_config_rec.x, reset_config_rec.y, 20, selected_index == BUTTON_RESET ? settings_text_color_selected : settings_text_color);

    // Draw the save button
    const Rectangle save_button_rec = (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT};
    if (selected_index == SCREEN_BUTTON_SAVE)
    {
        DrawText("Save!", save_button_rec.x, save_button_rec.y, 20, LIGHTGRAY);
    }
    else
    {
        DrawText("Save!", save_button_rec.x, save_button_rec.y, 20, char_count ? BLACK : LIGHTGRAY);
    }

    if (errno != 0)
        error_handler(0, TextFormat("error writing config %d", errno));

    const Rectangle back_button_rec = (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT};
    DrawText("< Back", back_button_rec.x, back_button_rec.y, 20, selected_index == SCREEN_BUTTON_BACK ? LIGHTGRAY : BLACK);

    if (show_reset_modal)
    {
        const char *reset_modal_text = "Are you sure you want to reset all settings to default?";
        const char *details_text = "";

        draw_confirmation_modal(reset_modal_text, details_text, "Reset", on_reset_modal_submit, on_reset_modal_cancel, E_MODAL_INFO);
    }

    EndDrawing();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !show_reset_modal)
    {
        if (CheckCollisionPointRec(GetMousePosition(), back_button_rec))
        {
            selected_index = SCREEN_BUTTON_BACK;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), save_button_rec))
        {
            selected_index = SCREEN_BUTTON_SAVE;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), clear_button_rec))
        {
            selected_index = SCREEN_BUTTON_CLEAR;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), reset_config_rec))
        {
            selected_index = BUTTON_RESET;
        }
        else
        {
            selected_index = SCREEN_BUTTON_NONE;
        }
    }

    static bool has_pressed_clear = false;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        switch (selected_index)
        {
        case SCREEN_BUTTON_BACK:
            if (CheckCollisionPointRec(GetMousePosition(), back_button_rec))
            {
                if (has_pressed_clear)
                {
                    strcpy(input_text, input_text_backup);
                }
                has_pressed_clear = false;
                *current_screen = SCREEN_SETTINGS;
                has_shown_placeholder = false;
            }
            selected_index = SCREEN_BUTTON_NONE;
            break;
        case SCREEN_BUTTON_SAVE:
            if (CheckCollisionPointRec(GetMousePosition(), save_button_rec) && char_count > 0)
            {
                file_op_result = write_key_to_config("SAVE_FILE_DIR", input_text);
                if (file_op_result == FILE_OP_SUCCESS)
                {
                    strcpy((char *)save_file_data->save_dir, input_text);
                    save_file_data->num_saves = 0;
                    *current_screen = SCREEN_SETTINGS;
                    has_shown_placeholder = false;
                }
                selected_index = SCREEN_BUTTON_NONE;
            }
            break;
        case SCREEN_BUTTON_CLEAR:
            if (CheckCollisionPointRec(GetMousePosition(), clear_button_rec) && char_count > 0)
            {
                // Clear the input text
                memset(input_text, 0, sizeof(input_text));

                char_count = 0;
                editing_text = true;
                has_shown_placeholder = true;
                file_op_result = FILE_OP_FAILURE;
                has_pressed_clear = true;
            }
            selected_index = SCREEN_BUTTON_NONE;
            break;
        case BUTTON_RESET:
            if (CheckCollisionPointRec(GetMousePosition(), reset_config_rec))
            {
                show_reset_modal = true;
            }
            selected_index = SCREEN_BUTTON_NONE;
            break;
        default:
            break;
        }
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (has_pressed_clear)
        {
            strcpy(input_text, input_text_backup);
        }
        has_pressed_clear = false;
        *current_screen = SCREEN_SETTINGS;
        has_shown_placeholder = false;
        selected_index = SCREEN_BUTTON_NONE;
    }
}
