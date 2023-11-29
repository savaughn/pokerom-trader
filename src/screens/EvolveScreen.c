#include "raylibhelper.h"
#include "pksavfilehelper.h"

void _debug__generate_evolve_pkmn_gen1(PokemonSave *pkmn_save, char *save_path, struct trainer_info *trainer)
{
    pkmn_save->save.gen1_save.pokemon_storage.p_party->species[0] = SI_KADABRA;
    pkmn_save->save.gen1_save.pokemon_storage.p_party->species[1] = SI_MACHOKE;
    pkmn_save->save.gen1_save.pokemon_storage.p_party->species[2] = SI_GRAVELER;
    pkmn_save->save.gen1_save.pokemon_storage.p_party->species[3] = SI_HAUNTER;

    pksav_gen1_export_text("KADABRA", pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[0], PKMN_NAME_TEXT_MAX);
    pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[0][strlen("KADABRA")] = 0x50;
    pksav_gen1_export_text("MACHOKE", pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[1], PKMN_NAME_TEXT_MAX);
    pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[1][strlen("MACHOKE")] = 0x50;
    pksav_gen1_export_text("GRAVELER", pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[2], PKMN_NAME_TEXT_MAX);
    pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[2][strlen("GRAVELER")] = 0x50;
    pksav_gen1_export_text("HAUNTER", pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[3], PKMN_NAME_TEXT_MAX);
    pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[3][strlen("HAUNTER")] = 0x50;

    for (int i = 0; i < 4; i++)
    {
        update_pkmn_DVs(pkmn_save, i);
        update_pkmn_stats(pkmn_save, i);
        update_seen_owned_pkmn(pkmn_save, i);
    }
    save_savefile_to_path(pkmn_save, save_path);
    create_trainer(pkmn_save, trainer);
}
void _debug__generate_evolve_pkmn_gen2(PokemonSave *pkmn_save, char *save_path, struct trainer_info *trainer)
{
    pkmn_save->save.gen2_save.pokemon_storage.p_party->species[0] = SCYTHER;
    pkmn_save->save.gen2_save.pokemon_storage.p_party->species[1] = POLIWHIRL;
    pkmn_save->save.gen2_save.pokemon_storage.p_party->species[2] = SLOWPOKE;
    pkmn_save->save.gen2_save.pokemon_storage.p_party->species[3] = ONIX;
    pkmn_save->save.gen2_save.pokemon_storage.p_party->species[4] = PORYGON;
    pkmn_save->save.gen2_save.pokemon_storage.p_party->species[5] = SEADRA;

    pksav_gen2_export_text("SCYTHER", pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[0], PKMN_NAME_TEXT_MAX);
    pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[0][strlen("SCYTHER")] = 0x50;
    pksav_gen2_export_text("POLIWHIRL", pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[1], PKMN_NAME_TEXT_MAX);
    pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[1][strlen("POLIWHIRL")] = 0x50;
    pksav_gen2_export_text("SLOWPOKE", pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[2], PKMN_NAME_TEXT_MAX);
    pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[2][strlen("SLOWPOKE")] = 0x50;
    pksav_gen2_export_text("ONIX", pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[3], PKMN_NAME_TEXT_MAX);
    pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[3][strlen("ONIX")] = 0x50;
    pksav_gen2_export_text("PORYGON", pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[4], PKMN_NAME_TEXT_MAX);
    pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[4][strlen("PORYGON")] = 0x50;
    pksav_gen2_export_text("SEADRA", pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[5], PKMN_NAME_TEXT_MAX);
    pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[5][strlen("SEADRA")] = 0x50;

    for (int i = 0; i < 6; i++)
    {
        update_pkmn_DVs(pkmn_save, i);
        update_pkmn_stats(pkmn_save, i);
        update_seen_owned_pkmn(pkmn_save, i);
    }
    save_savefile_to_path(pkmn_save, save_path);
    create_trainer(pkmn_save, trainer);
}

void draw_evolve(PokemonSave *pkmn_save, char *save_path, struct trainer_info *trainer, GameScreen *current_screen, Texture2D *evolve_texture)
{
    SaveGenerationType save_generation_type = pkmn_save->save_generation_type;
    static enum ui_selections {
        E_UI_NONE = -1,
        E_UI_BACK,
        E_UI_EVOLVE
    } ui_selection = E_UI_NONE;

    // Call rng
    generate_rand_num_step(save_generation_type);

    BeginDrawing();
    ClearBackground(RED);
    draw_background_grid();
    DrawTextureEx(*evolve_texture, (Vector2){50, 10}, 0, 0.8, WHITE);

    const int NONE = -1;
    static int selected_index = NONE;
    static enum eligible_evolution_status evolve_eligible = E_EVO_STATUS_ELIGIBLE;
    char pokemon_nickname[PKMN_NAME_TEXT_MAX + 1] = "\0";
    const int TRAINER_NAME_X = 50;
    const int TRAINER_NAME_Y = 115;
    static bool is_trade_eligible = false;
    static bool show_evolve_toast = false;
    static bool show_saving_icon = false;

    int party_count = 0;
    if (save_generation_type == SAVE_GENERATION_1)
        party_count = pkmn_save->save.gen1_save.pokemon_storage.p_party->count;
    else if (save_generation_type == SAVE_GENERATION_2)
        party_count = pkmn_save->save.gen2_save.pokemon_storage.p_party->count;


    // Details Panel Rectangle
    Rectangle container_rec;
    container_rec.x = TRAINER_NAME_X - 3;
    container_rec.y = TRAINER_NAME_Y - 10;
    container_rec.width = 200;
    container_rec.height = 300;

    // Draw rectangle with transparent background blue
    DrawRectangleRec(container_rec, (Color){0, 100, 255, 158});

    // draw border
    DrawRectangleLinesEx((Rectangle){container_rec.x - 4, container_rec.y - 4, container_rec.width + 8, container_rec.height + 8}, 1, BLACK);
    DrawRectangleLinesEx((Rectangle){container_rec.x - 3, container_rec.y - 3, container_rec.width + 6, container_rec.height + 6}, 3, WHITE);
    DrawRectangleLinesEx((Rectangle){container_rec.x, container_rec.y, container_rec.width, container_rec.height}, 1, BLACK);

    // Create the trainer name and id strings for Raylib drawing
    char trainer_name[15] = "\0";
    create_trainer_name_str(trainer, trainer_name);
    char trainer_id[11] = "\0";
    create_trainer_id_str(trainer, trainer_id);
    shadow_text(trainer_name, TRAINER_NAME_X + 10, TRAINER_NAME_Y, 20, WHITE);
    shadow_text(trainer_id, TRAINER_NAME_X + 10, TRAINER_NAME_Y + 25, 20, WHITE);

    // Search party for pkmn eligible for trade evolution
    for (int i = 0; i < party_count; i++)
    {
        if (save_generation_type == SAVE_GENERATION_1)
        {
            evolve_eligible = check_trade_evolution_gen1(pkmn_save, i);
            pksav_gen1_import_text(pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[i], pokemon_nickname, PKMN_NAME_TEXT_MAX);
            draw_pkmn_button((Rectangle){TRAINER_NAME_X, TRAINER_NAME_Y + 75 + (i * 30), 200, 30}, i, pokemon_nickname, selected_index == i || evolve_eligible == E_EVO_STATUS_NOT_ELIGIBLE);
        }
        else if (save_generation_type == SAVE_GENERATION_2)
        {
            evolve_eligible = check_trade_evolution_gen2(pkmn_save, i);
            pksav_gen2_import_text(pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[i], pokemon_nickname, PKMN_NAME_TEXT_MAX);
            draw_pkmn_button((Rectangle){TRAINER_NAME_X, TRAINER_NAME_Y + 75 + (i * 30), MeasureText(pokemon_nickname, 20) + 10, 30}, i, pokemon_nickname, selected_index == i || evolve_eligible == E_EVO_STATUS_NOT_ELIGIBLE);
        }
        // Selected pokemon button
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){TRAINER_NAME_X, TRAINER_NAME_Y + 75 + (i * 30), 200, 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
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
                if (save_generation_type == SAVE_GENERATION_1)
                    is_trade_eligible = check_trade_evolution_gen1(pkmn_save, selected_index);
                else if (save_generation_type == SAVE_GENERATION_2)
                    is_trade_eligible = check_trade_evolution_gen2(pkmn_save, selected_index);
            }
        }
    }

    const int x_offset = SCREEN_WIDTH - 400;

    int text_pos_x = container_rec.x + x_offset + 70;
    int stat_text_pos_x = container_rec.x + x_offset + 150;
    int dv_text_pos_x = container_rec.x + x_offset + 210;

    // Draw the selected pokemon nickname
    if (selected_index != NONE)
    {
        DrawRectangle(container_rec.x + x_offset, container_rec.y - 10, container_rec.width * 1.5, container_rec.height, (Color){0, 0, 0, 75});
        
        // Name of the pokemon selected from list
        static char selected_pokemon_nickname[PKMN_NAME_TEXT_MAX + 1] = "\0";
        if (pkmn_save->save_generation_type == SAVE_GENERATION_1)
        {
            pksav_gen1_import_text(pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[selected_index], selected_pokemon_nickname, PKMN_NAME_TEXT_MAX);
            struct pksav_gen1_party_pokemon party_pkmn = pkmn_save->save.gen1_save.pokemon_storage.p_party->party[selected_index];
            // Draw level
            shadow_text(TextFormat("Level %u", party_pkmn.party_data.level), text_pos_x, container_rec.y + 40, 20, WHITE);
            shadow_text("Stats", stat_text_pos_x - 15, container_rec.y + 70, 20, WHITE);
            shadow_text("DVs", dv_text_pos_x, container_rec.y + 70, 20, WHITE);
            // Draw stats
            shadow_text("HP:", text_pos_x, container_rec.y + 100, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(party_pkmn.party_data.max_hp)), stat_text_pos_x, container_rec.y + 100, 20, WHITE);
            shadow_text("Atk:", text_pos_x, container_rec.y + 130, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(party_pkmn.party_data.atk)), stat_text_pos_x, container_rec.y + 130, 20, WHITE);
            shadow_text("Def:", text_pos_x, container_rec.y + 160, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(party_pkmn.party_data.def)), stat_text_pos_x, container_rec.y + 160, 20, WHITE);
            shadow_text("Spd:", text_pos_x, container_rec.y + 190, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(party_pkmn.party_data.spd)), stat_text_pos_x, container_rec.y + 190, 20, WHITE);
            shadow_text("Spcl:", text_pos_x, container_rec.y + 220, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(party_pkmn.party_data.spcl)), stat_text_pos_x, container_rec.y + 220, 20, WHITE);

            // Draw DVs inline with stats
            uint8_t pkmn_dv[PKSAV_NUM_GB_IVS];
            pksav_get_gb_IVs(&party_pkmn.pc_data.iv_data, pkmn_dv, sizeof(pkmn_dv));
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_HP]), dv_text_pos_x, container_rec.y + 100, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_ATTACK]), dv_text_pos_x, container_rec.y + 130, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_DEFENSE]), dv_text_pos_x, container_rec.y + 160, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_SPEED]), dv_text_pos_x, container_rec.y + 190, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_SPECIAL]), dv_text_pos_x, container_rec.y + 220, 20, WHITE);
        }
        else if (pkmn_save->save_generation_type == SAVE_GENERATION_2)
        {
            pksav_gen1_import_text(pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[selected_index], selected_pokemon_nickname, PKMN_NAME_TEXT_MAX);
            struct pksav_gen2_party_pokemon party_pkmn = pkmn_save->save.gen2_save.pokemon_storage.p_party->party[selected_index];
            // Draw level
            shadow_text(TextFormat("Level %u", party_pkmn.pc_data.level), text_pos_x, container_rec.y + 40, 20, WHITE);
            shadow_text("Stats", stat_text_pos_x - 15, container_rec.y + 70, 20, WHITE);
            shadow_text("DVs", dv_text_pos_x, container_rec.y + 70, 20, WHITE);
            // Draw stats
            shadow_text("HP:", text_pos_x, container_rec.y + 100, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(party_pkmn.party_data.max_hp)), stat_text_pos_x, container_rec.y + 100, 20, WHITE);
            shadow_text("Atk:", text_pos_x, container_rec.y + 130, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(party_pkmn.party_data.atk)), stat_text_pos_x, container_rec.y + 130, 20, WHITE);
            shadow_text("Def:", text_pos_x, container_rec.y + 160, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(party_pkmn.party_data.def)), stat_text_pos_x, container_rec.y + 160, 20, WHITE);
            shadow_text("Spd:", text_pos_x, container_rec.y + 190, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(party_pkmn.party_data.spd)), stat_text_pos_x, container_rec.y + 190, 20, WHITE);
            shadow_text("Sp.A:", text_pos_x, container_rec.y + 220, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(party_pkmn.party_data.spatk)), stat_text_pos_x, container_rec.y + 220, 20, WHITE);
            shadow_text("Sp.D:", text_pos_x, container_rec.y + 250, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(party_pkmn.party_data.spdef)), stat_text_pos_x, container_rec.y + 250, 20, WHITE);

            // Draw DVs
            uint8_t pkmn_dv[PKSAV_NUM_GB_IVS];
            pksav_get_gb_IVs(&party_pkmn.pc_data.iv_data, pkmn_dv, sizeof(pkmn_dv));
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_HP]), dv_text_pos_x, container_rec.y + 100, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_ATTACK]), dv_text_pos_x, container_rec.y + 130, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_DEFENSE]), dv_text_pos_x, container_rec.y + 160, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_SPEED]), dv_text_pos_x, container_rec.y + 190, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_SPECIAL]), dv_text_pos_x, container_rec.y + 220, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_SPECIAL]), dv_text_pos_x, container_rec.y + 250, 20, WHITE);
        }
        // Draw nickname
        shadow_text(selected_pokemon_nickname, text_pos_x, container_rec.y + 10, 20, WHITE);
    }

    // Bottom bar
    const Rectangle bottom_bar_rec = (Rectangle){0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, 100};
    DrawRectangleRec(bottom_bar_rec, WHITE);
    DrawLineEx((Vector2){bottom_bar_rec.x, bottom_bar_rec.y}, (Vector2){bottom_bar_rec.width, bottom_bar_rec.y}, 15, BLACK);

    // Evolve button (next button)
    const Rectangle evolve_button_rec = (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y + 8, BUTTON_WIDTH, BUTTON_HEIGHT};
    DrawText("Evolve !", evolve_button_rec.x + 15, evolve_button_rec.y + 10, 20, selected_index != NONE ? is_trade_eligible ? BLACK : LIGHTGRAY : LIGHTGRAY);

    const Rectangle back_button_rec = (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y + 8, BUTTON_WIDTH, BUTTON_HEIGHT};
    DrawText("< Back", back_button_rec.x + 15, back_button_rec.y + 10, 20, ui_selection == E_UI_BACK ? LIGHTGRAY : BLACK);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        if (CheckCollisionPointRec(GetMousePosition(), evolve_button_rec) && is_trade_eligible)
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
                // Evolve pokemon with the simulated new trainer
                evolve_party_pokemon_at_index(pkmn_save, selected_index);
                // Update stats
                update_pkmn_stats(pkmn_save, selected_index);
                // Update pokedex
                update_seen_owned_pkmn(pkmn_save, selected_index);
                // Finalize pkmn data changes
                show_saving_icon = true;
                save_savefile_to_path(pkmn_save, save_path);
                // Refresh trainer data
                create_trainer(pkmn_save, trainer);

                selected_index = NONE;
                is_trade_eligible = false;
                show_evolve_toast = true;
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

    if (show_saving_icon)
    {
        show_saving_icon = draw_save_icon(SCREEN_WIDTH - 50, 10, show_saving_icon);
    }
    if (show_evolve_toast)
    {
        show_evolve_toast = !draw_toast_message("Evolved Successfully!", TOAST_SHORT, TOAST_INFO);
    }

    EndDrawing();

    if (IsKeyPressed(KEY_ESCAPE))
    {
        *current_screen = SCREEN_EVOLVE_FILE_SELECT;
        selected_index = NONE;
        ui_selection = E_UI_NONE;
    }
}
