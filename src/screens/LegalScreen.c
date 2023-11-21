#include "raylibhelper.h"

void draw_legal(GameScreen *current_screen)
{
    const char *disclaimer_lines[] = {
        "Pokerom Trader is an unofficial application and is not",
        "affiliated with or endorsed by Nintendo, Game Freak, Creatures,",
        "The Pokémon Company, or any related entities. Pokémon and Pokémon",
        "character names are trademarks of Nintendo, Game Freak, Creatures,",
        "and The Pokémon Company. All trademarks, character names, and other",
        "intellectual property used in this application are used for",
        "identification and informational purposes only. The use of",
        "these names and marks is believed to qualify as fair use under",
        "trademark law. Pokerom Trader is not endorsed by or affiliated with",
        "any of the aforementioned entities. Pokerom Trader is provided \"as is\"",
        "without warranty of any kind, and the developers make no warranties,",
        "express or implied, regarding the accuracy or completeness of the",
        "content provided in this application."};

    enum screen_buttons
    {
        SCREEN_BUTTON_NONE = -1,
        SCREEN_BUTTON_BACK
    };
    static int selected_index = SCREEN_BUTTON_NONE;

    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);
    DrawText("Disclaimer", 50, 50, 20, BLACK);
    for (int i = 0; i < 13; i++)
    {
        DrawText(disclaimer_lines[i], 50, 75 + (i * 25), 20, BLACK);
    }

    const Rectangle back_button_rec = (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT};
    DrawText("< Back", back_button_rec.x + 15, back_button_rec.y + 30, 20, selected_index == SCREEN_BUTTON_BACK ? LIGHTGRAY : BLACK);
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), back_button_rec))
        {
            selected_index = SCREEN_BUTTON_BACK;
        }
        else
        {
            selected_index = SCREEN_BUTTON_NONE;
        }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), back_button_rec))
        {
            *current_screen = SCREEN_ABOUT;
            selected_index = SCREEN_BUTTON_NONE;
        }
    }
    EndDrawing();

    if (IsKeyPressed(KEY_ESCAPE))
    {
        *current_screen = SCREEN_ABOUT;
        selected_index = SCREEN_BUTTON_NONE;
    }
}
