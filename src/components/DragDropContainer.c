#include "raylibhelper.h"

void draw_drag_drop_container(Rectangle container_rec)
{
    DrawRectangleRec(container_rec, (Color){62, 60, 153, 125});

    // draw border
    DrawRectangleLinesEx((Rectangle){container_rec.x - 4, container_rec.y - 4, container_rec.width + 8, container_rec.height + 8}, 1, BLACK);
    DrawRectangleLinesEx((Rectangle){container_rec.x - 3, container_rec.y - 3, container_rec.width + 6, container_rec.height + 6}, 3, (Color){255, 255, 255, 125});
    DrawRectangleLinesEx((Rectangle){container_rec.x, container_rec.y, container_rec.width, container_rec.height}, 1, BLACK);

    // draw text centered horizontally and vertically
    shadow_text("Drag and Drop save files here", container_rec.x + container_rec.width / 2 - MeasureText("Drag and Drop save files here", 20) / 2, container_rec.y + container_rec.height / 2 - 20 / 2, 20, (Color){255, 255, 255, 200});
}
