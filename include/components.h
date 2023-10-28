#ifndef COMPONENTS_H
#define COMPONENTS_H

bool draw_menu_button(int x, int y, const char *text, int text_size);
void draw_confirmation_modal(const char *header_text, const char *body_text, const char *submit_button_text, void (*_onsubmit)(void), void (*_oncancel)(void), const uint8_t modal_type);
void draw_trainer_info(struct TrainerInfo *trainer, int x, int y, struct TrainerSelection trainerSelection[2], bool showGender);

#endif // COMPONENTS_H
