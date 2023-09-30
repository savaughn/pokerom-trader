#ifndef PKSAVHELPER_H
#define PKSAVHELPER_H

#include <pksav.h>
#include "common.h"

int error_handler(enum pksav_error error, const char *message);
struct pksav_gen2_save loadSaveFromFile(const char *path);
void printTrainerBadges(struct pksav_gen2_save *save);
void printTrainerData(struct pksav_gen2_save *save);
void saveToFile(struct pksav_gen2_save *save, char *path);
void printPartyPokemonAtIndex(struct pksav_gen2_save *save, int pokemon_index);
void printParty(struct pksav_gen2_save *save);
void swapPartyPokemonAtIndices(struct pksav_gen2_save *save, int pokemon_index1, int pokemon_index2);
void swapPokemonAtIndexBetweenSaves(struct pksav_gen2_save *player1_save, struct pksav_gen2_save *player2_save, int selected_index1, int selected_index2);
void create_trainer(struct pksav_gen2_save *save, struct TrainerInfo *trainer);
void updateSeenOwnedPokemon(struct pksav_gen2_save *save, int pokemon_party_index);
void createTrainerNameStr(struct TrainerInfo *trainer, char *trainer_name);
void createTrainerIdStr(struct TrainerInfo *trainer, char *trainer_id);

#endif /* PKSAVHELPER_H */
