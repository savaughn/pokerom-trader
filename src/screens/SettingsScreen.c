#include "raylibhelper.h"
#include "filehelper.h"

static bool show_delete_modal = false;
static bool was_data_deleted = false;

void on_delete_modal_cancel(void)
{
    show_delete_modal = false;
    was_data_deleted = false;
}

void on_delete_modal_submit(void)
{
    delete_app_data();
    show_delete_modal = false;
    was_data_deleted = true;
}

void draw_settings(struct save_file_data *save_file_data, GameScreen *current_screen, Texture2D *settings_texture)
{
    // _save_file_data = save_file_data;
    const Color settings_text_color = BLACK;
    const Color settings_text_color_selected = LIGHTGRAY;
    const int start_y = 200;
    enum buttons
    {
        BUTTON_NONE = -1,
        BUTTON_CHANGE_DIR,
        BUTTON_ABOUT,
        BUTTON_BACK,
        BUTTON_DELETE,
        BUTTON_RESET
    };
    static int selected_index = BUTTON_NONE;

    BeginDrawing();
    ClearBackground(RED);
    draw_background_grid();
    DrawCircleSector((Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT * 3.5}, 1350, 0, 360, 500, BLACK);
    DrawCircleSector((Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT * 3.5}, 1320, 0, 360, 500, WHITE);

    DrawTextureEx(*settings_texture, (Vector2){50, 35}, 0, 1, WHITE);

    Rectangle change_dir_rec = (Rectangle){50, start_y, 200, 20};
    DrawText("Change Save Directory", change_dir_rec.x, change_dir_rec.y, 20, selected_index == BUTTON_CHANGE_DIR ? settings_text_color_selected : settings_text_color);
    const Rectangle about_button_rec = (Rectangle){50, start_y + 25, MeasureText("About Pokerom Trader", 20) + 10, 30};
    DrawText("About Pokerom Trader", about_button_rec.x, about_button_rec.y, 20, selected_index == BUTTON_ABOUT ? settings_text_color_selected : settings_text_color);

    // Delete app data button
    Rectangle delete_app_data_rec = (Rectangle){SCREEN_WIDTH - MeasureText("Delete app data", 20) + 10 - 75, NEXT_BUTTON_Y - 5, MeasureText("Delete app data", 20) + 10, 30};
    DrawRectangleRec(delete_app_data_rec, show_delete_modal || was_data_deleted || selected_index == BUTTON_DELETE ? LIGHTGRAY : RED);
    DrawText("Delete app data", delete_app_data_rec.x + 5, NEXT_BUTTON_Y, 20, WHITE);
    DrawLine(delete_app_data_rec.x + delete_app_data_rec.width + 1, delete_app_data_rec.y + 1, delete_app_data_rec.x + delete_app_data_rec.width + 1, delete_app_data_rec.y + delete_app_data_rec.height, show_delete_modal || was_data_deleted ? LIGHTGRAY : BLACK);
    DrawLine(delete_app_data_rec.x + 1, delete_app_data_rec.y + delete_app_data_rec.height + 1, delete_app_data_rec.x + delete_app_data_rec.width + 1, delete_app_data_rec.y + delete_app_data_rec.height + 1, show_delete_modal || was_data_deleted ? LIGHTGRAY : BLACK);

    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, selected_index == BUTTON_BACK ? settings_text_color_selected : settings_text_color);
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !show_delete_modal)
    {
        if (CheckCollisionPointRec(GetMousePosition(), change_dir_rec))
        {
            selected_index = BUTTON_CHANGE_DIR;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), about_button_rec))
        {
            selected_index = BUTTON_ABOUT;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
        {
            selected_index = BUTTON_BACK;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), delete_app_data_rec) && !was_data_deleted)
        {
            selected_index = BUTTON_DELETE;
        }
        else
        {
            selected_index = BUTTON_NONE;
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        switch (selected_index)
        {
        case BUTTON_CHANGE_DIR:
            if (CheckCollisionPointRec(GetMousePosition(), change_dir_rec))
            {
                *current_screen = SCREEN_FILE_EDIT;
            }
            selected_index = BUTTON_NONE;
            break;
        case BUTTON_ABOUT:
            if (CheckCollisionPointRec(GetMousePosition(), about_button_rec))
            {
                *current_screen = SCREEN_ABOUT;
            }
            selected_index = BUTTON_NONE;
            break;
        case BUTTON_BACK:
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
            {
                *current_screen = SCREEN_MAIN_MENU;
            }
            selected_index = BUTTON_NONE;
            break;
        case BUTTON_DELETE:
            if (CheckCollisionPointRec(GetMousePosition(), delete_app_data_rec) && !was_data_deleted)
            {
                show_delete_modal = true;
            }
            selected_index = BUTTON_NONE;
            break;
        default:
            break;
        }
    }

    // Draw a delete modal to confirm deleting app data on center screen box with shadow. confirm button cancel button
    if (show_delete_modal)
    {
        const char *delete_modal_text = "Are you sure you want to delete all app data?";
        const char *details_text = "This will delete all save files and settings in app directory.";

        draw_confirmation_modal(delete_modal_text, details_text, "Delete", on_delete_modal_submit, on_delete_modal_cancel, E_MODAL_WARN);
    }

    EndDrawing();

    if (IsKeyPressed(KEY_ESCAPE))
    {
        *current_screen = SCREEN_MAIN_MENU;
        selected_index = BUTTON_NONE;
    }
}
