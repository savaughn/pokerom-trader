#include "raylibhelper.h"
#include "filehelper.h"

static bool show_delete_modal = false;
static bool was_data_deleted = false;
static bool show_reset_modal = false;
static struct save_file_data *_save_file_data = NULL;

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
}

void draw_settings(struct save_file_data *save_file_data, GameScreen *current_screen, Texture2D *settings_texture)
{
    _save_file_data = save_file_data;
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
    DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 3.5, 1350, BLACK);
    DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 3.5, 1320, WHITE);

    DrawTextureEx(*settings_texture, (Vector2){50, 50}, 0, 0.4, WHITE);
    // Toggle for random ivs on trade boolean
    DrawText("Disable random DVs on trade", 50, start_y - 25, 20, settings_text_color);
    // Checkbox for random ivs on trade
    DrawText("ON", 385, start_y - 25, 20, settings_text_color);
    Rectangle checkbox_rec_on = (Rectangle){385 + MeasureText("ON", 20) + 5, start_y - 25, 20, 20};
    DrawRectangleLinesEx(checkbox_rec_on, 2, settings_text_color);
    Rectangle checkbox_rec_off = (Rectangle){checkbox_rec_on.x + checkbox_rec_on.width + 5, start_y - 25, 20, 20};
    DrawRectangleLinesEx(checkbox_rec_off, 2, settings_text_color);
    DrawText("OFF", checkbox_rec_off.x + checkbox_rec_off.width + 5, checkbox_rec_off.y, 20, settings_text_color);
    // Toggle for item override evolutions
    DrawText("Item required for evolution", 50, start_y, 20, settings_text_color);
    // Checkbox for item override evolutions
    DrawText("ON", 385, start_y, 20, settings_text_color);
    Rectangle checkbox_rec_on_item = (Rectangle){385 + MeasureText("ON", 20) + 5, start_y, 20, 20};
    DrawRectangleLinesEx(checkbox_rec_on_item, 2, settings_text_color);
    Rectangle checkbox_rec_off_item = (Rectangle){checkbox_rec_on_item.x + checkbox_rec_on_item.width + 5, start_y, 20, 20};
    DrawRectangleLinesEx(checkbox_rec_off_item, 2, settings_text_color);
    DrawText("OFF", checkbox_rec_off_item.x + checkbox_rec_off_item.width + 5, checkbox_rec_off_item.y, 20, settings_text_color);

    bool _is_rand_disabled = get_is_random_DVs_disabled();
    if (_is_rand_disabled)
    {
        // Draw filled in square
        DrawRectangle(checkbox_rec_on.x + 3, checkbox_rec_on.y + 3, checkbox_rec_on.width - 6, checkbox_rec_on.height - 6, settings_text_color);
        DrawText("DVs will be retained", checkbox_rec_off.x + checkbox_rec_off.width + 65, start_y - 25, 16, settings_text_color);
    }
    else
    {
        // Draw filled in square
        DrawRectangle(checkbox_rec_off.x + 3, checkbox_rec_off.y + 3, checkbox_rec_off.width - 6, checkbox_rec_off.height - 6, settings_text_color);
        DrawText("DVs will not be retained (default)", checkbox_rec_off.x + checkbox_rec_off.width + 65, start_y - 25, 16, settings_text_color);
    }

    bool _is_item_required = get_is_item_required();
    if (_is_item_required)
    {
        // Draw filled in square
        DrawRectangle(checkbox_rec_on_item.x + 3, checkbox_rec_on_item.y + 3, checkbox_rec_on_item.width - 6, checkbox_rec_on_item.height - 6, settings_text_color);
        DrawText("Items will be required (default)", checkbox_rec_off_item.x + checkbox_rec_off_item.width + 65, start_y, 16, settings_text_color);
    }
    else
    {
        // Draw filled in square
        DrawRectangle(checkbox_rec_off_item.x + 3, checkbox_rec_off_item.y + 3, checkbox_rec_off_item.width - 6, checkbox_rec_off_item.height - 6, settings_text_color);
        DrawText("Items will not be required", checkbox_rec_off_item.x + checkbox_rec_off_item.width + 65, start_y, 16, settings_text_color);
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), checkbox_rec_on))
        {
            set_is_random_DVs_disabled(true);
            write_key_to_config("DISABLE_RANDOM_IVS_ON_TRADE", "true");
        }
        else if (CheckCollisionPointRec(GetMousePosition(), checkbox_rec_off))
        {
            set_is_random_DVs_disabled(false);
            write_key_to_config("DISABLE_RANDOM_IVS_ON_TRADE", "false");
        }
        else if (CheckCollisionPointRec(GetMousePosition(), checkbox_rec_on_item))
        {
            set_is_item_required(true);
            write_key_to_config("ITEM_REQUIRED_EVOLUTIONS", "true");
        }
        else if (CheckCollisionPointRec(GetMousePosition(), checkbox_rec_off_item))
        {
            set_is_item_required(false);
            write_key_to_config("ITEM_REQUIRED_EVOLUTIONS", "false");
        }
    }
    Rectangle change_dir_rec = (Rectangle){50, start_y + 75, 200, 20};
    DrawText("Change Save Directory", change_dir_rec.x, change_dir_rec.y, 20, selected_index == BUTTON_CHANGE_DIR ? settings_text_color_selected : settings_text_color);
    // Draw reset default config button
    const char *reset_config_text = "Reset to defaults";
    Rectangle reset_config_rec = (Rectangle){50, start_y + 25, MeasureText(reset_config_text, 20) + 10, 30};
    DrawText(reset_config_text, reset_config_rec.x, reset_config_rec.y, 20, selected_index == BUTTON_RESET ? settings_text_color_selected : settings_text_color);

    const Rectangle about_button_rec = (Rectangle){50, start_y + 100, MeasureText("About Pokerom Trader", 20) + 10, 30};
    DrawText("About Pokerom Trader", about_button_rec.x, about_button_rec.y, 20, selected_index == BUTTON_ABOUT ? settings_text_color_selected : settings_text_color);

    // Delete app data button
    Rectangle delete_app_data_rec = (Rectangle){SCREEN_WIDTH - MeasureText("Delete app data", 20) + 10 - 75, NEXT_BUTTON_Y - 5, MeasureText("Delete app data", 20) + 10, 30};
    DrawRectangleRec(delete_app_data_rec, show_delete_modal || was_data_deleted || selected_index == BUTTON_DELETE ? LIGHTGRAY : RED);
    DrawText("Delete app data", delete_app_data_rec.x + 5, NEXT_BUTTON_Y, 20, WHITE);
    DrawLine(delete_app_data_rec.x + delete_app_data_rec.width + 1, delete_app_data_rec.y + 1, delete_app_data_rec.x + delete_app_data_rec.width + 1, delete_app_data_rec.y + delete_app_data_rec.height, show_delete_modal || was_data_deleted ? LIGHTGRAY : BLACK);
    DrawLine(delete_app_data_rec.x + 1, delete_app_data_rec.y + delete_app_data_rec.height + 1, delete_app_data_rec.x + delete_app_data_rec.width + 1, delete_app_data_rec.y + delete_app_data_rec.height + 1, show_delete_modal || was_data_deleted ? LIGHTGRAY : BLACK);

    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, selected_index == BUTTON_BACK ? settings_text_color_selected : settings_text_color);
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !show_delete_modal && !show_reset_modal)
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
        else if (CheckCollisionPointRec(GetMousePosition(), reset_config_rec))
        {
            selected_index = BUTTON_RESET;
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
        case BUTTON_RESET:
            if (CheckCollisionPointRec(GetMousePosition(), reset_config_rec))
            {
                show_reset_modal = true;
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
    else if (show_reset_modal)
    {
        const char *reset_modal_text = "Are you sure you want to reset all settings to default?";
        const char *details_text = "";

        draw_confirmation_modal(reset_modal_text, details_text, "Reset", on_reset_modal_submit, on_reset_modal_cancel, E_MODAL_INFO);
    }

    EndDrawing();
}
