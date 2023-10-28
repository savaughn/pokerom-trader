#include "raylibhelper.h"
#include "chelper.h"

void draw_confirmation_modal(const char *header_text, const char *body_text, const char *submit_button_text, void (*_onsubmit)(void), void (*_oncancel)(void), uint8_t modal_type)
{
    enum buttons
    {
        BUTTON_NONE = -1,
        BUTTON_CANCEL,
        BUTTON_SUBMIT
    };
    static int selected_index = BUTTON_NONE;
    // Default string values
    char _header_text[60];
    set_default_value_string(header_text, "Are you sure you want to do this action?", _header_text);
    char _body_text[64];
    set_default_value_string(body_text, "This action cannot be undone.", _body_text);
    char submit_text[11];
    set_default_value_string(submit_button_text, "Confirm", submit_text);

    const Color scrim = (Color){0, 0, 0, 30};
    // Background scrim
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, scrim);

    // Modal Rectangle
    Rectangle confirm_modal_rec = (Rectangle){SCREEN_WIDTH / 2 - MeasureText(_header_text, 20) / 2 - 50, SCREEN_HEIGHT / 2 - 150, MeasureText(_header_text, 20) + 100, 300};
    // Bottom Shadow
    DrawRectangleGradientV(confirm_modal_rec.x + 4, confirm_modal_rec.y + confirm_modal_rec.height - 2, confirm_modal_rec.width - 5, 6, LIGHTGRAY, (Color){0, 0, 0, 0});
    // Right Shadow
    DrawRectangleGradientH(confirm_modal_rec.x + confirm_modal_rec.width - 2, confirm_modal_rec.y + 6, 6, confirm_modal_rec.height - 3, LIGHTGRAY, (Color){0, 0, 0, 0});

    // Draw the modal box
    DrawRectangleRec(confirm_modal_rec, WHITE);
    DrawRectangleLinesEx(confirm_modal_rec, 2, LIGHTGRAY);

    if (modal_type == E_MODAL_WARN)
    {
        // Draw Triangle lines
        DrawTriangle(
            (Vector2){confirm_modal_rec.x + confirm_modal_rec.width / 2 - 50, confirm_modal_rec.y + 110},
            (Vector2){confirm_modal_rec.x + confirm_modal_rec.width / 2 + 50, confirm_modal_rec.y + 110},
            (Vector2){confirm_modal_rec.x + confirm_modal_rec.width / 2, confirm_modal_rec.y + 25},
            RED);
        // Draw smaller white triangle to cover the red triangle lines
        DrawTriangle(
            (Vector2){confirm_modal_rec.x + confirm_modal_rec.width / 2 - 40, confirm_modal_rec.y + 104.5},
            (Vector2){confirm_modal_rec.x + confirm_modal_rec.width / 2 + 40, confirm_modal_rec.y + 104.5},
            (Vector2){confirm_modal_rec.x + confirm_modal_rec.width / 2, confirm_modal_rec.y + 35},
            WHITE);
    }

    if (modal_type == E_MODAL_INFO)
    {
        DrawCircle(confirm_modal_rec.x + confirm_modal_rec.width / 2, confirm_modal_rec.y + 75, 45, LIGHTGRAY);
        DrawCircle(confirm_modal_rec.x + confirm_modal_rec.width / 2, confirm_modal_rec.y + 75, 35, WHITE);
    }

    // Draw exclamation point in triangle
    DrawRectangleRounded((Rectangle){confirm_modal_rec.x + confirm_modal_rec.width / 2 - 3.5, confirm_modal_rec.y + 47, 7, 37}, 1, 5, modal_type == E_MODAL_WARN ? RED : LIGHTGRAY);
    DrawCircle(confirm_modal_rec.x + confirm_modal_rec.width / 2, confirm_modal_rec.y + 95, 5, modal_type == E_MODAL_WARN ? RED : LIGHTGRAY);

    // Modal Text
    DrawText(_header_text, confirm_modal_rec.x + confirm_modal_rec.width / 2 - MeasureText(_header_text, 22) / 2, confirm_modal_rec.y + 135, 22, BLACK);
    DrawText(_body_text, confirm_modal_rec.x + confirm_modal_rec.width / 2 - MeasureText(_body_text, 19) / 2, confirm_modal_rec.y + 185, 19, BLACK);

    // Draw Submit Button
    Rectangle submit_button_rec = (Rectangle){confirm_modal_rec.x + confirm_modal_rec.width - 200, confirm_modal_rec.y + confirm_modal_rec.height - 65, MeasureText(submit_text, 20) + 30, 40};
    DrawRectangleRec(submit_button_rec, selected_index == BUTTON_SUBMIT ? LIGHTGRAY : modal_type == E_MODAL_WARN ? RED
                                                                                                     : COLOR_PKMN_GREEN);
    // Button shadow
    if (selected_index != BUTTON_SUBMIT) {
        DrawLine(submit_button_rec.x + submit_button_rec.width + 1, submit_button_rec.y + 1, submit_button_rec.x + submit_button_rec.width + 1, submit_button_rec.y + submit_button_rec.height, BLACK);
        DrawLine(submit_button_rec.x + 1, submit_button_rec.y + submit_button_rec.height + 1, submit_button_rec.x + submit_button_rec.width + 1, submit_button_rec.y + submit_button_rec.height + 1, BLACK);
    }
    
    // Draw Cancel Button
    const char *cancel_button_text = "Cancel";
    Rectangle cancel_button_rec = (Rectangle){confirm_modal_rec.x + 100, submit_button_rec.y, MeasureText(cancel_button_text, 20) + 10, 30};

    DrawText(submit_text, submit_button_rec.x + 15, submit_button_rec.y + 10, 20, selected_index == BUTTON_SUBMIT ? DARKGRAY : BLACK);
    DrawText(cancel_button_text, cancel_button_rec.x + 5, cancel_button_rec.y + 5, 20, selected_index == BUTTON_CANCEL ? LIGHTGRAY : BLACK);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), submit_button_rec))
        {
            selected_index = BUTTON_SUBMIT;
        }
        else if (CheckCollisionPointRec(GetMousePosition(), cancel_button_rec))
        {
            selected_index = BUTTON_CANCEL;
        }
        else
        {
            selected_index = BUTTON_NONE;
        }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        switch (selected_index)
        {
        case BUTTON_CANCEL:
            if (CheckCollisionPointRec(GetMousePosition(), cancel_button_rec) && selected_index == BUTTON_CANCEL)
            {
                _oncancel();
                selected_index = BUTTON_NONE;
            }
            break;
        case BUTTON_SUBMIT:
            if (CheckCollisionPointRec(GetMousePosition(), submit_button_rec) && selected_index == BUTTON_SUBMIT)
            {
                _onsubmit();
                selected_index = BUTTON_NONE;
            }
            break;
        default:
            break;
        }
    }
}
