#ifndef COMPONENTS_H
#define COMPONENTS_H

enum E_TOAST_DURATION
{
    TOAST_SHORT = 60,
    TOAST_MEDIUM = 120,
    TOAST_LONG = 240
};

enum E_TOAST_TYPE
{
    TOAST_SUCCESS = 0,
    TOAST_ERROR,
    TOAST_INFO
};

bool draw_menu_button(int x, int y, const char *text, int text_size);
void draw_confirmation_modal(const char *header_text, const char *body_text, const char *submit_button_text, void (*_onsubmit)(void), void (*_oncancel)(void), const uint8_t modal_type);
void draw_trainer_info(struct TrainerInfo *trainer, int x, int y, struct TrainerSelection trainerSelection[2], bool is_same_generation, bool *is_valid_trade);
void reset_details_panel(void);
void draw_save_file_container(PokemonSave *pkmn_save, char *save_name, Rectangle container_rec, bool is_selected);
void draw_corrupted_save(const char *save_name, const int y_offset, const int index, const int corrupted_count);
void draw_pokeball_scroll(float scroll_position, float transparency);
void draw_pokeball(int pos_x, int pos_y, int scale);
bool draw_toast_message(char *message, enum E_TOAST_DURATION duration, enum E_TOAST_TYPE type);
void reset_toast_message(void);
bool draw_save_icon(int pos_x, int pos_y, bool is_visible);

#endif // COMPONENTS_H
