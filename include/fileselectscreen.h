#ifndef FILESELECTSCREEN_H
#define FILESELECTSCREEN_H

#include <pksav.h>
#include "common.h"

void DrawFileSelectScreen(
    struct pksav_gen2_save *save_player1, 
    struct pksav_gen2_save *save_player2, 
    char *player1_save_path,
    char *player2_save_path,
    struct TrainerInfo *trainer1, 
    struct TrainerInfo *trainer2, 
    struct TrainerSelection trainerSelection[2], 
    struct SaveFileData *save_file_data,
    GameScreen *current_screen, 
    Error_Handler error_handler
);

#endif
