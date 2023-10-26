#include "raylibhelper.h"
#include "filehelper.h"
#ifdef _WIN32
#include <errno.h>
#else
#include <sys/errno.h>
#endif

void draw_change_dir(struct SaveFileData *save_file_data, GameScreen *current_screen)
{
    static bool editing_text = false;
    static char input_text[MAX_FILE_PATH_CHAR];
    static int text_size = 0;
    static bool has_shown_placeholder = false;
    static int err = 0;
    char input_text_backup[MAX_FILE_PATH_CHAR];
    enum screen_buttons
    {
        SCREEN_BUTTON_NONE = -1,
        SCREEN_BUTTON_BACK,
        SCREEN_BUTTON_SAVE,
        SCREEN_BUTTON_CLEAR
    };
    static int selected_index = SCREEN_BUTTON_NONE;
    const Rectangle input_box_rec = (Rectangle){50, SCREEN_HEIGHT / 2 +5, SCREEN_WIDTH - 100, 40};

    text_size = strlen(input_text);
    strcpy(input_text_backup, (char *)save_file_data->save_dir);

    // Placeholder Text
    if (!editing_text && !has_shown_placeholder)
    {
        strcpy(input_text, (char *)save_file_data->save_dir);
        text_size = strlen(input_text);
        has_shown_placeholder = true;
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
        if (key >= 32 && key <= 125 && text_size < MAX_FILE_PATH_CHAR)
        {
            // Append character to input_text
            input_text[text_size] = (char)key;
            text_size++;
        }
        else if ((key == KEY_BACKSPACE || backspace == KEY_BACKSPACE) && text_size > 0)
        {
            // Remove last character
            text_size--;
            input_text[text_size] = '\0';
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

    DrawTextureEx(settings, (Vector2){50, 50}, 0, 0.4, WHITE);
    DrawText("Specify folder name containing saves", 50, SCREEN_HEIGHT / 2 - 50, 25, BLACK);

    // Draw the input box
    DrawRectangleRec(input_box_rec, WHITE);
    DrawRectangleLinesEx(input_box_rec, 2, editing_text ? BLACK : DARKGRAY);

    // Draw the text inside the input box
    DrawText(input_text, input_box_rec.x + 10, input_box_rec.y + 10, 20, BLACK);

    Rectangle clear_button_rec = (Rectangle){SCREEN_WIDTH - MeasureText("Clear input", 20) + 10 - 70, input_box_rec.y + 25 + input_box_rec.height - 5, MeasureText("Clear input", 20) + 10, 30};
    DrawRectangleRec(clear_button_rec, selected_index == SCREEN_BUTTON_CLEAR ? LIGHTGRAY : RED);
    DrawText("Clear input", clear_button_rec.x + 5, clear_button_rec.y + 5, 20, WHITE);

    // Draw the cursor
    if (editing_text)
    {
        DrawLine(input_box_rec.x + 12 + MeasureText(input_text, 20), input_box_rec.y + 10,
                 input_box_rec.x + 12 + MeasureText(input_text, 20), input_box_rec.y + 30, BLACK);
    }

    // Draw the save button
    const Rectangle save_button_rec = (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT};
    if (selected_index == SCREEN_BUTTON_SAVE)
    {
        DrawText("Save!", save_button_rec.x, save_button_rec.y, 30, LIGHTGRAY);
    }
    else
    {
        DrawText("Save!", save_button_rec.x, save_button_rec.y, 30, text_size ? BLACK : LIGHTGRAY);
    }

    if (err == 1)
        error_handler(0, TextFormat("error writing config %d", errno));

    const Rectangle back_button_rec = (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT};
    DrawText("< Back", back_button_rec.x, back_button_rec.y, 30, selected_index == SCREEN_BUTTON_BACK ? LIGHTGRAY : BLACK);
    EndDrawing();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
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
        else
        {
            selected_index = SCREEN_BUTTON_NONE;
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        static bool has_pressed_clear = false;

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
                selected_index = SCREEN_BUTTON_NONE;
            }
            break;
        case SCREEN_BUTTON_SAVE:
            if (CheckCollisionPointRec(GetMousePosition(), save_button_rec) && text_size > 0)
            {
                err = write_key_to_config("SAVE_FILE_DIR", input_text);
                if (err == 0)
                {
                    strcpy((char *)save_file_data->save_dir, input_text);
                    save_file_data->num_saves = 0;
                    *save_file_data->saves_file_path = NULL;
                    *current_screen = SCREEN_SETTINGS;
                    has_shown_placeholder = false;
                    selected_index = SCREEN_BUTTON_NONE;
                }
            }
            break;
        case SCREEN_BUTTON_CLEAR:
            if (CheckCollisionPointRec(GetMousePosition(), clear_button_rec) && text_size > 0)
            {
                // Clear the input text
                memset(input_text, 0, sizeof(input_text));

                text_size = 0;
                editing_text = true;
                has_shown_placeholder = true;
                err = 0;
                has_pressed_clear = true;
                selected_index = SCREEN_BUTTON_NONE;
            }
            break;
        default:
            selected_index = SCREEN_BUTTON_NONE;
            break;
        }
    }
}
