#include "raylibhelper.h"

void shadow_text(char *text, int pos_x, int pos_y, int font_size, Color color)
{
    DrawText(text, pos_x + 1, pos_y + 1, font_size, BLACK);
    DrawText(text, pos_x, pos_y, font_size, color);
}
