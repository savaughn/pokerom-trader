#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "common.h"
#include "tradescreen.h"

void createTrainerNameStr(struct TrainerInfo *trainer, char *trainer_name)
{
    strcpy(trainer_name, "NAME/");
    strcat(trainer_name, trainer->trainer_name);
    strcat(trainer_name, " ");
    strcat(trainer_name, trainer->trainer_gender == PKSAV_GEN2_GENDER_FEMALE ? "F" : "M");
}

void createTrainerIdStr(struct TrainerInfo *trainer, char *trainer_id)
{
    char id_str[6];
    strcpy(trainer_id, "IDNo ");
    snprintf(id_str, sizeof(id_str), "%u", trainer->trainer_id); // "IDNo %u" loses 3 chars even with enough space?
    strcat(trainer_id, id_str);
}

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

void DrawTrainerInfo(struct TrainerInfo *trainer, int x, int y, struct TrainerSelection *trainerSelection)
{
    char trainer_name[15];
    createTrainerNameStr(trainer, trainer_name);
    char trainer_id[11];
    createTrainerIdStr(trainer, trainer_id);

    int current_trainer_index = -1;
    for (int i = 0; i < 2; i++)
    {
        if (trainerSelection[i].trainer_id == trainer->trainer_id)
        {
            current_trainer_index = i;
        }
    }

    int party_count = trainer->pokemon_party->count;
    static char selected_pokemon_nickname[11];

    DrawText(trainer_name, x, y, 20, BLACK);
    DrawText(trainer_id, x, y + 30, 20, BLACK);

    for (int i = 0; i < party_count; i++)
    {
        char pokemon_nickname[11];
        pksav_gen2_import_text(trainer->pokemon_party->nicknames[i], pokemon_nickname, 10);
        PokemonButton((Rectangle){x - 10, y + 70 + (i * 30), 200, 30}, i, pokemon_nickname);
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){x - 10, y + 70 + (i * 30), 200, 30}) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            trainerSelection[current_trainer_index].pokemon_index = i;
        }
    }

    if (trainerSelection[current_trainer_index].pokemon_index != -1)
    {
        pksav_gen2_import_text(trainer->pokemon_party->nicknames[trainerSelection[current_trainer_index].pokemon_index], selected_pokemon_nickname, 10);
        DrawText(selected_pokemon_nickname, trainerSelection[current_trainer_index].trainer_index ? (GetScreenWidth() / 2) + 50 : x, y + 300, 20, BLACK);
    }
}

void DrawTradeScreen(struct TrainerSelection *trainerSelection, struct TrainerInfo trainer1, struct TrainerInfo trainer2, bool *should_trade)
{
    // Trainer 1
    DrawTrainerInfo(&trainer1, 50, 50, trainerSelection);
    DrawTrainerInfo(&trainer2, GetScreenWidth() / 2 + 50, 50, trainerSelection);
    // draw a button with rectangle lines
    DrawRectangleLines(GetScreenWidth() / 2 - 50, GetScreenHeight() / 2 + 50, 100, 50, BLACK);
    // draw text trade in the middle of the button
    DrawText("Trade", GetScreenWidth() / 2 - 30, GetScreenHeight() / 2 + 65, 20, BLACK);
    // check if the mouse is over the button
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){GetScreenWidth() / 2 - 50, GetScreenHeight() / 2 + 50, 100, 50}))
    {
        // draw a rectangle over the button
        DrawRectangle(GetScreenWidth() / 2 - 50, GetScreenHeight() / 2 + 50, 100, 50, GRAY);
        // check if the mouse is clicked
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            *should_trade = true;
        }
    }
    else
    {
        *should_trade = false;
    }
}
