#include "raylibhelper.h"

// Draw Bill's PC for moving PC pokemon to party or between boxes
void draw_bills_pc(PokemonSave *pkmn_save, char *save_path, struct trainer_info *trainer, struct TrainerSelection *trainerSelection, GameScreen *current_screen)
{
    // Update
    int save_generation = pkmn_save->save_generation_type;

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Bill's PC", 50, 50, 20, BLACK);

    // draw_trainer_info(trainer, 50, 100, trainerSelection, pkmn_save->save.gen2_save.save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL, true, );

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
            *current_screen = SCREEN_MAIN_MENU;
        }
    }

    EndDrawing();
}
