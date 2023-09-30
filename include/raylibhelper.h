#include <string.h>
#include "raylib.h"
#include "common.h"

void DrawAboutScreen(void);
void DrawFileEditScreen(struct SaveFileData *save_file_data);
void DrawSettingsScreen(void);
void DrawMainMenuScreen(struct SaveFileData *save_file_data);
void DrawFileSelectScreen(struct SaveFileData *save_file_data, struct pksav_gen2_save *save_player1, struct pksav_gen2_save *save_player2, char *player1_save_path, char *player2_save_path, struct TrainerInfo *trainer1, struct TrainerInfo *trainer2, struct TrainerSelection trainerSelection[2]);
void DrawTradeScreen(struct pksav_gen2_save *save_player1, struct pksav_gen2_save *save_player2, char *player1_save_path, char *player2_save_path, struct TrainerSelection trainerSelection[2], struct TrainerInfo *trainer1, struct TrainerInfo *trainer2);
void DrawRaylibScreenLoop(
    struct SaveFileData *save_file_data,
    struct TrainerInfo *trainer1,
    struct TrainerInfo *trainer2,
    struct TrainerSelection trainerSelection[2],
    char *player1_save_path,
    char *player2_save_path,
    struct pksav_gen2_save *save_player1,
    struct pksav_gen2_save *save_player2
);
