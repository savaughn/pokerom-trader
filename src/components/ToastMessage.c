#include "raylibhelper.h"

bool draw_toast_message(char *message, enum E_TOAST_LENGTH toast_duration) 
{
    const uint8_t toast_height = 50;
    
    static uint16_t display_offset_y = 0;
    static uint16_t display_duration = 0;
    static bool has_reached_top = false;
    
    uint16_t current_position = SCREEN_HEIGHT - display_offset_y;
    uint16_t message_width = MeasureText(message, 20);
    uint16_t toast_width = clamp_max(message_width + 200, SCREEN_WIDTH);
    
    // Animate
    if (!has_reached_top) display_offset_y += 5;
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
        display_offset_y = 0;
        display_duration = 0;
        has_reached_top = false;
        current_position = SCREEN_HEIGHT;
        return true;
    }

    DrawRectangle(SCREEN_WIDTH / 2 - toast_width/2, current_position, toast_width, toast_height, (Color){ 0, 0, 0, 200 });
    DrawText(message, SCREEN_WIDTH_TEXT_CENTER(message, 20), current_position + 15, 20, WHITE);
    draw_pokeball(SCREEN_WIDTH_TEXT_CENTER(message, 20) + message_width + 45, current_position + toast_height/2, 2);
    DrawCircleSector((Vector2){ SCREEN_WIDTH_TEXT_CENTER(message, 20) + message_width + 45, current_position + toast_height/2 }, toast_height/4.5, (float)(display_duration/(float)toast_duration)*360, 360, 0, (Color){0, 0, 0, 200});

    return false;
}