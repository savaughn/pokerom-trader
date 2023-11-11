#include "raylibhelper.h"
#include "pksavhelper.h"

void draw_save_file_container(PokemonSave *pkmn_save, char *save_name, Rectangle container_rec, bool is_selected)
{
    char trainer_name[TRAINER_NAME_TEXT_MAX + 1] = "\0";
    uint16_t trainer_id;
    uint16_t hours = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;

    if (pkmn_save->save_generation_type == SAVE_GENERATION_1)
    {
        pksav_gen1_import_text(pkmn_save->save.gen1_save.trainer_info.p_name, trainer_name, TRAINER_NAME_TEXT_MAX);
        trainer_id = pksav_bigendian16(*pkmn_save->save.gen1_save.trainer_info.p_id);
        struct pksav_gen1_time *save_time = pkmn_save->save.gen1_save.p_time_played;
        hours = pksav_littleendian16(save_time->hours);
        minutes = save_time->minutes;
        seconds = save_time->seconds;
    }
    else if (pkmn_save->save_generation_type == SAVE_GENERATION_2)
    {
        pksav_gen2_import_text(pkmn_save->save.gen2_save.trainer_info.p_name, trainer_name, TRAINER_NAME_TEXT_MAX);
        trainer_id = pksav_bigendian16(*pkmn_save->save.gen2_save.trainer_info.p_id);
        const struct pksav_gen2_time *save_time = pkmn_save->save.gen2_save.save_time.p_time_played;
        hours = save_time->hours;
        minutes = save_time->minutes;
        seconds = save_time->seconds;
    }
    else
    {
        return;
    }

    DrawRectangleRec(container_rec, (Color){62, 60, 153, is_selected ? 150 : 255});

    // draw trainer info box
    DrawRectangle(container_rec.x, container_rec.y, 125 + 5, container_rec.height, (Color){100, 101, 193, is_selected ? 150 : 255});
    DrawRectangleLines(container_rec.x, container_rec.y, 125 + 5, container_rec.height, BLACK);

    // draw trainer name
    shadow_text(trainer_name, container_rec.x + 5, container_rec.y + 5, 20, WHITE);

    // draw trainer id
    shadow_text(TextFormat("ID No.%05u", trainer_id), container_rec.x + 5, container_rec.y + 5 + 25, 20, WHITE);

    shadow_text(TextFormat("%dh%02dm%02ds", hours, minutes, seconds), container_rec.x + 5, container_rec.y + 5 + 50, 20, WHITE);

    // pokemon party grid
    DrawLine(container_rec.x + 130, container_rec.y + (container_rec.height - 30) / 2, container_rec.x + container_rec.width, container_rec.y + (container_rec.height - 30) / 2, BLACK);
    DrawLine(container_rec.x + 130 + (container_rec.width - 135) / 3, container_rec.y, container_rec.x + 130 + (container_rec.width - 135) / 3, container_rec.y + container_rec.height - 30, BLACK);
    DrawLine(container_rec.x + 130 + (container_rec.width - 135) / 3 * 2, container_rec.y, container_rec.x + 130 + (container_rec.width - 135) / 3 * 2, container_rec.y + container_rec.height - 30, BLACK);

    Rectangle name_slots[6] = {
        (Rectangle){container_rec.x + 135, container_rec.y + 5, (container_rec.width - 135) / 3, (container_rec.height - 30) / 2},
        (Rectangle){container_rec.x + 135 + (container_rec.width - 135) / 3, container_rec.y + 5, (container_rec.width - 135) / 3, (container_rec.height - 30) / 2},
        (Rectangle){container_rec.x + 135 + (container_rec.width - 135) / 3 * 2, container_rec.y + 5, (container_rec.width - 135) / 3, (container_rec.height - 30) / 2},
        (Rectangle){container_rec.x + 135, container_rec.y + 5 + (container_rec.height - 30) / 2, (container_rec.width - 135) / 3, (container_rec.height - 30) / 2},
        (Rectangle){container_rec.x + 135 + (container_rec.width - 135) / 3, container_rec.y + 5 + (container_rec.height - 30) / 2, (container_rec.width - 135) / 3, (container_rec.height - 30) / 2},
        (Rectangle){container_rec.x + 135 + (container_rec.width - 135) / 3 * 2, container_rec.y + 5 + (container_rec.height - 30) / 2, (container_rec.width - 135) / 3, (container_rec.height - 30) / 2},
    };

    // draw pokemon party
    if (pkmn_save->save_generation_type == SAVE_GENERATION_1)
    {
        for (int i = 0; i < pkmn_save->save.gen1_save.pokemon_storage.p_party->count; i++)
        {
            char pokemon_name[PKMN_NAME_TEXT_MAX + 1] = "\0";
            pksav_gen1_import_text(pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[i], pokemon_name, PKMN_NAME_TEXT_MAX);
            shadow_text(pokemon_name, name_slots[i].x, name_slots[i].y, 20, WHITE);
            shadow_text(TextFormat(" L%d", pkmn_save->save.gen1_save.pokemon_storage.p_party->party[i].party_data.level), (name_slots[i].x + ((container_rec.width - 135) / 3)) - 60, name_slots[i].y, 20, WHITE);
        }
    }
    else if (pkmn_save->save_generation_type == SAVE_GENERATION_2)
    {
        for (int i = 0; i < pkmn_save->save.gen2_save.pokemon_storage.p_party->count; i++)
        {
            char pokemon_name[PKMN_NAME_TEXT_MAX + 1] = "\0";
            pksav_gen2_import_text(pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[i], pokemon_name, PKMN_NAME_TEXT_MAX);
            shadow_text(pokemon_name, name_slots[i].x, name_slots[i].y, 20, WHITE);
            shadow_text(TextFormat(" L%d", pkmn_save->save.gen2_save.pokemon_storage.p_party->party[i].pc_data.level), (name_slots[i].x + ((container_rec.width - 135) / 3)) - 60, name_slots[i].y, 20, WHITE);
        }
    }
    else
    {
        return;
    }

    // draw save name
    DrawLine(container_rec.x + 130, container_rec.y + container_rec.height - 30, container_rec.width + container_rec.x, container_rec.y + container_rec.height - 30, BLACK);
    char save_name_text[39] = "\0";
    strncpy(save_name_text, save_name, 32);
    if (strlen(save_name) > 32)
    {
        strcat(save_name_text, "...sav");
    }
    shadow_text(save_name_text, (container_rec.width - 135) / 2 + container_rec.x + 135 - MeasureText(save_name_text, 20) / 2, container_rec.y + container_rec.height - 25, 20, WHITE);

    // draw border
    DrawRectangleLinesEx((Rectangle){container_rec.x - 4, container_rec.y - 4, container_rec.width + 8, container_rec.height + 8}, 1, BLACK);
    DrawRectangleLinesEx((Rectangle){container_rec.x - 3, container_rec.y - 3, container_rec.width + 6, container_rec.height + 6}, 3, WHITE);
    DrawRectangleLinesEx((Rectangle){container_rec.x, container_rec.y, container_rec.width, container_rec.height}, 1, BLACK);

    if (is_selected)
    {
        DrawRectangleGradientH(container_rec.x, container_rec.y, 200, container_rec.height, (Color){0, 0, 0, 175}, (Color){0, 0, 0, 0});
        DrawRectangleGradientH(container_rec.x + container_rec.width - 200, container_rec.y, 200, container_rec.height, (Color){0, 0, 0, 0}, (Color){0, 0, 0, 175});
    }
}

void draw_corrupted_save(const char *save_name, const int y_offset, const int index, const int corrupted_count)
{
    DrawRectangle(30 + (SCREEN_WIDTH - 50) / 12, y_offset + (93 * index) - (60 * corrupted_count) - 2, SCREEN_WIDTH - 60 - (2 * (SCREEN_WIDTH - 50) / 12), 25, (Color){255, 255, 255, 125});
    DrawRectangleGradientH(30, y_offset + (93 * index) - (60 * corrupted_count) - 2, (SCREEN_WIDTH - 50) / 12, 25, (Color){255, 255, 255, 0}, (Color){255, 255, 255, 125});
    DrawRectangleGradientH(SCREEN_WIDTH - 31 - (SCREEN_WIDTH - 50) / 12, y_offset + (93 * index) - (60 * corrupted_count) - 2, (SCREEN_WIDTH - 50) / 12, 25, (Color){255, 255, 255, 125}, (Color){255, 255, 255, 0});
    DrawText(save_name, 100, y_offset + (93 * index) - (60 * corrupted_count) + 2, 20, DARKGRAY);
    DrawText("save file invalid", 100 + MeasureText(save_name, 20) + 10, y_offset + (93 * index) - (60 * corrupted_count), 15, BLACK);
}
