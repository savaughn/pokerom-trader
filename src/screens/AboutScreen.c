#include "raylibhelper.h"

void draw_about(GameScreen *current_screen, bool is_build_prerelease)
{   
    int x = 50;
    enum screen_buttons
    {
        SCREEN_BUTTON_NONE = -1,
        SCREEN_BUTTON_BACK,
        SCREEN_BUTTON_NEXT
    };
    static int selected_index = SCREEN_BUTTON_NONE;
    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);

    DrawText("About Pokerom Trader", x, 100, 20, BLACK);
    DrawText(TextFormat("v%s%s", PROJECT_VERSION, is_build_prerelease ? "-prerelease" : ""), x, 125, 20, BLACK);
    DrawText("Pokerom Trader is a tool for trading Pokemon between two save files", x, 200, 20, BLACK);
    DrawText("written by github.com/savaughn 2023", x, 225, 20, BLACK);
    DrawText("Pokerom Trader is open source and licensed under the MIT license", x, 250, 20, BLACK);
    DrawText("Pokerom Trader uses the following libraries:", x, 300, 20, BLACK);
    DrawText("raylib - https://www.raylib.com/", x, 325, 20, BLACK);
    DrawText("pksav - https://github.com/savaughn/pksav", x, 350, 20, BLACK);
    DrawText("Legal >", NEXT_BUTTON_X, NEXT_BUTTON_Y, 20, selected_index == SCREEN_BUTTON_NEXT ? LIGHTGRAY : BLACK);
    DrawText("< Back", BACK_BUTTON_X, BACK_BUTTON_Y, 20, selected_index == SCREEN_BUTTON_BACK ? LIGHTGRAY : BLACK);

    EndDrawing();

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
        {
            selected_index = SCREEN_BUTTON_NEXT;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
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
        switch (selected_index)
        {
        case SCREEN_BUTTON_BACK:
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){BACK_BUTTON_X - 15, BACK_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
            {
                *current_screen = SCREEN_SETTINGS;
                selected_index = SCREEN_BUTTON_NONE;
            }
            break;
        case SCREEN_BUTTON_NEXT:
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){NEXT_BUTTON_X - 15, NEXT_BUTTON_Y - 30, BUTTON_WIDTH, BUTTON_HEIGHT}))
            {
                *current_screen = SCREEN_LEGAL;
                selected_index = SCREEN_BUTTON_NONE;
            }
            break;
        default:
            break;
        }
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        *current_screen = SCREEN_SETTINGS;
        selected_index = SCREEN_BUTTON_NONE;
    }
}
