#include "raylibhelper.h"
#include <stdio.h>

bool draw_save_icon(int pos_x, int pos_y, bool is_visible)
{
    if (is_visible)
    {
        static int16_t frame_counter = 0;

        DrawRectangle(pos_x, pos_y, 15, 20, (Color){255, 255, 255, 255});
        DrawTriangle((Vector2){pos_x + 15, pos_y}, (Vector2){pos_x + 15, pos_y + 5}, (Vector2){pos_x + 20, pos_y + 5}, (Color){255, 255, 255, 255});
        DrawRectangle(pos_x + 15, pos_y + 5, 5, 15, (Color){255, 255, 255, 255});
        DrawCircle(pos_x + 10, pos_y + 12, 4, (Color){230, 41, 55, 255});

        frame_counter++;
        if (frame_counter >= 30)
        {
            frame_counter = 0;
            return false;
        } 
        else
        {
            return true;
        }
    }
    return false;
}
