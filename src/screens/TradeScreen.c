#include "raylibhelper.h"
#include "pksavfilehelper.h"

void draw_trade(PokemonSave *save_player1, PokemonSave *save_player2, char *player1_save_path, char *player2_save_path, struct TrainerSelection trainerSelection[2], struct TrainerInfo *trainer1, struct TrainerInfo *trainer2, bool *is_same_generation, GameScreen *current_screen)
{
    // Update
    int selected_index_trainer1 = trainerSelection[0].pkmn_party_index;
    int selected_index_trainer2 = trainerSelection[1].pkmn_party_index;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    draw_trainer_info(trainer1, 50, 50, trainerSelection, save_player1->save.gen2_save.save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL, *is_same_generation);
    draw_trainer_info(trainer2, GetScreenWidth() / 2 + 50, 50, trainerSelection, save_player2->save.gen2_save.save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL, *is_same_generation);
    uint8_t canSubmitTrade = trainerSelection[0].pkmn_party_index != -1 && trainerSelection[1].pkmn_party_index != -1;
    DrawText("Trade!", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, canSubmitTrade ? BLACK : LIGHTGRAY);
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}) && canSubmitTrade)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // Reset Trading state
            trainerSelection[0].pkmn_party_index = -1;
            trainerSelection[1].pkmn_party_index = -1;

            if (*is_same_generation)
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
            *current_screen = SCREEN_FILE_SELECT;
        }
    }

    EndDrawing();
}
