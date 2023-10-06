#ifndef PKSAVHELPER_H
#define PKSAVHELPER_H

#include <pksav.h>
#include "common.h"

int error_handler(enum pksav_error error, const char *message);
PokemonSave loadSaveFromFile(const char *path);
void printTrainerBadges(struct pksav_gen2_save *save);  // TODO: Update for cross-generation
void printTrainerData(struct pksav_gen2_save *save);    // TODO: Update for cross-generation
void saveToFile(PokemonSave *save, char *path);
void printPartyPokemonAtIndex(struct pksav_gen2_save *save, int pokemon_index); // TODO: Update for cross-generation
void printParty(struct pksav_gen2_save *save);          // TODO: Update for cross-generation
void swapPartyPokemonAtIndices(struct pksav_gen2_save *save, int pokemon_index1, int pokemon_index2); // TODO: Update for cross-generation
void swapPokemonAtIndexBetweenSaves(PokemonSave *player1_save, PokemonSave *player2_save, int selected_index1, int selected_index2);
void create_trainer(PokemonSave *save, struct TrainerInfo *trainer);
void updateSeenOwnedPokemon(PokemonSave *save, int pokemon_party_index);
void createTrainerNameStr(struct TrainerInfo *trainer, char *trainer_name, bool showGender);
void createTrainerIdStr(struct TrainerInfo *trainer, char *trainer_id);

#endif /* PKSAVHELPER_H */
