#include "raylibhelper.h"
#include "pksavhelper.h"

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
