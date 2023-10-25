#include "raylibhelper.h"
#include "filehelper.h"
#include "pksavhelper.h"
#include "pksavfilehelper.h"
#include "chelper.h"
#ifdef _WIN32
#include <errno.h>
#else
#include <sys/errno.h>
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#else

#endif
#endif

const Rectangle input_box_rec = (Rectangle){50, SCREEN_HEIGHT / 2 +5, SCREEN_WIDTH - 100, 40};
const Color background_color = RAYWHITE;
static bool should_close_window = false;
static GameScreen current_screen = SCREEN_MAIN_MENU;
static int no_dir_err = 0;
static bool show_delete_modal = false;
static bool was_data_deleted = false;
static bool show_reset_modal = false;
static struct SaveFileData *_save_file_data = NULL;
static bool is_build_prerelease = false;
static bool is_same_generation = true;
static int rand_console_index_1 = -1;
static int rand_console_index_2 = -1;
static int rand_pokeball_index = -1;
static int anim_from_right[4] = {300, 300, 300, 300};
static int active_anim_index = -1;
static int active_hover_index = -1;
static int selected_main_menu_index = -1;

Texture2D pkrom_trader_logo;
Texture2D trade;
Texture2D evolve;
Texture2D settings;
Texture2D quit;
static Texture2D consoles[10];
static Texture2D pk_balls[4];

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
    create_default_config(true);
    init_settings_from_config(_save_file_data);
    show_reset_modal = false;
}

bool draw_menu_button(int x, int y, const char *text, int text_size, int index)
{
    const int rec_height = 45;
    const int line_width = 3;
    bool clicked = false;
    int selected_offset = 0;
    Color selected_color = COLOR_PKMN_YELLOW;

    // on mouse hover
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){x, y, 150, rec_height}))
    {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            clicked = true;
            selected_offset = 2;
            selected_color = COLOR_PKMN_DARKYELLOW;
            selected_main_menu_index = index;
        }

        // Shadow
        if (!clicked)
            DrawRectangleGradientH(x + 140, y - 2 + 2, 6, rec_height + 6, DARKGRAY, (Color){0, 0, 0, 0});
        if (!clicked)
            DrawRectangleGradientV(x - 2, y + rec_height + 1, 142, 6, DARKGRAY, (Color){0, 0, 0, 0});

        // Hovered box
        DrawTriangle((Vector2){x - 10 + selected_offset, y - 2}, (Vector2){x - 20 + selected_offset, y - 2}, (Vector2){x - 10 + selected_offset, y + rec_height + 1}, selected_color);
        DrawRectangle(x - 10 + selected_offset, y - 2, 150, rec_height + 4, selected_color);
        DrawText(text, x + selected_offset, y + 7, text_size, BLACK);

        // Box outline
        DrawLineEx((Vector2){x - 20 + selected_offset, y - 2}, (Vector2){x + 140 + selected_offset, y - 2}, line_width, BLACK);
        DrawLineEx((Vector2){x - 10 + selected_offset, y + rec_height + 1}, (Vector2){x + 140 + selected_offset, y + rec_height + 1}, line_width, BLACK);
        DrawLineEx((Vector2){x - 20 + selected_offset, y - 2}, (Vector2){x - 10 + selected_offset, y + rec_height + 1}, line_width, BLACK);
        DrawLineEx((Vector2){x + 140 + selected_offset, y - 2}, (Vector2){x + 140 + selected_offset, y + rec_height + 2}, line_width, BLACK);

        return true;
    }
    else
    {
        DrawText(text, x + 10, y + 5, text_size, BLACK);
        // TODO: bad ux where pressing button and moving
        // mouse focus off button will still select button
        // on mouse button release. Adding next line will
        // stop the button from working
        // selected_main_menu_index = -1;
        clicked = false;
        selected_color = COLOR_PKMN_YELLOW;
        selected_offset = 0;
    }

    return false;
}

void _draw_confirmation_modal(const char *header_text, const char *body_text, const char *submit_button_text, void (*_onsubmit)(void), void (*_oncancel)(void), enum E_MODAL_TYPES modal_type)
{
    enum buttons
    {
        BUTTON_NONE = -1,
        BUTTON_CANCEL,
        BUTTON_SUBMIT
    };
    static int selected_index = BUTTON_NONE;
    // Default string values
    char _header_text[60];
    set_default_value_string(header_text, "Are you sure you want to do this action?", _header_text);
    char _body_text[64];
    set_default_value_string(body_text, "This action cannot be undone.", _body_text);
    char submit_text[11];
    set_default_value_string(submit_button_text, "Confirm", submit_text);

    const Color scrim = (Color){0, 0, 0, 30};
    // Background scrim
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, scrim);

    // Modal Rectangle
    Rectangle confirm_modal_rec = (Rectangle){SCREEN_WIDTH / 2 - MeasureText(_header_text, 20) / 2 - 50, SCREEN_HEIGHT / 2 - 150, MeasureText(_header_text, 20) + 100, 300};
    // Bottom Shadow
    DrawRectangleGradientV(confirm_modal_rec.x + 4, confirm_modal_rec.y + confirm_modal_rec.height - 2, confirm_modal_rec.width - 5, 6, LIGHTGRAY, (Color){0, 0, 0, 0});
    // Right Shadow
    DrawRectangleGradientH(confirm_modal_rec.x + confirm_modal_rec.width - 2, confirm_modal_rec.y + 6, 6, confirm_modal_rec.height - 3, LIGHTGRAY, (Color){0, 0, 0, 0});

    // Draw the modal box
    DrawRectangleRec(confirm_modal_rec, WHITE);
    DrawRectangleLinesEx(confirm_modal_rec, 2, LIGHTGRAY);

    if (modal_type == E_MODAL_WARN)
    {
        // Draw Triangle lines
        DrawTriangle(
            (Vector2){confirm_modal_rec.x + confirm_modal_rec.width / 2 - 50, confirm_modal_rec.y + 110},
            (Vector2){confirm_modal_rec.x + confirm_modal_rec.width / 2 + 50, confirm_modal_rec.y + 110},
            (Vector2){confirm_modal_rec.x + confirm_modal_rec.width / 2, confirm_modal_rec.y + 25},
            RED);
        // Draw smaller white triangle to cover the red triangle lines
        DrawTriangle(
            (Vector2){confirm_modal_rec.x + confirm_modal_rec.width / 2 - 40, confirm_modal_rec.y + 104.5},
            (Vector2){confirm_modal_rec.x + confirm_modal_rec.width / 2 + 40, confirm_modal_rec.y + 104.5},
            (Vector2){confirm_modal_rec.x + confirm_modal_rec.width / 2, confirm_modal_rec.y + 35},
            WHITE);
    }

    if (modal_type == E_MODAL_INFO)
    {
        DrawCircle(confirm_modal_rec.x + confirm_modal_rec.width / 2, confirm_modal_rec.y + 75, 45, LIGHTGRAY);
        DrawCircle(confirm_modal_rec.x + confirm_modal_rec.width / 2, confirm_modal_rec.y + 75, 35, WHITE);
    }

    // Draw exclamation point in triangle
    DrawRectangleRounded((Rectangle){confirm_modal_rec.x + confirm_modal_rec.width / 2 - 3.5, confirm_modal_rec.y + 47, 7, 37}, 1, 5, modal_type == E_MODAL_WARN ? RED : LIGHTGRAY);
    DrawCircle(confirm_modal_rec.x + confirm_modal_rec.width / 2, confirm_modal_rec.y + 95, 5, modal_type == E_MODAL_WARN ? RED : LIGHTGRAY);

    // Modal Text
    DrawText(_header_text, confirm_modal_rec.x + confirm_modal_rec.width / 2 - MeasureText(_header_text, 22) / 2, confirm_modal_rec.y + 135, 22, BLACK);
    DrawText(_body_text, confirm_modal_rec.x + confirm_modal_rec.width / 2 - MeasureText(_body_text, 19) / 2, confirm_modal_rec.y + 185, 19, BLACK);

    // Draw Submit Button
    Rectangle submit_button_rec = (Rectangle){confirm_modal_rec.x + confirm_modal_rec.width - 200, confirm_modal_rec.y + confirm_modal_rec.height - 65, MeasureText(submit_text, 20) + 30, 40};
    DrawRectangleRec(submit_button_rec, selected_index == BUTTON_SUBMIT ? LIGHTGRAY : modal_type == E_MODAL_WARN ? RED
                                                                                                     : COLOR_PKMN_GREEN);
    // Button shadow
    if (selected_index != BUTTON_SUBMIT)
        DrawLine(submit_button_rec.x + submit_button_rec.width + 1, submit_button_rec.y + 1, submit_button_rec.x + submit_button_rec.width + 1, submit_button_rec.y + submit_button_rec.height, BLACK);
    if (selected_index != BUTTON_SUBMIT)
        DrawLine(submit_button_rec.x + 1, submit_button_rec.y + submit_button_rec.height + 1, submit_button_rec.x + submit_button_rec.width + 1, submit_button_rec.y + submit_button_rec.height + 1, BLACK);

    // Draw Cancel Button
    const char *cancel_button_text = "Cancel";
    Rectangle cancel_button_rec = (Rectangle){confirm_modal_rec.x + 100, submit_button_rec.y, MeasureText(cancel_button_text, 20) + 10, 30};

    DrawText(submit_text, submit_button_rec.x + 15, submit_button_rec.y + 10, 20, selected_index == BUTTON_SUBMIT ? DARKGRAY : BLACK);
    DrawText(cancel_button_text, cancel_button_rec.x + 5, cancel_button_rec.y + 5, 20, selected_index == BUTTON_CANCEL ? LIGHTGRAY : BLACK);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), submit_button_rec))
        {
            selected_index = BUTTON_SUBMIT;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), cancel_button_rec))
        {
            selected_index = BUTTON_CANCEL;
        }
        else
        {
            selected_index = BUTTON_NONE;
        }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        switch (selected_index)
        {
        case 0:
            if (CheckCollisionPointRec(GetMousePosition(), cancel_button_rec) && selected_index == BUTTON_CANCEL)
            {
                _oncancel();
                selected_index = BUTTON_NONE;
                break;
            }
        case 1:
            if (CheckCollisionPointRec(GetMousePosition(), submit_button_rec) && selected_index == BUTTON_SUBMIT)
            {
                _onsubmit();
                selected_index = BUTTON_NONE;
                break;
            }
        default:
            break;
        }
    }
}

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
// Draws the trainers name, id, and party pokemon in pokemon buttons
void draw_trainer_info(struct TrainerInfo *trainer, int x, int y, struct TrainerSelection trainerSelection[2], bool showGender)
{
    // Get trainer generation 1 or 2
    SaveGenerationType trainer_generation = trainer->trainer_generation;

    // Create the trainer name and id strings for Raylib drawing
    char trainer_name[15];
    create_trainer_name_str(trainer, trainer_name, showGender);
    char trainer_id[11];
    create_trainer_id_str(trainer, trainer_id);
    int current_trainer_index = trainerSelection[0].trainer_id == trainer->trainer_id ? 0 : trainerSelection[1].trainer_id == trainer->trainer_id ? 1
                                                                                                                                                  : -1;
    // Get the party count for drawing the pokemon buttons
    int party_count = 0;
    if (trainer_generation == SAVE_GENERATION_1)
    {
        party_count = trainer->pokemon_party.gen1_pokemon_party.count;
    }
    else if (trainer_generation == SAVE_GENERATION_2)
    {
        party_count = trainer->pokemon_party.gen2_pokemon_party.count;
    }

    DrawText(trainer_name, x, y, 20, BLACK);
    DrawText(trainer_id, x, y + 30, 20, BLACK);

    // Draw the pokemon buttons
    for (int party_index = 0; party_index < party_count; party_index++)
    {
        enum eligible_trade_status trade_status = check_trade_eligibility(trainer, party_index);

        char pokemon_nickname[11] = "\0";
        if (trainer_generation == SAVE_GENERATION_1)
        {
            pksav_gen1_import_text(trainer->pokemon_party.gen1_pokemon_party.nicknames[party_index], pokemon_nickname, 10);
        }
        else if (trainer_generation == SAVE_GENERATION_2)
        {
            pksav_gen2_import_text(trainer->pokemon_party.gen2_pokemon_party.nicknames[party_index], pokemon_nickname, 10);
        }

        draw_pkmn_button((Rectangle){x - 10, y + 70 + (party_index * 30), 200, 30}, party_index, pokemon_nickname, current_trainer_index != -1 && (trainerSelection[current_trainer_index].pkmn_party_index == party_index || trade_status != E_TRADE_STATUS_ELIGIBLE));
        if (trade_status == E_TRADE_STATUS_GEN2_PKMN)
            DrawText("Gen 2 only ", x + MeasureText(pokemon_nickname, 20) + 5, y + 70 + (party_index * 30), 20, RED);
        else if (trade_status == E_TRADE_STATUS_GEN2_MOVE)
            DrawText("Gen 2 move ", x + MeasureText(pokemon_nickname, 20) + 5, y + 70 + (party_index * 30), 20, RED);
        else if (trade_status == E_TRADE_STATUS_HM_MOVE)
            DrawText("HM move", x + MeasureText(pokemon_nickname, 20) + 5, y + 70 + (party_index * 30), 20, RED);

        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){x - 10, y + 70 + (party_index * 30), 200, 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (trainerSelection[current_trainer_index].pkmn_party_index != party_index && trade_status == E_TRADE_STATUS_ELIGIBLE)
            {
                trainerSelection[current_trainer_index].pkmn_party_index = party_index;
            }
            else
            {
                trainerSelection[current_trainer_index].pkmn_party_index = -1;
            }
        }
    }

    // Draw the selected pokemon nickname
    if (current_trainer_index != -1 && trainerSelection[current_trainer_index].pkmn_party_index != -1)
    {
        // Name of the pokemon selected from list
        static char selected_pokemon_nickname[11];
        if (trainer_generation == SAVE_GENERATION_1)
        {
            pksav_gen1_import_text(trainer->pokemon_party.gen1_pokemon_party.nicknames[trainerSelection[current_trainer_index].pkmn_party_index], selected_pokemon_nickname, 10);
        }
        else if (trainer_generation == SAVE_GENERATION_2)
        {
            pksav_gen2_import_text(trainer->pokemon_party.gen2_pokemon_party.nicknames[trainerSelection[current_trainer_index].pkmn_party_index], selected_pokemon_nickname, 10);
        }
        DrawText(selected_pokemon_nickname, trainerSelection[current_trainer_index].trainer_index ? (GetScreenWidth() / 2) + 50 : x, y + 300, 20, BLACK);
    }
}

void draw_about(void)
{
    int x = 50;
    enum screen_buttons
    {
        SCREEN_BUTTON_NONE = -1,
        SCREEN_BUTTON_BACK,
        SCREEN_BUTTON_NEXT
    };
    static int selected_index = SCREEN_BUTTON_NONE;
    BeginDrawing();
    ClearBackground(background_color);

    DrawText("About Pokerom Trader", x, 100, 20, BLACK);
    DrawText(TextFormat("v%s%s", PROJECT_VERSION, is_build_prerelease ? "-prerelease" : ""), x, 125, 20, BLACK);
    DrawText("Pokerom Trader is a tool for trading Pokemon between two save files", x, 200, 20, BLACK);
    DrawText("written by github.com/savaughn 2023", x, 225, 20, BLACK);
    DrawText("Pokerom Trader is open source and licensed under the MIT license", x, 250, 20, BLACK);
    DrawText("Pokerom Trader uses the following libraries:", x, 300, 20, BLACK);
    DrawText("raylib - https://www.raylib.com/", x, 325, 20, BLACK);
    DrawText("pksav - https://github.com/ncorgan/pksav", x, 350, 20, BLACK);
    DrawText("Legal >", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, selected_index == SCREEN_BUTTON_NEXT ? LIGHTGRAY : BLACK);
    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, selected_index == SCREEN_BUTTON_BACK ? LIGHTGRAY : BLACK);

    EndDrawing();

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
        {
            selected_index = SCREEN_BUTTON_NEXT;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
        {
            selected_index = SCREEN_BUTTON_BACK;
        }
        else
        {
            selected_index = SCREEN_BUTTON_NONE;
        }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        switch (selected_index)
        {
        case 0:
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
            {
                current_screen = SCREEN_SETTINGS;
                selected_index = SCREEN_BUTTON_NONE;
            }
            break;
        case 1:
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
            {
                current_screen = SCREEN_LEGAL;
                selected_index = SCREEN_BUTTON_NONE;
            }
            break;
        default:
            break;
        }
    }
}
void draw_legal(void)
{
    const char *disclaimer_lines[] = {
        "Pokerom Trader is an unofficial application and is not",
        "affiliated with or endorsed by Nintendo, Game Freak, Creatures,",
        "The Pokémon Company, or any related entities. Pokémon and Pokémon",
        "character names are trademarks of Nintendo, Game Freak, Creatures,",
        "and The Pokémon Company. All trademarks, character names, and other",
        "intellectual property used in this application are used for",
        "identification and informational purposes only. The use of",
        "these names and marks is believed to qualify as fair use under",
        "trademark law. Pokerom Trader is not endorsed by or affiliated with",
        "any of the aforementioned entities. Pokerom Trader is provided \"as is\"",
        "without warranty of any kind, and the developers make no warranties,",
        "express or implied, regarding the accuracy or completeness of the",
        "content provided in this application."};

    enum screen_buttons
    {
        SCREEN_BUTTON_NONE = -1,
        SCREEN_BUTTON_BACK
    };
    static int selected_index = SCREEN_BUTTON_NONE;

    BeginDrawing();
    ClearBackground(background_color);
    DrawText("Disclaimer", 50, 50, 20, BLACK);
    for (int i = 0; i < 13; i++)
    {
        DrawText(disclaimer_lines[i], 50, 75 + (i * 25), 20, BLACK);
    }

    const Rectangle back_button_rec = (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT};
    DrawText("< Back", back_button_rec.x + 15, back_button_rec.y + 30, 20, selected_index == SCREEN_BUTTON_BACK ? LIGHTGRAY : BLACK);
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), back_button_rec))
        {
            selected_index = SCREEN_BUTTON_BACK;
        }
        else
        {
            selected_index = SCREEN_BUTTON_NONE;
        }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), back_button_rec))
        {
            current_screen = SCREEN_ABOUT;
            selected_index = SCREEN_BUTTON_NONE;
        }
    }
    EndDrawing();
}
void draw_change_dir(struct SaveFileData *save_file_data)
{
    static bool editing_text = false;
    static char input_text[MAX_FILE_PATH_CHAR];
    static int text_size = 0;
    static bool has_shown_placeholder = false;
    static bool has_pressed_clear = false;
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
    ClearBackground(background_color);
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
                current_screen = SCREEN_SETTINGS;
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
                    current_screen = SCREEN_SETTINGS;
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
void draw_settings(void)
{
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
    ClearBackground(COLOR_PKMN_RED);
    DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 3.5, 1350, BLACK);
    DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 3.5, 1320, WHITE);

    DrawTextureEx(settings, (Vector2){50, 50}, 0, 0.4, WHITE);
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
        case 0:
            if (CheckCollisionPointRec(GetMousePosition(), change_dir_rec))
            {
                current_screen = SCREEN_FILE_EDIT;
            }
            selected_index = BUTTON_NONE;
            break;
        case 1:
            if (CheckCollisionPointRec(GetMousePosition(), about_button_rec))
            {
                current_screen = SCREEN_ABOUT;
            }
            selected_index = BUTTON_NONE;
            break;
        case 2:
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
            {
                current_screen = SCREEN_MAIN_MENU;
            }
            selected_index = BUTTON_NONE;
            break;
        case 3:
            if (CheckCollisionPointRec(GetMousePosition(), delete_app_data_rec) && !was_data_deleted)
            {
                show_delete_modal = true;
            }
            selected_index = BUTTON_NONE;
            break;
        case 4:
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

        _draw_confirmation_modal(delete_modal_text, details_text, "Delete", on_delete_modal_submit, on_delete_modal_cancel, E_MODAL_WARN);
    }
    else if (show_reset_modal)
    {
        const char *reset_modal_text = "Are you sure you want to reset all settings to default?";
        const char *details_text = "";

        _draw_confirmation_modal(reset_modal_text, details_text, "Reset", on_reset_modal_submit, on_reset_modal_cancel, E_MODAL_INFO);
    }

    EndDrawing();
}

void draw_main_menu(struct SaveFileData *save_file_data)
{
    BeginDrawing();
    ClearBackground(COLOR_PKMN_RED);
    DrawCircle(SCREEN_WIDTH * 1.15, SCREEN_HEIGHT * 1.725, 800, BLACK);
    DrawCircle(SCREEN_WIDTH * 1.15, SCREEN_HEIGHT * 1.725, 730, WHITE);

    const int start_y = 200;
    const int start_x = 75;
    const int rec_height_offset = 50;
    const int text_size = 30;
    const uint8_t anim_speed = 45;

    if (consoles[0].id == 0)
    {
        for (int i = 0; i < 10; i++)
        {
            consoles[i] = LoadTextureFromImage(LoadImage(TextFormat("assets/images/Pixel_Fantasy_Icons_Consoles/Consoles/console_%d.png", i)));
        }
    }
    if (pk_balls[0].id == 0)
    {
        for (int i = 0; i < 4; i++)
        {
            pk_balls[i] = LoadTextureFromImage(LoadImage(TextFormat("assets/images/pokeballs_MPR/ball_%d.png", i)));
        }
    }

    const Rectangle details_rec = (Rectangle){SCREEN_WIDTH * 0.55, SCREEN_HEIGHT * 0.35, 200, 200};
    const Vector2 details_text = (Vector2){details_rec.x - 100, SCREEN_HEIGHT - 30};
    const uint8_t console_x_offset = 70;
    static uint8_t anim_index = 0;

    // if (SHOW_BILLS_PC && CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 300, 200, 20}))
    //          get_save_files(save_file_data);
    //          current_screen = SCREEN_BILLS_PC_FILE_SELECT;

    // Draw image pkrom_trader_logo
    DrawTextureEx(pkrom_trader_logo, (Vector2){50, 50}, 0, 0.62, WHITE);
    if (draw_menu_button(start_x, start_y, "Trade", text_size, 0))
    {
        active_anim_index = 0;
        active_hover_index = 0;
    }
    else
    {
        active_hover_index = -1;
    }
    if (active_anim_index == 0)
    {
        static int frame_counter = 0;
        if (rand_console_index_1 == -1)
            rand_console_index_1 = GetRandomValue(0, 9);
        if (rand_console_index_2 == -1)
            rand_console_index_2 = GetRandomValue(0, 9);

        DrawTextureEx(trade, (Vector2){details_rec.x + anim_from_right[0], details_rec.y + 25}, 0, 0.5, WHITE);
        DrawTextureEx(consoles[rand_console_index_1], (Vector2){details_rec.x + anim_from_right[0] - 40, details_rec.y + 150}, 0, 3, WHITE);

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
        if (active_hover_index == 0 && anim_from_right[0] >= 0)
            anim_from_right[0] -= anim_speed;
        if (active_hover_index == -1)
            anim_from_right[0] += anim_speed;
        if (anim_from_right[0] > 300)
        {
            active_anim_index = -1;
            anim_from_right[0] = 300;
        }
        anim_index %= 9;
        frame_counter++;

        DrawTextureEx(consoles[rand_console_index_2], (Vector2){details_rec.x + anim_from_right[0] + console_x_offset + 120, details_rec.y + 150}, 0, 3, WHITE);
        DrawText("Trade pokemon between save files", details_text.x + anim_from_right[0] + 40, details_text.y, 20, BLACK);
    }
    else
    {
        rand_console_index_1 = -1;
        rand_console_index_2 = -1;
        anim_from_right[0] = 300;
    }
    if (SHOW_BILLS_PC)
        DrawText("Bill's PC", 190, 300, 20, BLACK);
    if (draw_menu_button(start_x + 15, start_y + rec_height_offset, "Evolve", text_size, 1))
    {
        active_anim_index = 1;
        active_hover_index = 1;
    }
    else
    {
        active_hover_index = -1;
    }
    if (active_anim_index == 1)
    {
        static int frame_counter = 0;
        if (rand_pokeball_index == -1)
            rand_pokeball_index = GetRandomValue(0, 3);

        DrawTextureEx(evolve, (Vector2){details_rec.x + anim_from_right[1], details_rec.y + 25}, 0, 0.5, WHITE);
        DrawTextureEx(pk_balls[rand_pokeball_index], (Vector2){details_rec.x + anim_from_right[1] + 15, details_rec.y + 80}, 0, 0.25, WHITE);
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
        if (active_hover_index == 1 && anim_from_right[1] >= 0)
            anim_from_right[1] -= anim_speed;
        if (active_hover_index == -1)
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
    if (draw_menu_button(start_x + 30, start_y + (rec_height_offset * 2), "Settings", text_size, 2))
    {
        active_anim_index = 2;
        active_hover_index = 2;
    }
    else
    {
        active_hover_index = -1;
    }
    if (active_anim_index == 2)
    {
        DrawTextureEx(settings, (Vector2){details_rec.x + anim_from_right[2] - 25, details_rec.y + 100}, 0, 0.5, WHITE);
        DrawText("Change trade and evolution settings", details_text.x + anim_from_right[2] + 40, details_text.y, 20, BLACK);
        if (active_hover_index == 2 && anim_from_right[2] >= 0)
            anim_from_right[2] -= anim_speed;
        if (active_hover_index == -1)
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
    if (draw_menu_button(start_x + 45, start_y + (rec_height_offset * 3), "Quit", text_size, 3))
    {
        active_anim_index = 3;
        active_hover_index = 3;
    }
    else
    {
        active_hover_index = -1;
    }
    if (active_anim_index == 3)
    {
        DrawTextureEx(quit, (Vector2){details_rec.x + anim_from_right[3], details_rec.y + 100}, 0, 0.5, WHITE);
        DrawText("Quit Pokerom Trader", details_text.x + anim_from_right[3] + 80, details_text.y, 20, BLACK);
        if (active_hover_index == 3 && anim_from_right[3] >= 0)
            anim_from_right[3] -= anim_speed;
        if (active_hover_index == -1)
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
        case 0:
        {
            int err = get_save_files(save_file_data);
            no_dir_err = err;
            current_screen = SCREEN_FILE_SELECT;
            selected_main_menu_index = -1;
            anim_from_right[0] = 300;
            break;
        }
        case 1:
        {
            get_save_files(save_file_data);
            current_screen = SCREEN_EVOLVE_FILE_SELECT;
            selected_main_menu_index = -1;
            anim_from_right[1] = 300;
            break;
        }
        case 2:
        {
            current_screen = SCREEN_SETTINGS;
            selected_main_menu_index = -1;
            anim_from_right[2] = 300;
            break;
        }
        case 3:
        {
            should_close_window = true;
            selected_main_menu_index = -1;
            anim_from_right[3] = 300;
            break;
        }
        default:
            selected_main_menu_index = -1;
            break;
        }
    }

    EndDrawing();
}
void draw_file_select(struct SaveFileData *save_file_data, char *player1_save_path, char *player2_save_path, struct TrainerInfo *trainer1, struct TrainerInfo *trainer2, struct TrainerSelection trainerSelection[2], PokemonSave *pkmn_save_player1, PokemonSave *pkmn_save_player2)
{
    static int selected_saves_index[2] = {-1, -1};

    BeginDrawing();
    ClearBackground(RAYWHITE);

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

        DrawText("Select two save files of the same generation to trade between", SCREEN_WIDTH / 2 - MeasureText("Select two save files of the same generation to trade between", 20) / 2, 25, 20, BLACK);
        for (int i = 0; i < save_file_data->num_saves; i++)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){100, 75 + 25 * i, SCREEN_WIDTH / 2, 25}))
                {
                    if (selected_saves_index[0] == i)
                    {
                        selected_saves_index[0] = -1;
                    }
                    else if (selected_saves_index[1] == i)
                    {
                        selected_saves_index[1] = -1;
                    }
                    else if (selected_saves_index[0] == -1)
                    {
                        selected_saves_index[0] = i;
                    }
                    else if (selected_saves_index[1] == -1)
                    {
                        selected_saves_index[1] = i;
                    }
                    pkmn_save_player1->save_generation_type = SAVE_GENERATION_NONE;
                    pkmn_save_player2->save_generation_type = SAVE_GENERATION_NONE;
                    has_file_error = false;
                }
            }
            char *save_name = strrchr(save_file_data->saves_file_path[i], '/');
            save_name++;
            DrawText(save_name, 100, 75 + 25 * i, 20, (selected_saves_index[0] == i || selected_saves_index[1] == i) ? LIGHTGRAY : BLACK);
        }
        // Reset generation check
        if (!is_same_generation)
        {
            is_same_generation = selected_saves_index[1] == -1;
        }

        DrawText("Trade >", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, has_selected_two_saves && is_same_generation ? BLACK : LIGHTGRAY);
        if (has_selected_two_saves)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
                {
                    // load selection to player1_save
                    *pkmn_save_player1 = load_savefile_from_path(save_file_data->saves_file_path[selected_saves_index[0]]);

                    // save the selected path name
                    strcpy(player1_save_path, save_file_data->saves_file_path[selected_saves_index[0]]);
                    // generate trainer info from save
                    create_trainer(pkmn_save_player1, trainer1);
                    // save trainer id to trainerSelection
                    trainerSelection[0].trainer_id = trainer1->trainer_id;

                    // load selection to player2_save
                    *pkmn_save_player2 = load_savefile_from_path(save_file_data->saves_file_path[selected_saves_index[1]]);
                    strcpy(player2_save_path, save_file_data->saves_file_path[selected_saves_index[1]]);
                    create_trainer(pkmn_save_player2, trainer2);
                    trainerSelection[1].trainer_id = trainer2->trainer_id;

                    if (pkmn_save_player1->save_generation_type == SAVE_GENERATION_CORRUPTED || pkmn_save_player2->save_generation_type == SAVE_GENERATION_CORRUPTED)
                    {
                        has_file_error = true;
                    }
                    else
                    {
                        is_same_generation = pkmn_save_player1->save_generation_type == pkmn_save_player2->save_generation_type;
                        current_screen = SCREEN_TRADE;
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
    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            current_screen = SCREEN_MAIN_MENU;
            selected_saves_index[0] = -1;
            selected_saves_index[1] = -1;
            trainer1->trainer_id = 0;
            trainer2->trainer_id = 0;
        }
    }

    // change directory button between back and next buttons in line horizontally centered vertically
    int button_width = MeasureText("Change Save Directory", 20) + 20;
    DrawText("Change Save Directory", SCREEN_WIDTH / 2 - button_width / 2, BACK_BUTTON_Y, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){SCREEN_WIDTH / 2 - button_width / 2 - 10, BACK_BUTTON_Y - 30, button_width, BUTTON_HEIGHT}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            current_screen = SCREEN_FILE_EDIT;
            selected_saves_index[0] = -1;
            selected_saves_index[1] = -1;
            trainer1->trainer_id = 0;
            trainer2->trainer_id = 0;
        }
    }

    EndDrawing();
}
void draw_trade(PokemonSave *save_player1, PokemonSave *save_player2, char *player1_save_path, char *player2_save_path, struct TrainerSelection trainerSelection[2], struct TrainerInfo *trainer1, struct TrainerInfo *trainer2)
{
    // Update
    int selected_index_trainer1 = trainerSelection[0].pkmn_party_index;
    int selected_index_trainer2 = trainerSelection[1].pkmn_party_index;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    draw_trainer_info(trainer1, 50, 50, trainerSelection, save_player1->save.gen2_save.save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL);
    draw_trainer_info(trainer2, GetScreenWidth() / 2 + 50, 50, trainerSelection, save_player2->save.gen2_save.save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL);
    uint8_t canSubmitTrade = trainerSelection[0].pkmn_party_index != -1 && trainerSelection[1].pkmn_party_index != -1;
    DrawText("Trade!", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, canSubmitTrade ? BLACK : LIGHTGRAY);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}) && canSubmitTrade)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // Reset Trading state
            trainerSelection[0].pkmn_party_index = -1;
            trainerSelection[1].pkmn_party_index = -1;

            if (is_same_generation)
            {
                swap_pkmn_at_index_between_saves(save_player1, save_player2, selected_index_trainer1, selected_index_trainer2);
            }
            else
            {
                swap_pkmn_at_index_between_saves_cross_gen(save_player1, save_player2, selected_index_trainer1, selected_index_trainer2);
            }
            update_seen_owned_pkmn(save_player1, selected_index_trainer1);
            update_seen_owned_pkmn(save_player2, selected_index_trainer2);
            save_savefile_to_path(save_player1, player1_save_path);
            save_savefile_to_path(save_player2, player2_save_path);

            // Recreate trainer info with updated saves
            create_trainer(save_player1, trainer1);
            create_trainer(save_player2, trainer2);
        }
    }

    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            trainer1->trainer_id = 0;
            trainer2->trainer_id = 0;
            trainerSelection[0].pkmn_party_index = -1;
            trainerSelection[1].pkmn_party_index = -1;
            current_screen = SCREEN_FILE_SELECT;
        }
    }

    EndDrawing();
}
// Draw Bill's PC for moving PC pokemon to party or between boxes
void draw_file_select_single(struct SaveFileData *save_file_data, PokemonSave *save_player1, char *player1_save_path, struct TrainerInfo *trainer1, struct TrainerSelection *trainerSelection, enum single_player_menu_types menu_type)
{

    BeginDrawing();
    ClearBackground(RAYWHITE);

    // select a save file
    static int selected_saves_index = -1;
    bool hasSelectedSave = selected_saves_index != -1 && save_player1->save_generation_type != SAVE_GENERATION_CORRUPTED;

    if (save_file_data->num_saves == 0)
    {
        get_save_files(save_file_data);
        char *save_dir = save_file_data->save_dir;
        DrawText("No save files found in save folder", SCREEN_WIDTH_TEXT_CENTER("No save files found in save folder", 20), 200, 20, BLACK);
        DrawText(TextFormat("%s/", save_dir), SCREEN_WIDTH_TEXT_CENTER(save_dir, 20), 250, 20, BLACK);
    }
    if (menu_type == SINGLE_PLAYER_MENU_TYPE_BILLS_PC)
        DrawText("Select a save file to access Bill's PC", 190, 25, 20, BLACK);
    if (menu_type == SINGLE_PLAYER_MENU_TYPE_EVOLVE)
        DrawText("Select a save file to access your party", 190, 25, 20, BLACK);
    for (int i = 0; i < save_file_data->num_saves; i++)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){100, 75 + 25 * i, SCREEN_WIDTH / 2, 25}))
            {
                if (selected_saves_index == i)
                {
                    selected_saves_index = -1;
                }
                else
                {
                    selected_saves_index = i;
                }
                // Reset when selecting a file after seeing error message
                save_player1->save_generation_type = SAVE_GENERATION_NONE;
            }
        }
        char *save_name = strrchr(save_file_data->saves_file_path[i], '/');
        save_name++;

        DrawText(save_name, 100, 75 + 25 * i, 20, (selected_saves_index == i) ? LIGHTGRAY : BLACK);
        if (menu_type == SINGLE_PLAYER_MENU_TYPE_BILLS_PC)
            DrawText("Open Bill's PC >", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, hasSelectedSave ? BLACK : LIGHTGRAY);
        if (menu_type == SINGLE_PLAYER_MENU_TYPE_EVOLVE)
            DrawText("Open Party >", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, hasSelectedSave ? BLACK : LIGHTGRAY);

        // On selected file next button press
        if (hasSelectedSave)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
                {
                    // load selection to player1_save
                    *save_player1 = load_savefile_from_path(save_file_data->saves_file_path[selected_saves_index]);
                    // save the selected path name
                    strcpy(player1_save_path, save_file_data->saves_file_path[selected_saves_index]);
                    // generate trainer info from save
                    create_trainer(save_player1, trainer1);
                    // save trainer id to trainerSelection
                    trainerSelection->trainer_id = trainer1->trainer_id;

                    if (menu_type == SINGLE_PLAYER_MENU_TYPE_BILLS_PC)
                        current_screen = SCREEN_BILLS_PC;
                    if (menu_type == SINGLE_PLAYER_MENU_TYPE_EVOLVE && save_player1->save_generation_type != SAVE_GENERATION_CORRUPTED)
                        current_screen = SCREEN_EVOLVE;
                }
            }
        }
    }
    if (save_player1->save_generation_type == SAVE_GENERATION_CORRUPTED)
    {
        DrawText("save file invalid", NEXT_BUTTON_X + 5, NEXT_BUTTON_Y + 25, 15, RED);
    }
    if (menu_type == SINGLE_PLAYER_MENU_TYPE_BILLS_PC)
        DrawText("Bill's PC", 25, 25, 20, BLACK);
    if (menu_type == SINGLE_PLAYER_MENU_TYPE_EVOLVE)
        DrawText("Trade Evolve", 25, 25, 20, BLACK);
    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            current_screen = SCREEN_MAIN_MENU;
            selected_saves_index = -1;
        }
    }

    // change directory button
    int button_width = MeasureText("Change Save Directory", 20) + 20;
    DrawText("Change Save Directory", SCREEN_WIDTH / 2 - button_width / 2, BACK_BUTTON_Y, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){SCREEN_WIDTH / 2 - button_width / 2 - 10, BACK_BUTTON_Y - 30, button_width, BUTTON_HEIGHT}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            trainer1->trainer_id = 0;
            trainerSelection->pkmn_party_index = -1;
            current_screen = SCREEN_FILE_EDIT;
        }
    }

    EndDrawing();
    if (current_screen != SCREEN_EVOLVE_FILE_SELECT && current_screen != SCREEN_BILLS_PC_FILE_SELECT)
        selected_saves_index = -1;
}
void draw_evolve(PokemonSave *pkmn_save, char *save_path, struct TrainerInfo *trainer)
{
    SaveGenerationType save_generation_type = pkmn_save->save_generation_type;

    // Call rng
    generate_rand_num_step(save_generation_type);

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Trade Evolve", 50, 50, 20, BLACK);

    int party_count = 0;
    const int NONE = -1;
    static int selected_index = NONE;
    static int result = 0;
    char pokemon_nickname[11];
    int eligible_pokemon_count = 0;
    const int TRAINER_NAME_X = 50;
    const int TRAINER_NAME_Y = 100;

    if (save_generation_type == SAVE_GENERATION_1)
        party_count = pkmn_save->save.gen1_save.pokemon_storage.p_party->count;
    else if (save_generation_type == SAVE_GENERATION_2)
        party_count = pkmn_save->save.gen2_save.pokemon_storage.p_party->count;

    // Create the trainer name and id strings for Raylib drawing
    char trainer_name[15];
    create_trainer_name_str(trainer, trainer_name, false);
    char trainer_id[11];
    create_trainer_id_str(trainer, trainer_id);
    DrawText(trainer_name, TRAINER_NAME_X, TRAINER_NAME_Y, 20, BLACK);
    DrawText(trainer_id, TRAINER_NAME_X, TRAINER_NAME_Y + 25, 20, BLACK);

    // Search party for pkmn eligible for trade evolution
    for (int i = 0; i < party_count; i++)
    {
        if (save_generation_type == SAVE_GENERATION_1)
        {
            result = check_trade_evolution_gen1(pkmn_save, i);
            eligible_pokemon_count++;
            pksav_gen1_import_text(pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[i], pokemon_nickname, 10);
            draw_pkmn_button((Rectangle){TRAINER_NAME_X, TRAINER_NAME_Y + 75 + (i * 30), 200, 30}, i, pokemon_nickname, selected_index == i || result == E_EVO_STATUS_NOT_ELIGIBLE);
        }
        else if (save_generation_type == SAVE_GENERATION_2)
        {
            result = check_trade_evolution_gen2(pkmn_save, i);
            eligible_pokemon_count++;
            pksav_gen2_import_text(pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[i], pokemon_nickname, 10);
            draw_pkmn_button((Rectangle){TRAINER_NAME_X, TRAINER_NAME_Y + 75 + (i * 30), MeasureText(pokemon_nickname, 20) + 10, 30}, i, pokemon_nickname, selected_index == i || result == E_EVO_STATUS_NOT_ELIGIBLE);
            if (result == E_EVO_STATUS_MISSING_ITEM)
                DrawText("Missing required item!", 75 + MeasureText(pokemon_nickname, 20), TRAINER_NAME_Y + 75 + (i * 30), 20, RED);
        }
        // Selected pokemon button
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){TRAINER_NAME_X, TRAINER_NAME_Y + 75 + (i * 30), 200, 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && result == E_EVO_STATUS_ELIGIBLE)
        {
            // Unselect if already selected
            if (selected_index == i)
            {
                selected_index = NONE;
            }
            // Select a different index
            else
            {
                selected_index = i;
            }
        }
    }

    // Draw selected nickname to the right of the screen
    char selected_nickname[11];
    if (save_generation_type == SAVE_GENERATION_1 && selected_index != NONE)
    {
        pksav_gen1_import_text(pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[selected_index], selected_nickname, 10);
    }
    else if (save_generation_type == SAVE_GENERATION_2 && selected_index != NONE)
    {
        pksav_gen2_import_text(pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[selected_index], selected_nickname, 10);
    }
    if (selected_index != NONE)
        DrawText(selected_nickname, NEXT_BUTTON_X, SCREEN_HEIGHT_TEXT_CENTER(20), 20, BLACK);

    // No eligible pokemon message
    if (eligible_pokemon_count == 0)
    {
        const char *no_pkmn = "No pokemon eligible for trade evolution";
        Vector2 text_center = SCREEN_CENTER(no_pkmn, 20);
        DrawText(no_pkmn, text_center.x, text_center.y, 20, BLACK);
    }
    // Evolve button (next button)
    DrawText("Evolve!", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, selected_index != NONE ? BLACK : LIGHTGRAY);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}) && selected_index != NONE)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // Generates and assigns random dvs for simulated trade to new trainer
            update_pkmn_DVs(pkmn_save, selected_index);
            // Evolve pokemon with the simulated new trainer
            evolve_party_pokemon_at_index(pkmn_save, selected_index);
            // Update pokedex
            update_seen_owned_pkmn(pkmn_save, selected_index);
            // Generates and assigns random dvs on simulated trade back to OT
            update_pkmn_DVs(pkmn_save, selected_index);
            // Finalize pkmn data changes
            save_savefile_to_path(pkmn_save, save_path);

            selected_index = NONE;
        }
    }

    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            current_screen = SCREEN_EVOLVE_FILE_SELECT;
            selected_index = NONE;
        }
    }

    EndDrawing();
}
void draw_bills_pc(PokemonSave *pkmn_save, char *save_path, struct TrainerInfo *trainer, struct TrainerSelection *trainerSelection)
{
    // Update
    int save_generation = pkmn_save->save_generation_type;

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Bill's PC", 50, 50, 20, BLACK);

    draw_trainer_info(trainer, 50, 100, trainerSelection, pkmn_save->save.gen2_save.save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL);

    // Draw a veritcal box to the right of the trainer info
    // labeled box n where n is the box number
    // filled with a list of pokemon names from that box
    // drawn arrows buttons right and left cycle through boxes
    // draw the arrow buttons on the left and right of the box number
    DrawRectangleLines(400, 100, 200, 300, BLACK);
    DrawText("Box 1", 400, 100, 20, BLACK);
    DrawText("<", 400, 150, 20, BLACK);
    DrawText(">", 600, 150, 20, BLACK);

    struct pksav_gen1_pokemon_box *pokemon_box_gen1;
    struct pksav_gen2_pokemon_box *pokemon_box_gen2;
    int box_pokemon_count = 0;
    if (save_generation == SAVE_GENERATION_1)
    {
        pokemon_box_gen1 = pkmn_save->save.gen1_save.pokemon_storage.p_current_box;
        box_pokemon_count = pokemon_box_gen1->count;
    }
    else
    {
        pokemon_box_gen2 = pkmn_save->save.gen2_save.pokemon_storage.p_current_box;
        box_pokemon_count = pokemon_box_gen2->count;
    }

    // list pokemon in the box using draw_pkmn_button
    for (int i = 0; i < box_pokemon_count; i++)
    {
        char pokemon_nickname[11];
        if (save_generation == SAVE_GENERATION_1)
        {
            pksav_gen1_import_text(pokemon_box_gen1->nicknames[i], pokemon_nickname, 10);
        }
        else
        {
            pksav_gen2_import_text(pokemon_box_gen2->nicknames[i], pokemon_nickname, 10);
        }
        draw_pkmn_button((Rectangle){400, 200 + (i * 30), 200, 30}, i, pokemon_nickname, trainerSelection->pkmn_party_index == i);
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){400, 200 + (i * 30), 200, 30}) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            trainerSelection->pkmn_party_index = i;
        }
    }

    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            trainer->trainer_id = 0;
            trainerSelection->pkmn_party_index = -1;
            current_screen = SCREEN_MAIN_MENU;
        }
    }

    EndDrawing();
}
#if defined(__APPLE__)
char *get_mac_resource_images_path(void)
{
    static char *images_path = NULL;
    if (images_path == NULL)
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

        // Construct the path to the "scripts" directory relative to the resources directory
        images_path = malloc(PATH_MAX);
        snprintf(images_path, PATH_MAX, "%s/assets/images", resources_path);

        // Change the current working directory to the resources directory
        if (chdir(resources_path) != 0)
        {
            fprintf(stderr, "Error: could not change working directory to resources directory\n");
            exit(EXIT_FAILURE);
        }
    }
    return images_path;
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
    is_build_prerelease = strcmp(PROJECT_VERSION_TYPE, "prerelease") == 0;

#if defined(__APPLE__)
    if (CI_BUILD)
        get_mac_resource_images_path();
#endif
    pkrom_trader_logo = LoadTextureFromImage(LoadImage("assets/images/logo-text.png"));

    if (trade.id == 0)
    {
        trade = LoadTextureFromImage(LoadImage("assets/images/trade.png"));
    }
    if (evolve.id == 0)
    {
        evolve = LoadTextureFromImage(LoadImage("assets/images/evolve.png"));
    }
    if (settings.id == 0)
    {
        settings = LoadTextureFromImage(LoadImage("assets/images/settings.png"));
    }
    if (quit.id == 0)
    {
        quit = LoadTextureFromImage(LoadImage("assets/images/quit.png"));
    }

    while (!should_close_window && !WindowShouldClose())
    {
        // Escape key to close window
        if (IsKeyPressed(KEY_ESCAPE))
        {
            should_close_window = true;
        }

        switch (current_screen)
        {
        case SCREEN_FILE_SELECT:
            draw_file_select(save_file_data, player1_save_path, player2_save_path, trainer1, trainer2, trainerSelection, pkmn_save_player1, pkmn_save_player2);
            break;
        case SCREEN_TRADE:
            draw_trade(pkmn_save_player1, pkmn_save_player2, player1_save_path, player2_save_path, trainerSelection, trainer1, trainer2);
            break;
        case SCREEN_MAIN_MENU:
            draw_main_menu(save_file_data);
            break;
        case SCREEN_SETTINGS:
            // for confirm modal to update app save file data struct
            _save_file_data = save_file_data;
            draw_settings();
            break;
        case SCREEN_FILE_EDIT:
            draw_change_dir(save_file_data);
            break;
        case SCREEN_BILLS_PC_FILE_SELECT:
            draw_file_select_single(save_file_data, pkmn_save_player1, player1_save_path, trainer1, &trainerSelection[0], SINGLE_PLAYER_MENU_TYPE_BILLS_PC);
            break;
        case SCREEN_BILLS_PC:
            draw_bills_pc(pkmn_save_player1, player1_save_path, trainer1, &trainerSelection[0]);
            break;
        case SCREEN_EVOLVE_FILE_SELECT:
            draw_file_select_single(save_file_data, pkmn_save_player1, player1_save_path, trainer1, &trainerSelection[0], SINGLE_PLAYER_MENU_TYPE_EVOLVE);
            break;
        case SCREEN_EVOLVE:
            draw_evolve(pkmn_save_player1, player1_save_path, trainer1);
            break;
        case SCREEN_ABOUT:
        {
            draw_about();
            break;
        }
        case SCREEN_LEGAL:
        {
            draw_legal();
            break;
        }
        default:
            BeginDrawing();
            ClearBackground(background_color);
            DrawText("Something went wrong", 190, 100, 20, BLACK);
            DrawText("Press ESC to exit", 190, 125, 20, BLACK);
            EndDrawing();
            break;
        }
    }

    UnloadTexture(pkrom_trader_logo);
    UnloadTexture(trade);
    UnloadTexture(evolve);
    UnloadTexture(settings);
    UnloadTexture(quit);
    for (int i = 0; i < 4; i++)
    {
        UnloadTexture(pk_balls[i]);
    }
    for (int i = 0; i < 10; i++)
    {
        UnloadTexture(consoles[i]);
    }

    CloseWindow();
}
