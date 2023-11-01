#ifndef COMPONENTS_H
#define COMPONENTS_H

bool draw_menu_button(int x, int y, const char *text, int text_size);
void draw_confirmation_modal(const char *header_text, const char *body_text, const char *submit_button_text, void (*_onsubmit)(void), void (*_oncancel)(void), const uint8_t modal_type);
void draw_trainer_info(struct TrainerInfo *trainer, int x, int y, struct TrainerSelection trainerSelection[2], bool showGender, bool is_same_generation);
void DrawSaveFileContainer(PokemonSave *pkmn_save, char *save_name, Rectangle container_rec, bool is_selected);
void draw_corrupted_save(const char *save_name, const int y_offset, const int index, const int corrupted_count);
void draw_pokeball_scroll(float scroll_position, float transparency);

#endif // COMPONENTS_H
