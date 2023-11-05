#include "raylibhelper.h"
#include "pksavhelper.h"
#include <stdio.h>

void animate_details_panel(int16_t *grow_x, float *scale_width, uint8_t current_trainer_index, bool tr1_active, bool tr2_active)
{
    const uint8_t min_grow_x = 0;
    const uint8_t max_grow_x = 207;
    const uint8_t grow_rate = 50;
    const float max_scale = 2.1825f;
    const float min_scale = 1.0f;
    const float scale_rate = 0.286f;
    
    if (tr2_active)
    {
        grow_x[1] += grow_rate;
    }
    else if (current_trainer_index == 1 && !tr2_active)
    {
        grow_x[1] -= grow_rate;
    }
    grow_x[1] = clamp_max(grow_x[1], max_grow_x);
    grow_x[1] = clamp_min(grow_x[1], min_grow_x);
    grow_x[0] = min_grow_x;

    if (tr1_active || tr2_active)
    {
        scale_width[current_trainer_index] += scale_rate;
    }
    else
    {
        scale_width[current_trainer_index] -= scale_rate;
    }

    scale_width[current_trainer_index] = clamp_max(scale_width[current_trainer_index], max_scale);
    scale_width[current_trainer_index] = clamp_min(scale_width[current_trainer_index], min_scale);
}

// Draws the trainers name, id, and party pokemon in pokemon buttons
void draw_trainer_info(struct TrainerInfo *trainer, int x, int y, struct TrainerSelection trainerSelection[2], bool showGender, bool is_same_generation, bool *is_valid_trade)
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
    bool tr1_active = trainerSelection[0].pkmn_party_index != -1 && current_trainer_index == 0;
    bool tr2_active = trainerSelection[1].pkmn_party_index != -1 && current_trainer_index == 1;

    static int16_t grow_x[2] = {0};
    static float scale_width[2] = {1.0f};

    animate_details_panel(grow_x, scale_width, current_trainer_index, tr1_active, tr2_active);

    // Details Panel Rectangle
    Rectangle container_rec;
    container_rec.x = x - 10 - grow_x[current_trainer_index];
    container_rec.y = y - 10;
    container_rec.width = 175 * scale_width[current_trainer_index];
    container_rec.height = 300;

    // Draw rectangle with transparent background blue
    DrawRectangleRec(container_rec, (Color){0, 100, 255, 158});

    // draw border
    DrawRectangleLinesEx((Rectangle){container_rec.x - 4, container_rec.y - 4, container_rec.width + 8, container_rec.height + 8}, 1, BLACK);
    DrawRectangleLinesEx((Rectangle){container_rec.x - 3, container_rec.y - 3, container_rec.width + 6, container_rec.height + 6}, 3, WHITE);
    DrawRectangleLinesEx((Rectangle){container_rec.x, container_rec.y, container_rec.width, container_rec.height}, 1, BLACK);

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

    shadow_text(trainer_name, x, y, 20, WHITE);
    shadow_text(trainer_id, x, y + 30, 20, WHITE);

    // Draw the pokemon buttons
    for (int party_index = 0; party_index < party_count; party_index++)
    {
        enum eligible_trade_status trade_status = E_TRADE_STATUS_ELIGIBLE;
        trade_status = check_trade_eligibility(trainer, party_index);
        is_valid_trade[current_trainer_index] = trade_status == E_TRADE_STATUS_ELIGIBLE;

        char pokemon_nickname[11] = "\0";
        if (trainer_generation == SAVE_GENERATION_1)
        {
            pksav_gen1_import_text(trainer->pokemon_party.gen1_pokemon_party.nicknames[party_index], pokemon_nickname, 10);
        }
        else if (trainer_generation == SAVE_GENERATION_2)
        {
            pksav_gen2_import_text(trainer->pokemon_party.gen2_pokemon_party.nicknames[party_index], pokemon_nickname, 10);
        }

        draw_pkmn_button((Rectangle){x - 10, y + 70 + (party_index * 30), 200, 30}, party_index, pokemon_nickname, current_trainer_index != -1 && (trainerSelection[current_trainer_index].pkmn_party_index == party_index));
        if (trade_status == E_TRADE_STATUS_GEN2_PKMN && (tr1_active || tr2_active))
            shadow_text("Gen 2 only ", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 275, 20, WHITE);
        else if (trade_status == E_TRADE_STATUS_GEN2_MOVE)
            shadow_text("Gen 2 move ", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 275, 20, WHITE);
        else if (trade_status == E_TRADE_STATUS_HM_MOVE)
            shadow_text("HM move", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 275, 20, WHITE);

        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){x - 10, y + 70 + (party_index * 30), 200, 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (trainerSelection[current_trainer_index].pkmn_party_index != party_index)
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
    if (current_trainer_index != -1 && trainerSelection[current_trainer_index].pkmn_party_index != -1 && ((current_trainer_index == 0 && scale_width[0] > 2.0f) || (current_trainer_index == 1 && grow_x[1] > 200)))
    {
        // Name of the pokemon selected from list
        static char selected_pokemon_nickname[11];
        if (trainer_generation == SAVE_GENERATION_1)
        {
            pksav_gen1_import_text(trainer->pokemon_party.gen1_pokemon_party.nicknames[trainerSelection[current_trainer_index].pkmn_party_index], selected_pokemon_nickname, 10);

            // Draw level
            shadow_text(TextFormat("Level %u", trainer->pokemon_party.gen1_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].party_data.level), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 40, 20, WHITE);
            shadow_text("Stats", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 60 : container_rec.x + container_rec.width / 2 + 60, container_rec.y + 70, 20, WHITE);
            shadow_text("DVs", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 140 : container_rec.x + container_rec.width / 2 + 140, container_rec.y + 70, 20, WHITE);
            // Draw stats
            shadow_text("HP:", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 100, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(trainer->pokemon_party.gen1_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].party_data.max_hp)), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 80 : container_rec.x + container_rec.width / 2 + 80, container_rec.y + 100, 20, WHITE);
            shadow_text("Atk:", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 130, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(trainer->pokemon_party.gen1_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].party_data.atk)), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 80 : container_rec.x + container_rec.width / 2 + 80, container_rec.y + 130, 20, WHITE);
            shadow_text("Def:", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 160, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(trainer->pokemon_party.gen1_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].party_data.def)), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 80 : container_rec.x + container_rec.width / 2 + 80, container_rec.y + 160, 20, WHITE);
            shadow_text("Spd:", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 190, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(trainer->pokemon_party.gen1_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].party_data.spd)), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 80 : container_rec.x + container_rec.width / 2 + 80, container_rec.y + 190, 20, WHITE);
            shadow_text("Spcl:", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 220, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(trainer->pokemon_party.gen1_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].party_data.spcl)), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 80 : container_rec.x + container_rec.width / 2 + 80, container_rec.y + 220, 20, WHITE);

            // Draw DVs inline with stats
            uint8_t pkmn_dv[PKSAV_NUM_GB_IVS];
            pksav_get_gb_IVs(&trainer->pokemon_party.gen1_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].pc_data.iv_data, pkmn_dv, sizeof(pkmn_dv));
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_HP]), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 140 : container_rec.x + container_rec.width / 2 + 140, container_rec.y + 100, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_ATTACK]), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 140 : container_rec.x + container_rec.width / 2 + 140, container_rec.y + 130, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_DEFENSE]), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 140 : container_rec.x + container_rec.width / 2 + 140, container_rec.y + 160, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_SPEED]), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 140 : container_rec.x + container_rec.width / 2 + 140, container_rec.y + 190, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_SPECIAL]), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 140 : container_rec.x + container_rec.width / 2 + 140, container_rec.y + 220, 20, WHITE);
        }
        else if (trainer_generation == SAVE_GENERATION_2)
        {
            pksav_gen2_import_text(trainer->pokemon_party.gen2_pokemon_party.nicknames[trainerSelection[current_trainer_index].pkmn_party_index], selected_pokemon_nickname, 10);
            // Draw level
            shadow_text(TextFormat("Level %u", trainer->pokemon_party.gen2_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].pc_data.level), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 40, 20, WHITE);
            shadow_text("Stats", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 60 : container_rec.x + container_rec.width / 2 + 60, container_rec.y + 70, 20, WHITE);
            shadow_text("IVs", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 140 : container_rec.x + container_rec.width / 2 + 140, container_rec.y + 70, 20, WHITE);
            // Draw stats
            shadow_text("HP:", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 100, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(trainer->pokemon_party.gen2_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].party_data.max_hp)), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 80 : container_rec.x + container_rec.width / 2 + 80, container_rec.y + 100, 20, WHITE);
            shadow_text("Atk:", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 130, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(trainer->pokemon_party.gen2_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].party_data.atk)), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 80 : container_rec.x + container_rec.width / 2 + 80, container_rec.y + 130, 20, WHITE);
            shadow_text("Def:", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 160, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(trainer->pokemon_party.gen2_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].party_data.def)), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 80 : container_rec.x + container_rec.width / 2 + 80, container_rec.y + 160, 20, WHITE);
            shadow_text("Spd:", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 190, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(trainer->pokemon_party.gen2_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].party_data.spd)), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 80 : container_rec.x + container_rec.width / 2 + 80, container_rec.y + 190, 20, WHITE);
            shadow_text("Sp.A:", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 220, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(trainer->pokemon_party.gen2_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].party_data.spatk)), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 80 : container_rec.x + container_rec.width / 2 + 80, container_rec.y + 220, 20, WHITE);
            shadow_text("Sp.D:", trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 250, 20, WHITE);
            shadow_text(TextFormat("%d", pksav_bigendian16(trainer->pokemon_party.gen2_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].party_data.spdef)), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 80 : container_rec.x + container_rec.width / 2 + 80, container_rec.y + 250, 20, WHITE);

            // Draw DVs
            uint8_t pkmn_dv[PKSAV_NUM_GB_IVS];
            pksav_get_gb_IVs(&trainer->pokemon_party.gen2_pokemon_party.party[trainerSelection[current_trainer_index].pkmn_party_index].pc_data.iv_data, pkmn_dv, sizeof(pkmn_dv));
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_HP]), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 140 : container_rec.x + container_rec.width / 2 + 140, container_rec.y + 100, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_ATTACK]), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 140 : container_rec.x + container_rec.width / 2 + 140, container_rec.y + 130, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_DEFENSE]), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 140 : container_rec.x + container_rec.width / 2 + 140, container_rec.y + 160, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_SPEED]), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 140 : container_rec.x + container_rec.width / 2 + 140, container_rec.y + 190, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_SPECIAL]), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 140 : container_rec.x + container_rec.width / 2 + 140, container_rec.y + 220, 20, WHITE);
            shadow_text(TextFormat("%d", pkmn_dv[PKSAV_GB_IV_SPECIAL]), trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 140 : container_rec.x + container_rec.width / 2 + 140, container_rec.y + 250, 20, WHITE);
        }
        // Draw nickname
        shadow_text(selected_pokemon_nickname, trainerSelection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10, container_rec.y + 10, 20, WHITE);
    }
}
