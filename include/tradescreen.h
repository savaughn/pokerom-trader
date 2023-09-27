#ifndef TRADESCREEN_H
#define TRADESCREEN_H

#include "common.h"

void DrawTradeScreen(struct TrainerSelection trainerSelection[2], struct TrainerInfo *trainer1, struct TrainerInfo *trainer2, bool *should_trade, int *current_screen);

#endif // TRADESCREEN_H
