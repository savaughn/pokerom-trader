#include "raylibhelper.h"
#include "pksavhelper.h"
#include <stdio.h>

static int16_t grow_x[2] = {0};
static float scale_width[2] = {1.0f};

void reset_details_panel(void)
{
    grow_x[0] = 0;
    grow_x[1] = 0;
    scale_width[0] = 1.0f;
    scale_width[1] = 1.0f;
}

void animate_details_panel(uint8_t current_trainer_index, bool tr1_active, bool tr2_active)
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
    else if (current_trainer_index == 1)
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
void draw_trainer_info(struct trainer_info *trainer, int x, int y, struct TrainerSelection trainer_selection[2], bool is_same_generation, bool is_valid_trade[2])
{
    // Get trainer generation 1 or 2
    SaveGenerationType trainer_generation = trainer->trainer_generation;

    // Create the trainer name and id strings for Raylib drawing
    char trainer_name[15] = "\0";
    create_trainer_name_str(trainer, trainer_name);
    char trainer_id[11];
    create_trainer_id_str(trainer, trainer_id);
    int current_trainer_index = trainer_selection[0].trainer_id == trainer->trainer_id ? 0 : trainer_selection[1].trainer_id == trainer->trainer_id ? 1
                                                                                                                                                  : -1;
    bool tr1_active = trainer_selection[0].pkmn_party_index != -1 && current_trainer_index == 0;
    bool tr2_active = trainer_selection[1].pkmn_party_index != -1 && current_trainer_index == 1;

    animate_details_panel(current_trainer_index, tr1_active, tr2_active);

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
    switch (trainer_generation)
    {
    case SAVE_GENERATION_1:
        party_count = trainer->pokemon_party.gen1_pokemon_party.count;
        break;
    case SAVE_GENERATION_2:
        party_count = trainer->pokemon_party.gen2_pokemon_party.count;
        break;
    case SAVE_GENERATION_3:
        party_count = trainer->pokemon_party.gen3_pokemon_party.count;
        break;
    default:
        break;
    }

    shadow_text(trainer_name, x - 7, y, 20, WHITE);
    shadow_text(trainer_id, x - 7, y + 30, 20, WHITE);

    int text_pos_x = trainer_selection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 + 10;
    int warn_text_pos_x = trainer_selection[current_trainer_index].trainer_index ? container_rec.x + 10 : container_rec.x + container_rec.width / 2 - 40;
    int stat_text_pos_x = trainer_selection[current_trainer_index].trainer_index ? container_rec.x + 80 : container_rec.x + container_rec.width / 2 + 80;
    int dv_text_pos_x = trainer_selection[current_trainer_index].trainer_index ? container_rec.x + 140 : container_rec.x + container_rec.width / 2 + 140;
    bool is_panel_out = (current_trainer_index == 0 && scale_width[0] > 2.0f) || (current_trainer_index == 1 && grow_x[1] > 200);
    // Draw the pokemon buttons
    for (int party_index = 0; party_index < party_count; party_index++)
    {
        enum eligible_trade_status trade_status = E_TRADE_STATUS_ELIGIBLE;
        if (!is_same_generation)
        {
           trade_status = check_trade_eligibility(trainer, party_index);
        }

        char pokemon_nickname[PKMN_NAME_TEXT_MAX + 1] = "\0";
        switch (trainer_generation)
        {
        case SAVE_GENERATION_1:
            pksav_gen1_import_text(trainer->pokemon_party.gen1_pokemon_party.nicknames[party_index], pokemon_nickname, PKMN_NAME_TEXT_MAX);
            break;
        case SAVE_GENERATION_2:
            pksav_gen2_import_text(trainer->pokemon_party.gen2_pokemon_party.nicknames[party_index], pokemon_nickname, PKMN_NAME_TEXT_MAX);
            break;
        case SAVE_GENERATION_3:
            pksav_gen3_import_text(trainer->pokemon_party.gen3_pokemon_party.party[party_index].pc_data.nickname, pokemon_nickname, PKMN_NAME_TEXT_MAX);
            break;
        default:
            break;
        }

        draw_pkmn_button((Rectangle){x - 10, y + 70 + (party_index * 30), 200, 30}, party_index, pokemon_nickname, current_trainer_index != -1 && (trainer_selection[current_trainer_index].pkmn_party_index == party_index));
        if (trainer_generation == SAVE_GENERATION_2 && trainer->trainer_mail[party_index].item_id != 0)
        {
            draw_mail_icon(x + MeasureText(pokemon_nickname, 20) + 5, y + 75 + (party_index * 30));
        }

        if (trainer_selection[current_trainer_index].pkmn_party_index == party_index && trade_status == E_TRADE_STATUS_MAIL && is_panel_out)
        {
            shadow_text("Pokemon has mail", warn_text_pos_x, container_rec.y + 275, 20, WHITE);
            is_valid_trade[current_trainer_index] = false;
        }
        else if (trainer_selection[current_trainer_index].pkmn_party_index == party_index && trade_status == E_TRADE_STATUS_GEN2_PKMN && is_panel_out)
        {
            shadow_text("Gen 2 only pokemon", warn_text_pos_x, container_rec.y + 275, 20, WHITE);
            is_valid_trade[current_trainer_index] = false;
        }
        else if (trainer_selection[current_trainer_index].pkmn_party_index == party_index && trade_status == E_TRADE_STATUS_GEN2_MOVE && is_panel_out)
        {
            shadow_text("Gen 2 only move", warn_text_pos_x, container_rec.y + 275, 20, WHITE);
            is_valid_trade[current_trainer_index] = false;
        }
        else if (trainer_selection[current_trainer_index].pkmn_party_index == party_index && trade_status == E_TRADE_STATUS_HM_MOVE && is_panel_out)
        {
            shadow_text("Pokemon has HM move", warn_text_pos_x, container_rec.y + 275, 20, WHITE);
            is_valid_trade[current_trainer_index] = false;
        }
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){x - 10, y + 70 + (party_index * 30), 200, 30}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (trainer_selection[current_trainer_index].pkmn_party_index != party_index)
            {
                trainer_selection[current_trainer_index].pkmn_party_index = party_index;
            }
            else
            {
                trainer_selection[current_trainer_index].pkmn_party_index = -1;
            }
        }
    }

    // Draw the selected pokemon nickname
    if (current_trainer_index != -1 && trainer_selection[current_trainer_index].pkmn_party_index != -1 && is_panel_out)
    {
        // Name of the pokemon selected from list
        char selected_pokemon_nickname[PKMN_NAME_TEXT_MAX + 1] = "\0";
        if (trainer_generation == SAVE_GENERATION_1)
        {
            pksav_gen1_import_text(trainer->pokemon_party.gen1_pokemon_party.nicknames[trainer_selection[current_trainer_index].pkmn_party_index], selected_pokemon_nickname, PKMN_NAME_TEXT_MAX);
            struct pksav_gen1_party_pokemon party_pkmn = trainer->pokemon_party.gen1_pokemon_party.party[trainer_selection[current_trainer_index].pkmn_party_index];
            // Draw level
            shadow_text(TextFormat("Level %u", party_pkmn.party_data.level), text_pos_x, container_rec.y + 40, 20, WHITE);
            shadow_text("Stats", trainer_selection[current_trainer_index].trainer_index ? container_rec.x + 60 : container_rec.x + container_rec.width / 2 + 60, container_rec.y + 70, 20, WHITE);
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
        else if (trainer_generation == SAVE_GENERATION_2)
        {
            pksav_gen2_import_text(trainer->pokemon_party.gen2_pokemon_party.nicknames[trainer_selection[current_trainer_index].pkmn_party_index], selected_pokemon_nickname, PKMN_NAME_TEXT_MAX);
            struct pksav_gen2_party_pokemon party_pkmn = trainer->pokemon_party.gen2_pokemon_party.party[trainer_selection[current_trainer_index].pkmn_party_index];
            // Draw level
            shadow_text(TextFormat("Level %u", party_pkmn.pc_data.level), text_pos_x, container_rec.y + 40, 20, WHITE);
            shadow_text("Stats", trainer_selection[current_trainer_index].trainer_index ? container_rec.x + 60 : container_rec.x + container_rec.width / 2 + 60, container_rec.y + 70, 20, WHITE);
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
        } else if (trainer_generation == SAVE_GENERATION_3)
        {
            pksav_gen3_import_text(trainer->pokemon_party.gen3_pokemon_party.party[trainer_selection[current_trainer_index].pkmn_party_index].pc_data.nickname, selected_pokemon_nickname, PKMN_NAME_TEXT_MAX);
            struct pksav_gen3_party_pokemon party_pkmn = trainer->pokemon_party.gen3_pokemon_party.party[trainer_selection[current_trainer_index].pkmn_party_index];
            // Draw level
            shadow_text(TextFormat("Level %u", party_pkmn.party_data.level), text_pos_x, container_rec.y + 40, 20, WHITE);
            shadow_text("Stats", trainer_selection[current_trainer_index].trainer_index ? container_rec.x + 60 : container_rec.x + container_rec.width / 2 + 60, container_rec.y + 70, 20, WHITE);
            shadow_text("EVs", dv_text_pos_x, container_rec.y + 70, 20, WHITE);
            // Draw stats
            shadow_text("HP:", text_pos_x, container_rec.y + 100, 20, WHITE);
            shadow_text(TextFormat("%d", party_pkmn.party_data.max_hp), stat_text_pos_x, container_rec.y + 100, 20, WHITE);
            shadow_text("Atk:", text_pos_x, container_rec.y + 130, 20, WHITE);
            shadow_text(TextFormat("%d", party_pkmn.party_data.atk), stat_text_pos_x, container_rec.y + 130, 20, WHITE);
            shadow_text("Def:", text_pos_x, container_rec.y + 160, 20, WHITE);
            shadow_text(TextFormat("%d", party_pkmn.party_data.def), stat_text_pos_x, container_rec.y + 160, 20, WHITE);
            shadow_text("Spd:", text_pos_x, container_rec.y + 190, 20, WHITE);
            shadow_text(TextFormat("%d", party_pkmn.party_data.spd), stat_text_pos_x, container_rec.y + 190, 20, WHITE);
            shadow_text("Sp.A:", text_pos_x, container_rec.y + 220, 20, WHITE);
            shadow_text(TextFormat("%d", party_pkmn.party_data.spatk), stat_text_pos_x, container_rec.y + 220, 20, WHITE);
            shadow_text("Sp.D:", text_pos_x, container_rec.y + 250, 20, WHITE);
            shadow_text(TextFormat("%d", party_pkmn.party_data.spdef), stat_text_pos_x, container_rec.y + 250, 20, WHITE);

            // Draw EVs
            volatile struct pksav_gen3_pokemon_effort_block pkmn_dv = party_pkmn.pc_data.blocks.effort;
            shadow_text(TextFormat("%u", pkmn_dv.ev_hp), dv_text_pos_x, container_rec.y + 100, 20, WHITE);
            shadow_text(TextFormat("%u", pkmn_dv.ev_atk), dv_text_pos_x, container_rec.y + 130, 20, WHITE);
            shadow_text(TextFormat("%u", pkmn_dv.ev_def), dv_text_pos_x, container_rec.y + 160, 20, WHITE);
            shadow_text(TextFormat("%u", pkmn_dv.ev_spd), dv_text_pos_x, container_rec.y + 190, 20, WHITE);
            shadow_text(TextFormat("%u", pkmn_dv.ev_spatk), dv_text_pos_x, container_rec.y + 220, 20, WHITE);
            shadow_text(TextFormat("%u", pkmn_dv.ev_spdef), dv_text_pos_x, container_rec.y + 250, 20, WHITE);
        }
        // Draw nickname
        shadow_text(selected_pokemon_nickname, text_pos_x, container_rec.y + 10, 20, WHITE);
    }
}
