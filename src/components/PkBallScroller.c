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
    // Back Shadow
    DrawCircleGradient(pos_x, pos_y, 10 * scale, (Color){0, 0, 0, 255}, (Color){0, 0, 0, 0});
    // Black ouline
    DrawCircle(pos_x, pos_y, 5.25 * scale, (Color){0, 0, 0, 255});
    // Bottom Half white (full-circle)
    DrawCircle(pos_x, pos_y, 5 * scale, (Color){255, 255, 255, 255});
    // Top Half red (semi-circle)
    DrawCircleSector((Vector2){pos_x, pos_y}, 5 * scale, 0, -180, 360, (Color){230, 41, 55, 255});
    // Black cicle outline open-button
    DrawCircle(pos_x, pos_y, 1.75 * scale, (Color){0, 0, 0, 255});
    // Lines from open-button to edges
    DrawLineEx((Vector2){pos_x, pos_y}, (Vector2){pos_x + (5 * scale), pos_y}, scale * 0.2, (Color){0, 0, 0, 255});
    DrawLineEx((Vector2){pos_x, pos_y}, (Vector2){pos_x - (5 * scale), pos_y}, scale * 0.2, (Color){0, 0, 0, 255});
    // white circle open-button
    DrawCircle(pos_x, pos_y, 1.35 * scale, (Color){255, 255, 255, 255});
}
