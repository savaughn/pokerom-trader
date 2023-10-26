#ifndef RAYLIBHELPER_H
#define RAYLIBHELPER_H

#include <string.h>
#include "raylib.h"
#include "common.h"
#include "components.h"

#define SCREEN_WIDTH_TEXT_CENTER(text, size) (SCREEN_WIDTH / 2 - MeasureText(text, size) / 2)
#define SCREEN_HEIGHT_TEXT_CENTER(size) (SCREEN_HEIGHT / 2 - size / 2)
#define SCREEN_CENTER(text, size) (Vector2){SCREEN_WIDTH_TEXT_CENTER(text, size), SCREEN_HEIGHT_TEXT_CENTER(size)}
#define BACKGROUND_COLOR RAYWHITE
#define COLOR_PKMN_GOLD (Color) {197,183,131,255}
#define COLOR_PKMN_SILVER (Color) {204,207,214,255}
#define COLOR_PKMN_GREEN (Color) {62,185,94,255}
#define COLOR_PKMN_YELLOW (Color) {255,222,0,255}
#define COLOR_PKMN_DARKYELLOW (Color) {179, 161, 37, 255}
#define COLOR_PKMN_RED (Color){204, 0, 0, 255}

enum E_MODAL_TYPES {
    E_MODAL_WARN,
    E_MODAL_INFO,
    E_MODAL_ERROR,
};

static int no_dir_err = 0;

Texture2D pkrom_trader_logo;
Texture2D trade;
Texture2D evolve;
Texture2D settings;
Texture2D quit;
static Texture2D consoles[10];
static Texture2D pk_balls[4];

void draw_about(GameScreen *current_screen, bool is_build_prerelease);
void draw_legal(GameScreen *current_screen);
void draw_settings(struct SaveFileData *save_file_data, GameScreen *current_screen);
void create_trainer_name_str(const struct TrainerInfo *trainer, char *trainer_name, bool show_gender);
void create_trainer_id_str(const struct TrainerInfo *trainer, char *trainer_id_out);
void draw_change_dir(struct SaveFileData *save_file_data, GameScreen *current_screen);
void draw_pkmn_button(Rectangle rect, int index, char *pokemon_nickname, bool selected);
void draw_main_menu(struct SaveFileData *save_file_data, GameScreen *current_screen, bool *should_close_window);
void draw_file_select(struct SaveFileData *save_file_data, char *player1_save_path, char *player2_save_path, struct TrainerInfo *trainer1, struct TrainerInfo *trainer2, struct TrainerSelection trainer_selection[2], PokemonSave *pkmn_save_player1, PokemonSave *pkmn_save_player2, GameScreen *current_screen, bool *is_same_generation);
void draw_trade(PokemonSave *save_player1, PokemonSave *save_player2, char *player1_save_path, char *player2_save_path, struct TrainerSelection trainer_selection[2], struct TrainerInfo *trainer1, struct TrainerInfo *trainer2, bool *is_same_generation, GameScreen *current_screen);
void draw_file_select_single(struct SaveFileData *save_file_data, PokemonSave *save_player1, char *player1_save_path, struct TrainerInfo *trainer1, struct TrainerSelection *trainer_selection, enum single_player_menu_types menu_type, GameScreen *current_screen);
void draw_bills_pc(PokemonSave *pkmn_save, char *save_path, struct TrainerInfo *trainer, struct TrainerSelection *trainerSelection, GameScreen *current_screen);
void draw_evolve(PokemonSave *pkmn_save, char *save_path, struct TrainerInfo *trainer, GameScreen *current_screen);
void draw_raylib_screen_loop(
    struct SaveFileData *save_file_data,
    struct TrainerInfo *trainer1,
    struct TrainerInfo *trainer2,
    struct TrainerSelection trainer_selection[2],
    char *player1_save_path,
    char *player2_save_path,
    PokemonSave *pkmn_save_player1,
    PokemonSave *pkmn_save_player2
);

#endif // RAYLIBHELPER_H
