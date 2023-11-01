#include "raylibhelper.h"
#include "pksavfilehelper.h"

void draw_evolve(PokemonSave *pkmn_save, char *save_path, struct TrainerInfo *trainer, GameScreen *current_screen)
{
    SaveGenerationType save_generation_type = pkmn_save->save_generation_type;
    static enum ui_selections
    {
        E_UI_NONE = -1,
        E_UI_BACK,
        E_UI_EVOLVE
    } ui_selection = E_UI_NONE;

    // Call rng
    generate_rand_num_step(save_generation_type);

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Trade Evolve", 50, 50, 20, BLACK);

    int party_count = 0;
    const int NONE = -1;
    static int selected_index = NONE;
    static int result = 0;
    char pokemon_nickname[11] = "\0";
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
    const Rectangle evolve_button_rec = (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT};
    DrawText("Evolve!", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, selected_index != NONE ? BLACK : LIGHTGRAY);

    const Rectangle back_button_rec = (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT};
    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, ui_selection == E_UI_BACK ? LIGHTGRAY : BLACK);
    
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        if (CheckCollisionPointRec(GetMousePosition(), evolve_button_rec))
        {
            ui_selection = E_UI_EVOLVE;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), back_button_rec))
        {
            ui_selection = E_UI_BACK;
        }
        else
        {
            ui_selection = E_UI_NONE;
        }
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && ui_selection != E_UI_NONE)
    {
        switch (ui_selection)
        {
        case E_UI_EVOLVE:
            if (CheckCollisionPointRec(GetMousePosition(), evolve_button_rec))
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
            else
            {
                ui_selection = E_UI_NONE;
            }
            break;
        case E_UI_BACK:
            if (CheckCollisionPointRec(GetMousePosition(), back_button_rec))
            {
                *current_screen = SCREEN_EVOLVE_FILE_SELECT;
                selected_index = NONE;
            }
            else
            {
                ui_selection = E_UI_NONE;
            }
            break;
        default:
            break;
        }
    }

    EndDrawing();
}
