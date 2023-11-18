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

void draw_about(GameScreen *current_screen, bool is_build_prerelease);
void draw_legal(GameScreen *current_screen);
void draw_settings(struct save_file_data *save_file_data, GameScreen *current_screen, Texture2D *settings_texture);
void create_trainer_name_str(const struct trainer_info *trainer, char *trainer_name);
void create_trainer_id_str(const struct trainer_info *trainer, char *trainer_id_out);
void draw_change_dir(struct save_file_data *save_file_data, GameScreen *current_screen, Texture2D *settings_texture);
void draw_pkmn_button(Rectangle rect, int index, char *pokemon_nickname, bool selected);
void draw_main_menu(struct save_file_data *save_file_data, GameScreen *current_screen, bool *should_close_window, Texture2D *textures);
void draw_file_select(struct save_file_data *save_file_data, char *player1_save_path, char *player2_save_path, struct trainer_info *trainer1, struct trainer_info *trainer2, struct TrainerSelection trainer_selection[2], PokemonSave *pkmn_save_player1, PokemonSave *pkmn_save_player2, GameScreen *current_screen, bool *is_same_generation);
void draw_trade(PokemonSave *save_player1, PokemonSave *save_player2, char *player1_save_path, char *player2_save_path, struct TrainerSelection trainer_selection[2], struct trainer_info *trainer1, struct trainer_info *trainer2, bool *is_same_generation, GameScreen *current_screen, Texture2D *trade_texture);
void draw_file_select_single(struct save_file_data *save_file_data, PokemonSave *save_player1, char *player1_save_path, struct trainer_info *trainer1, struct TrainerSelection *trainer_selection, enum single_player_menu_types menu_type, GameScreen *current_screen);
void draw_bills_pc(PokemonSave *pkmn_save, char *save_path, struct trainer_info *trainer, struct TrainerSelection *trainerSelection, GameScreen *current_screen);
void draw_evolve(PokemonSave *pkmn_save, char *save_path, struct trainer_info *trainer, GameScreen *current_screen, Texture2D *evolve_texture);
void handle_list_scroll(int *y_offset, const int num_saves, const int corrupted_count, int *mouses_down_index, bool *is_moving_scroll, int *banner_position_offset);
void update_selected_indexes_with_selection(int *selected_saves_index, int *mouses_down_index, bool *is_moving_scroll);
void draw_no_save_files(char *save_path);
void draw_top_banner(const char *text, const int *banner_position_offset);
void draw_background_grid(void);
void shadow_text(const char *text, int pos_x, int pos_y, int font_size, Color color);
void draw_raylib_screen_loop(
    struct save_file_data *save_file_data,
    struct trainer_info *trainer1,
    struct trainer_info *trainer2,
    struct TrainerSelection trainer_selection[2],
    char *player1_save_path,
    char *player2_save_path,
    PokemonSave *pkmn_save_player1,
    PokemonSave *pkmn_save_player2
);

#endif // RAYLIBHELPER_H
