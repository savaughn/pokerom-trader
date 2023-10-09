#include <string.h>
#include "raylib.h"
#include "common.h"

#define SCREEN_WIDTH_TEXT_CENTER(text, size) (SCREEN_WIDTH / 2 - MeasureText(text, size) / 2)
#define SCREEN_HEIGHT_TEXT_CENTER(size) (SCREEN_HEIGHT / 2 - size / 2)
#define SCREEN_CENTER(text, size) (Vector2){SCREEN_WIDTH_TEXT_CENTER(text, size), SCREEN_HEIGHT_TEXT_CENTER(size)}


void DrawAboutScreen(void);
void DrawFileEditScreen(struct SaveFileData *save_file_data);
void DrawSettingsScreen(void);
void DrawMainMenuScreen(struct SaveFileData *save_file_data);
void DrawFileSelectScreen(struct SaveFileData *save_file_data, char *player1_save_path, char *player2_save_path, struct TrainerInfo *trainer1, struct TrainerInfo *trainer2, struct TrainerSelection trainerSelection[2], PokemonSave *pokemon_save_player1, PokemonSave *pokemon_save_player2);
void DrawTradeScreen(PokemonSave *save_player1, PokemonSave *save_player2, char *player1_save_path, char *player2_save_path, struct TrainerSelection trainerSelection[2], struct TrainerInfo *trainer1, struct TrainerInfo *trainer2);
void DrawBillsPCFileSelectScreen(struct SaveFileData *save_file_data, PokemonSave *save_player1, char *player1_save_path, struct TrainerInfo *trainer1, struct TrainerSelection *trainerSelection);
void DrawBillsPCScreen(PokemonSave *save_player1, char *player1_save_path, struct TrainerInfo *trainer1, struct TrainerSelection *trainerSelection);
void DrawRaylibScreenLoop(
    struct SaveFileData *save_file_data,
    struct TrainerInfo *trainer1,
    struct TrainerInfo *trainer2,
    struct TrainerSelection trainerSelection[2],
    char *player1_save_path,
    char *player2_save_path,
    PokemonSave *pokemon_save_player1,
    PokemonSave *pokemon_save_player2
);
