#include "raylibhelper.h"

#define COLOR_PKMN_YELLOW (Color) {255,222,0,255}
#define COLOR_PKMN_DARKYELLOW (Color) {179, 161, 37, 255}

bool draw_menu_button(int x, int y, const char *text, int text_size)
{
    const int rec_height = 45;
    const int line_width = 3;
    Color selected_color = COLOR_PKMN_YELLOW;

    // on mouse hover
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){x, y, 150, rec_height}))
    {
        bool clicked = false;
        int selected_offset = 0;
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            clicked = true;
            selected_offset = 2;
            selected_color = COLOR_PKMN_DARKYELLOW;
        }

        // Shadow
        if (!clicked) {
            DrawRectangleGradientH(x + 140, y - 2 + 2, 6, rec_height + 6, DARKGRAY, (Color){0, 0, 0, 0});
            DrawRectangleGradientV(x - 2, y + rec_height + 1, 142, 6, DARKGRAY, (Color){0, 0, 0, 0});
        }

        // Hovered box
        DrawTriangle((Vector2){x - 10 + selected_offset, y - 2}, (Vector2){x - 20 + selected_offset, y - 2}, (Vector2){x - 10 + selected_offset, y + rec_height + 1}, selected_color);
        DrawRectangle(x - 10 + selected_offset, y - 2, 150, rec_height + 4, selected_color);
        DrawText(text, x + selected_offset, y + 7, text_size, BLACK);

        // Box outline
        DrawLineEx((Vector2){x - 20 + selected_offset, y - 2}, (Vector2){x + 140 + selected_offset, y - 2}, line_width, BLACK);
        DrawLineEx((Vector2){x - 10 + selected_offset, y + rec_height + 1}, (Vector2){x + 140 + selected_offset, y + rec_height + 1}, line_width, BLACK);
        DrawLineEx((Vector2){x - 20 + selected_offset, y - 2}, (Vector2){x - 10 + selected_offset, y + rec_height + 1}, line_width, BLACK);
        DrawLineEx((Vector2){x + 140 + selected_offset, y - 2}, (Vector2){x + 140 + selected_offset, y + rec_height + 2}, line_width, BLACK);

        return true;
    }
    else
    {
        DrawText(text, x + 10, y + 5, text_size, BLACK);
        return false;
    }
}
