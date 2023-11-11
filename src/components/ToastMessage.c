#include "raylibhelper.h"

static uint16_t display_offset_y = 0;
static uint16_t display_duration = 0;
static bool has_reached_top = false;

void reset_toast_message(void)
{
    display_offset_y = 0;
    display_duration = 0;
    has_reached_top = false;
}

bool draw_toast_message(char *message, enum E_TOAST_DURATION toast_duration, enum E_TOAST_TYPE type)
{
    const uint8_t toast_height = 50;

    uint16_t current_position = SCREEN_HEIGHT - display_offset_y;
    uint16_t message_width = MeasureText(message, 20);
    uint16_t toast_width = clamp_max(message_width + 200, SCREEN_WIDTH);
    Color message_color = WHITE;

    switch (type)
    {
    case TOAST_SUCCESS:
        message_color = GREEN;
        break;
    case TOAST_ERROR:
        message_color = RED;
        break;
    case TOAST_INFO:
        message_color = WHITE;
        break;
    default:
        break;
    }

    // Animate
    if (!has_reached_top)
    {
        display_offset_y += 5;
    }
    if (current_position < SCREEN_HEIGHT - toast_height - 25)
    {
        current_position = SCREEN_HEIGHT - toast_height - 25;
        has_reached_top = true;
        display_duration++;
    }
    if (display_duration >= toast_duration)
    {
        display_duration = toast_duration;
        display_offset_y -= 10;
    }
    if (has_reached_top && current_position >= SCREEN_HEIGHT)
    {
        reset_toast_message();
        return true;
    }

    DrawRectangle(SCREEN_WIDTH / 2 - toast_width / 2, current_position, toast_width, toast_height, (Color){0, 0, 0, 200});
    DrawText(message, SCREEN_WIDTH_TEXT_CENTER(message, 20), current_position + 15, 20, message_color);
    draw_pokeball(SCREEN_WIDTH_TEXT_CENTER(message, 20) + message_width + 45, current_position + toast_height / 2, 2);
    DrawCircleSector((Vector2){SCREEN_WIDTH_TEXT_CENTER(message, 20) + message_width + 45, current_position + toast_height / 2}, toast_height / 4.5, (float)(display_duration / (float)toast_duration) * 360, 360, 0, (Color){0, 0, 0, 200});
    DrawRectangleLinesEx((Rectangle){SCREEN_WIDTH / 2 - toast_width / 2 - 1, current_position - 1, toast_width + 2, toast_height + 2}, 2, WHITE);
    DrawRectangleLinesEx((Rectangle){SCREEN_WIDTH / 2 - toast_width / 2 - 2, current_position - 2, toast_width + 4, toast_height + 4}, 1, BLACK);

    return false;
}
