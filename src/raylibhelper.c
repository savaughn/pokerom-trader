#include "raylibhelper.h"
#include "filehelper.h"
#include "pksavhelper.h"
#include <sys/errno.h>

const Rectangle input_box_rec = (Rectangle){50, SCREEN_HEIGHT / 2 - 20, SCREEN_WIDTH - 100, 40};
const Color background_color = RAYWHITE;
static bool should_close_window = false;
static GameScreen current_screen = SCREEN_MAIN_MENU;
static int no_dir_err = 0;

void PokemonButton(Rectangle rect, int index, char *pokemon_nickname)
{
    bool mouse_over = false;
    bool selected = false;
    if (CheckCollisionPointRec(GetMousePosition(), rect))
    {
        mouse_over = true;
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            selected = true;
        }
    }

    if (mouse_over)
    {
        DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, selected ? LIGHTGRAY : DARKGRAY);
    }

    DrawText(pokemon_nickname, rect.x + 10, rect.y + 6, 20, selected ? LIGHTGRAY : BLACK);
}
void DrawTrainerInfo(struct TrainerInfo *trainer, int x, int y, struct TrainerSelection trainerSelection[2], bool showGender)
{
    SaveGenerationType trainer_generation = trainer->trainer_generation;

    char trainer_name[15];
    createTrainerNameStr(trainer, trainer_name, showGender);
    char trainer_id[11];
    createTrainerIdStr(trainer, trainer_id);
    int current_trainer_index = trainerSelection[0].trainer_id == trainer->trainer_id ? 0 : trainerSelection[1].trainer_id == trainer->trainer_id ? 1
                                                                                                                                                  : -1;
    int party_count = 0;
    if (trainer_generation == SAVE_GENERATION_1)
    {
        party_count = trainer->pokemon_party.gen1_pokemon_party.count;
    }
    else if (trainer_generation == SAVE_GENERATION_2)
    {
        party_count = trainer->pokemon_party.gen2_pokemon_party.count;
    }
    static char selected_pokemon_nickname[11];

    DrawText(trainer_name, x, y, 20, BLACK);
    DrawText(trainer_id, x, y + 30, 20, BLACK);

    // Draw the pokemon buttons
    for (int i = 0; i < party_count; i++)
    {
        char pokemon_nickname[11];
        if (trainer_generation == SAVE_GENERATION_1)
        {
            pksav_gen1_import_text(trainer->pokemon_party.gen1_pokemon_party.nicknames[i], pokemon_nickname, 10);
        }
        else if (trainer_generation == SAVE_GENERATION_2)
        {
            pksav_gen2_import_text(trainer->pokemon_party.gen2_pokemon_party.nicknames[i], pokemon_nickname, 10);
        }

        PokemonButton((Rectangle){x - 10, y + 70 + (i * 30), 200, 30}, i, pokemon_nickname);
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){x - 10, y + 70 + (i * 30), 200, 30}) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            trainerSelection[current_trainer_index].pokemon_index = i;
        }
    }

    // Draw the selected pokemon nickname
    if (current_trainer_index != -1 && trainerSelection[current_trainer_index].pokemon_index != -1)
    {
        if (trainer_generation == SAVE_GENERATION_1)
        {
            pksav_gen1_import_text(trainer->pokemon_party.gen1_pokemon_party.nicknames[trainerSelection[current_trainer_index].pokemon_index], selected_pokemon_nickname, 10);
        }
        else if (trainer_generation == SAVE_GENERATION_2)
        {
            pksav_gen2_import_text(trainer->pokemon_party.gen2_pokemon_party.nicknames[trainerSelection[current_trainer_index].pokemon_index], selected_pokemon_nickname, 10);
        }
        DrawText(selected_pokemon_nickname, trainerSelection[current_trainer_index].trainer_index ? (GetScreenWidth() / 2) + 50 : x, y + 300, 20, BLACK);
    }
}

void DrawAboutScreen(void)
{
    int x = 50;

    BeginDrawing();
    ClearBackground(background_color);

    DrawText("About Pokerom Trader", x, 100, 20, BLACK);
    DrawText("Pokerom Trader is a tool for trading Pokemon between two save files", x, 200, 20, BLACK);
    DrawText("written by github.com/savaughn 2023", x, 225, 20, BLACK);
    DrawText("Pokerom Trader is open source and licensed under the MIT license", x, 250, 20, BLACK);
    DrawText("Pokerom Trader uses the following libraries:", x, 300, 20, BLACK);
    DrawText("raylib - https://www.raylib.com/", x, 325, 20, BLACK);
    DrawText("pksav - https://github.com/ncorgan/pksav", x, 350, 20, BLACK);
    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, BLACK);

    EndDrawing();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
        {
            current_screen = SCREEN_SETTINGS;
        }
    }
}
void DrawFileEditScreen(struct SaveFileData *save_file_data)
{
    static bool editingText = false;
    static char inputText[MAX_INPUT_CHARS] = "\0";
    static int textSize = 0;
    static bool hasShownPlaceholder = false;
    static bool hasPressedClear = false;
    static int err = 0;
    char *input_text_backup[MAX_INPUT_CHARS];

    textSize = strlen(inputText);
    strcpy(input_text_backup, (char *)save_file_data->saveDir);

    // Placeholder Text
    if (!editingText && textSize == 0 && !hasShownPlaceholder)
    {
        strcpy(inputText, (char *)save_file_data->saveDir);
        textSize = strlen(inputText);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Check if the mouse is clicked within the input box
        if (CheckCollisionPointRec(GetMousePosition(), input_box_rec))
        {
            editingText = true;
        }
        else
        {
            editingText = false;
        }
    }

    if (editingText)
    {
        hasShownPlaceholder = true;
        int key = GetCharPressed();
        int backspace = GetKeyPressed();
        if (key >= 32 && key <= 125 && textSize < MAX_INPUT_CHARS)
        {
            // Append character to inputText
            inputText[textSize] = (char)key;
            textSize++;
        }
        else if ((key == KEY_BACKSPACE || backspace == KEY_BACKSPACE) && textSize > 0)
        {
            // Remove last character
            textSize--;
            inputText[textSize] = '\0';
        }

        // Finish editing by pressing Enter
        if (IsKeyPressed(KEY_ENTER))
        {
            editingText = false;
        }
    }

    BeginDrawing();
    ClearBackground(background_color);
    DrawText("Specify folder name containing saves", 50, SCREEN_HEIGHT / 2 - 75, 20, BLACK);

    // Draw the input box
    DrawRectangleRec(input_box_rec, WHITE);
    DrawRectangleLinesEx(input_box_rec, 2, editingText ? BLACK : DARKGRAY);

    // Draw the text inside the input box
    DrawText(inputText, input_box_rec.x + 10, input_box_rec.y + 10, 20, BLACK);

    Rectangle clear_button_rec = (Rectangle) {SCREEN_WIDTH- MeasureText("Clear input", 20) + 10 - 70, input_box_rec.y + 25 + input_box_rec.height - 5, MeasureText("Clear input", 20) + 10, 30};
    DrawRectangleRec(clear_button_rec, RED);
    DrawText("Clear input", clear_button_rec.x +5, clear_button_rec.y + 5, 20, WHITE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), clear_button_rec) && textSize > 0)
        {
            // Clear the input text
            memset(inputText, 0, sizeof(inputText));

            textSize = 0;
            editingText = true;
            hasShownPlaceholder = true;
            err = 0;
            hasPressedClear = true;
        }
    }

    // Draw the blinking cursor
    if (editingText)
    {
        DrawLine(input_box_rec.x + 8 + MeasureText(inputText, 20), input_box_rec.y + 15,
                 input_box_rec.x + 8 + MeasureText(inputText, 20), input_box_rec.y + 25, BLACK);
    }

    // Draw the save button
    DrawText("Save!", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, textSize ? BLACK : LIGHTGRAY);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}) && textSize > 0)
        {
            err = write_key_to_config("SAVE_FILE_DIR", inputText);
            if (err == 0) 
            {
                strcpy((char *)save_file_data->saveDir, inputText);
                save_file_data->numSaves = 0;
                *save_file_data->saves_file_path = NULL;
                current_screen = SCREEN_SETTINGS;
            }
        }
    }

    if (err == 1) DrawText(TextFormat("error writing config %d", errno), 50, 50, 20, BLACK);

    // add a back button
    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (hasPressedClear)
            {
                strcpy(inputText, input_text_backup);
            }
            hasPressedClear = false;
            current_screen = SCREEN_SETTINGS;
        }
    }
    EndDrawing();
}
void DrawSettingsScreen(void)
{
    BeginDrawing();
    ClearBackground(background_color);
    DrawText("Settings", 190, 100, 20, BLACK);
    DrawText("Change Save Directory", 190, 200, 20, BLACK);
    DrawText("About Pokerom Trader", 190, 225, 20, BLACK);
    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, BLACK);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 200, 200, 20}))
        {
            current_screen = SCREEN_FILE_EDIT;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 225, 200, 20}))
        {
            current_screen = SCREEN_ABOUT;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
        {
            current_screen = SCREEN_MAIN_MENU;
        }
    }
    EndDrawing();
}
void DrawMainMenuScreen(struct SaveFileData *save_file_data)
{
    BeginDrawing();
    ClearBackground(background_color);
    DrawText("Main Menu", 190, 100, 20, BLACK);
    DrawText("Trade", 190, 200, 20, BLACK);
    DrawText("Settings", 190, 225, 20, BLACK);
    DrawText("Quit", 190, 250, 20, BLACK);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 200, 200, 20}))
        {
            int err = get_save_files(save_file_data);
            no_dir_err = err;
            current_screen = SCREEN_FILE_SELECT;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 225, 200, 20}))
        {
            current_screen = SCREEN_SETTINGS;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 250, 200, 20}))
        {
            should_close_window = true;
        }
    }
    EndDrawing();
}
void DrawFileSelectScreen(struct SaveFileData *save_file_data, char *player1_save_path, char *player2_save_path, struct TrainerInfo *trainer1, struct TrainerInfo *trainer2, struct TrainerSelection trainerSelection[2], PokemonSave *pokemon_save_player1, PokemonSave *pokemon_save_player2)
{
    static int selected_saves_index[2] = {-1, -1};
    
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (save_file_data->numSaves == 0)
    {
        if (no_dir_err) {
            Vector2 text_center = SCREEN_CENTER("Save folder doesn't exist!", 20);
            DrawText("Save folder doesn't exist!", text_center.x, text_center.y, 20, BLACK);
        } else {
            DrawText("No save files found in save folder", 190, 200, 20, BLACK);
        }
        char *save_dir = save_file_data->saveDir;
        DrawText(TextFormat("%s", save_dir), SCREEN_CENTER(save_dir, 20).x, 275, 20, BLACK);
    }
    else
    {
        bool hasSelectedTwoSaves = selected_saves_index[0] != -1 && selected_saves_index[1] != -1;
        static bool isSameGeneration = true;

        DrawText("Select two save files of the same generation to trade between", SCREEN_WIDTH / 2 - MeasureText("Select two save files of the same generation to trade between", 20) / 2, 100, 20, BLACK);
        for (int i = 0; i < save_file_data->numSaves; i++)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){190, 200 + 25 * i, SCREEN_WIDTH / 2, 25}))
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
                }
            }
            char *save_name = strrchr(save_file_data->saves_file_path[i], '/');
            save_name++;
            DrawText(save_name, 190, 200 + 25 * i, 20, (selected_saves_index[0] == i || selected_saves_index[1] == i) ? LIGHTGRAY : BLACK);

            // Reset generation check
            if (!isSameGeneration)
            {
                isSameGeneration = selected_saves_index[1] == -1;
            }

            DrawText("Trade >", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, hasSelectedTwoSaves && isSameGeneration ? BLACK : LIGHTGRAY);
            if (!isSameGeneration)
                DrawText("Cross-gen trades are not yet supported", NEXT_BUTTON_X - 125, NEXT_BUTTON_Y + 25, 15, RED);

            if (hasSelectedTwoSaves && isSameGeneration)
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
                    {
                        // load selection to player1_save
                        *pokemon_save_player1 = loadSaveFromFile(save_file_data->saves_file_path[selected_saves_index[0]]);

                        // save the selected path name
                        strcpy(player1_save_path, save_file_data->saves_file_path[selected_saves_index[0]]);
                        // generate trainer info from save
                        create_trainer(pokemon_save_player1, trainer1);
                        // save trainer id to trainerSelection
                        trainerSelection[0].trainer_id = trainer1->trainer_id;

                        // load selection to player2_save
                        *pokemon_save_player2 = loadSaveFromFile(save_file_data->saves_file_path[selected_saves_index[1]]);
                        strcpy(player2_save_path, save_file_data->saves_file_path[selected_saves_index[1]]);
                        create_trainer(pokemon_save_player2, trainer2);
                        trainerSelection[1].trainer_id = trainer2->trainer_id;

                        if (pokemon_save_player1->save_generation_type != pokemon_save_player2->save_generation_type)
                        {
                            isSameGeneration = false;
                        }
                        else
                        {
                            current_screen = SCREEN_TRADE;
                        }
                    }
                }
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
            pokemon_save_player1 = NULL;
            pokemon_save_player2 = NULL;
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
            pokemon_save_player1 = NULL;
            pokemon_save_player2 = NULL;
        }
    }

    EndDrawing();
}
void DrawTradeScreen(PokemonSave *save_player1, PokemonSave *save_player2, char *player1_save_path, char *player2_save_path, struct TrainerSelection trainerSelection[2], struct TrainerInfo *trainer1, struct TrainerInfo *trainer2)
{
    // Update
    int selected_index_trainer1 = trainerSelection[0].pokemon_index;
    int selected_index_trainer2 = trainerSelection[1].pokemon_index;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTrainerInfo(trainer1, 50, 50, trainerSelection, save_player1->save.gen2_save.save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL);
    DrawTrainerInfo(trainer2, GetScreenWidth() / 2 + 50, 50, trainerSelection, save_player2->save.gen2_save.save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL);
    uint8_t canSubmitTrade = trainerSelection[0].pokemon_index != -1 && trainerSelection[1].pokemon_index != -1;
    DrawText("Trade!", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, canSubmitTrade ? BLACK : LIGHTGRAY);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}) && canSubmitTrade)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // Reset Trading state
            trainerSelection[0].pokemon_index = -1;
            trainerSelection[1].pokemon_index = -1;

            swapPokemonAtIndexBetweenSaves(save_player1, save_player2, selected_index_trainer1, selected_index_trainer2);
            updateSeenOwnedPokemon(save_player1, selected_index_trainer1);
            updateSeenOwnedPokemon(save_player2, selected_index_trainer2);
            saveToFile(save_player1, player1_save_path);
            saveToFile(save_player2, player2_save_path);

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
            trainerSelection[0].pokemon_index = -1;
            trainerSelection[1].pokemon_index = -1;
            current_screen = SCREEN_FILE_SELECT;
        }
    }

    EndDrawing();
}

void DrawRaylibScreenLoop(
    struct SaveFileData *save_file_data,
    struct TrainerInfo *trainer1,
    struct TrainerInfo *trainer2,
    struct TrainerSelection trainerSelection[2],
    char *player1_save_path,
    char *player2_save_path,
    PokemonSave *pokemon_save_player1,
    PokemonSave *pokemon_save_player2)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pokerom Trader");

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
            DrawFileSelectScreen(save_file_data, player1_save_path, player2_save_path, trainer1, trainer2, trainerSelection, pokemon_save_player1, pokemon_save_player2);
            break;
        case SCREEN_TRADE:
            DrawTradeScreen(pokemon_save_player1, pokemon_save_player2, player1_save_path, player2_save_path, trainerSelection, trainer1, trainer2);
            break;
        case SCREEN_MAIN_MENU:
            DrawMainMenuScreen(save_file_data);
            break;
        case SCREEN_SETTINGS:
            DrawSettingsScreen();
            break;
        case SCREEN_FILE_EDIT:
            DrawFileEditScreen(save_file_data);
            break;
        case SCREEN_ABOUT:
        {
            DrawAboutScreen();
            break;
        }
        default:
            DrawText("Something went wrong", 190, 100, 20, BLACK);
            DrawText("Press ESC to exit", 190, 125, 20, BLACK);
            break;
        }
    }
}
