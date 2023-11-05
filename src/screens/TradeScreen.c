#include "raylibhelper.h"
#include "pksavfilehelper.h"

void draw_trade(PokemonSave *save_player1, PokemonSave *save_player2, char *player1_save_path, char *player2_save_path, struct TrainerSelection trainerSelection[2], struct TrainerInfo *trainer1, struct TrainerInfo *trainer2, bool *is_same_generation, GameScreen *current_screen, Texture2D *trade_texture)
{
    // Update
    int selected_index_trainer1 = trainerSelection[0].pkmn_party_index;
    int selected_index_trainer2 = trainerSelection[1].pkmn_party_index;
    static enum ui_selections {
        BUTTON_NONE = -1,
        BUTTON_TRADE,
        BUTTON_BACK,
    } ui_selection = BUTTON_NONE;
    static bool show_trade_toast = false;
    static bool show_saving_icon = false;

    BeginDrawing();
    ClearBackground(RED);
    draw_background_grid();

    DrawTextureEx(*trade_texture, (Vector2){50, 10}, 0, 0.4, WHITE);

    bool is_valid_trade[2] = {true};
    draw_trainer_info(trainer1, 25, (SCREEN_HEIGHT - 100)/2 - 75, trainerSelection, save_player1->save.gen2_save.save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL, *is_same_generation, &is_valid_trade);
    draw_trainer_info(trainer2, SCREEN_WIDTH - 178, (SCREEN_HEIGHT - 100)/2-75, trainerSelection, save_player2->save.gen2_save.save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL, *is_same_generation, &is_valid_trade);
    bool can_submit_trade = trainerSelection[0].pkmn_party_index != -1 && trainerSelection[1].pkmn_party_index != -1 && is_valid_trade[0] && is_valid_trade[1];

    // Bottom bar
    const Rectangle bottom_bar_rec = (Rectangle){0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, 100};
    DrawRectangleRec(bottom_bar_rec, WHITE);
    DrawLineEx((Vector2){bottom_bar_rec.x, bottom_bar_rec.y}, (Vector2){bottom_bar_rec.width, bottom_bar_rec.y}, 15, BLACK);

    const Rectangle trade_button_rec = (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y + 8, BUTTON_WIDTH, BUTTON_HEIGHT};
    DrawText("Trade !", trade_button_rec.x + 15, trade_button_rec.y + 10, 20, can_submit_trade ? ui_selection == BUTTON_TRADE ? LIGHTGRAY : BLACK : LIGHTGRAY);

    const Rectangle back_button_rec = (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y + 8, BUTTON_WIDTH, BUTTON_HEIGHT};
    DrawText("< Back", back_button_rec.x + 15, back_button_rec.y + 10, 20, ui_selection == BUTTON_BACK ? LIGHTGRAY : BLACK);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
        {
            ui_selection = BUTTON_TRADE;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
        {
            ui_selection = BUTTON_BACK;
        }
        else
        {
            ui_selection = BUTTON_NONE;
        }
    }
    
    static pksavhelper_error pksavhelper_error = error_none;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && ui_selection != BUTTON_NONE)
    {
        switch (ui_selection)
        {
        case BUTTON_TRADE:
            if (can_submit_trade && CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
            {
                // Reset Trading state
                trainerSelection[0].pkmn_party_index = -1;
                trainerSelection[1].pkmn_party_index = -1;

                if (*is_same_generation)
                {
                    pksavhelper_error = swap_pkmn_at_index_between_saves(save_player1, save_player2, selected_index_trainer1, selected_index_trainer2);
                }
                else
                {
                    pksavhelper_error = swap_pkmn_at_index_between_saves_cross_gen(save_player1, save_player2, selected_index_trainer1, selected_index_trainer2);
                }
                if (pksavhelper_error != error_none)
                {
                    show_trade_toast = true;
                    break;
                }
                pksavhelper_error = update_seen_owned_pkmn(save_player1, selected_index_trainer1);
                pksavhelper_error = update_seen_owned_pkmn(save_player2, selected_index_trainer2);
                if (pksavhelper_error != error_none)
                {
                    show_trade_toast = true;
                    break;
                }
                pksavhelper_error = save_savefile_to_path(save_player1, player1_save_path);
                pksavhelper_error = save_savefile_to_path(save_player2, player2_save_path);
                if (pksavhelper_error != error_none)
                {
                    show_trade_toast = true;
                    break;
                }
                show_saving_icon = true;

                // Recreate trainer info with updated saves
                create_trainer(save_player1, trainer1);
                create_trainer(save_player2, trainer2);

                show_trade_toast = true;
            }
            else
            {
                ui_selection = BUTTON_NONE;
            }
            break;
        case BUTTON_BACK:
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
            {
                trainer1->trainer_id = 0;
                trainer2->trainer_id = 0;
                trainerSelection[0].pkmn_party_index = -1;
                trainerSelection[1].pkmn_party_index = -1;
                *current_screen = SCREEN_FILE_SELECT;

                ui_selection = BUTTON_NONE;
            }
            break;
        default:
            break;
        }
        ui_selection = BUTTON_NONE;
    }

    // Draw Toast with status message
    if (show_trade_toast)
    {
        char *toast_message = NULL;
        switch (pksavhelper_error)
        {
        case error_none:
            toast_message = "Trade successful!";
            break;
        case error_update_pokedex:
            toast_message = "Pokédex update failed!";
            break;
        case error_swap_pkmn:
            toast_message = "Pokémon trade failed!";
            break;
        case error_update_save:
            toast_message = "Save update failed!";
            break;
        default:
            break;
        }
        
        show_trade_toast = !draw_toast_message(toast_message, TOAST_LONG, TOAST_INFO);
    }
    else
    {
        pksavhelper_error = error_none;
    }
    if (show_saving_icon)
    {
        show_saving_icon = draw_save_icon(SCREEN_WIDTH - 50, 10, show_saving_icon);
    }

    EndDrawing();
}
