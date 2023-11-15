#include "raylibhelper.h"

void draw_mail_icon(int x, int y)
{
    uint8_t size_x = 10;
    uint8_t size_y = 7;

    DrawRectangle(x, y, size_x, size_y, WHITE);
    DrawRectangleLinesEx((Rectangle){x - 1, y - 1, size_x + 2, size_y + 2}, 1, BLACK);
    DrawLineEx((Vector2){x, y}, (Vector2){x + size_x / 2, y + (size_y / 2) + 1}, 1, BLACK);
    DrawLineEx((Vector2){x + size_x, y}, (Vector2){x + size_x / 2, y + (size_y / 2) + 1}, 1, BLACK);
}
