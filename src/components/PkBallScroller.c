#include "raylibhelper.h"

void draw_pokeball_scroll(float scroll_position, float transparency)
{
    DrawCircleGradient(SCREEN_WIDTH - 10, 75 + (scroll_position * (SCREEN_HEIGHT - 150)), 10, (Color){0, 0, 0, transparency}, (Color){0, 0, 0, 0});
    DrawCircle(SCREEN_WIDTH - 10, 75 + (scroll_position * (SCREEN_HEIGHT - 150)), 5, (Color){255, 255, 255, transparency});
    DrawCircleSector((Vector2){SCREEN_WIDTH - 10, 74 + (scroll_position * (SCREEN_HEIGHT - 150))}, 5, 0, -180, 6, (Color){230, 41, 55, transparency});
    DrawCircle(SCREEN_WIDTH - 10, 75 + (scroll_position * (SCREEN_HEIGHT - 150)), 2, (Color){0, 0, 0, transparency});
    DrawLine(SCREEN_WIDTH - 10 - 5, 75 + (scroll_position * (SCREEN_HEIGHT - 150)), SCREEN_WIDTH - 10 + 5, 75 + (scroll_position * (SCREEN_HEIGHT - 150)), (Color){0, 0, 0, transparency});
    DrawCircle(SCREEN_WIDTH - 10, 75 + (scroll_position * (SCREEN_HEIGHT - 150)), 1, (Color){255, 255, 255, transparency});
}

void draw_pokeball(int pos_x, int pos_y, int scale)
{
    DrawCircleGradient(pos_x, pos_y, 10 * scale, (Color){0, 0, 0, 255}, (Color){0, 0, 0, 0});
    DrawCircle(pos_x, pos_y, 5 * scale, (Color){255, 255, 255, 255});
    DrawCircleSector((Vector2){pos_x, pos_y}, 5 * scale, 0, -180, 360, (Color){230, 41, 55, 255});
    DrawCircle(pos_x, pos_y, 2 * scale, (Color){0, 0, 0, 255});
    DrawLine(pos_x, pos_y, pos_x + (5 * scale), pos_y, (Color){0, 0, 0, 255});
    DrawCircle(pos_x, pos_y, 1* scale, (Color){255, 255, 255, 255});
}
