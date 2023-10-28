#ifndef PKSAVHELPER_H
#define PKSAVHELPER_H

#include "common.h"
#include "pkmnstats.h"

#define MAX_GEN1_MOVE_INDEX 165
#define MOVE_INDEX_HM01 15
#define MOVE_INDEX_HM02 19
#define MOVE_INDEX_HM03 57
#define MOVE_INDEX_HM04 70
#define MOVE_INDEX_HM05 127
#define MOVE_INDEX_HM06 165

struct pkmn_evolution_pair_data
{
    char species_name[11];
    char evolution_name[11];
    uint8_t species_index;
    uint8_t evolution_index;
    uint8_t evolution_item;
};

enum eligible_evolution_status
{
    E_EVO_STATUS_NOT_ELIGIBLE,
    E_EVO_STATUS_ELIGIBLE,
    E_EVO_STATUS_MISSING_ITEM
};

enum eligible_trade_status
{
    E_TRADE_STATUS_ELIGIBLE,
    E_TRADE_STATUS_GEN2_PKMN,
    E_TRADE_STATUS_GEN2_MOVE,
    E_TRADE_STATUS_HM_MOVE,
};

// Pokémon Evolution Pair Lookup Table
static const struct pkmn_evolution_pair_data pkmn_evolution_pairs_gen1[SI_HAUNTER + 1] = {
    [SI_KADABRA] = {.species_name = "KADABRA", .evolution_name = "ALAKAZAM", .species_index = SI_KADABRA, .evolution_index = SI_ALAKAZAM},
    [SI_MACHOKE] = {.species_name = "MACHOKE", .evolution_name = "MACHAMP", .species_index = SI_MACHOKE, .evolution_index = SI_MACHAMP},
    [SI_GRAVELER] = {.species_name = "GRAVELER", .evolution_name = "GOLEM", .species_index = SI_GRAVELER, .evolution_index = SI_GOLEM},
    [SI_HAUNTER] = {.species_name = "HAUNTER", .evolution_name = "GENGAR", .species_index = SI_HAUNTER, .evolution_index = SI_GENGAR}};

static const struct pkmn_evolution_pair_data pkmn_evolution_pairs_gen2[MAX_SPECIES_INDEX + 1] = {
    [KADABRA] = { .species_name = "KADABRA", .evolution_name = "ALAKAZAM", .species_index = KADABRA, .evolution_index = ALAKAZAM},
    [MACHOKE] = {.species_name = "MACHOKE", .evolution_name = "MACHAMP", .species_index = MACHOKE, .evolution_index = MACHAMP},
    [GRAVELER] = {.species_name = "GRAVELER", .evolution_name = "GOLEM", .species_index = GRAVELER, .evolution_index = GOLEM},
    [HAUNTER] = {.species_name = "HAUNTER", .evolution_name = "GENGAR", .species_index = HAUNTER, .evolution_index = GENGAR},
    [SCYTHER] = {.species_name = "SCYTHER", .evolution_name = "SCIZOR", .species_index = SCYTHER, .evolution_index = SCIZOR, .evolution_item = METAL_COAT},
    [POLIWHIRL] = {.species_name = "POLIWHIRL", .evolution_name = "POLITOED", .species_index = POLIWHIRL, .evolution_index = POLITOED, .evolution_item = KING_ROCK},
    [SLOWPOKE] = {.species_name = "SLOWPOKE", .evolution_name = "SLOWKING", .species_index = SLOWPOKE, .evolution_index = SLOWKING, .evolution_item = KING_ROCK},
    [ONIX] = {.species_name = "ONIX", .evolution_name = "STEELIX", .species_index = ONIX, .evolution_index = STEELIX, .evolution_item = METAL_COAT},
    [PORYGON] = {.species_name = "PORYGON", .evolution_name = "PORYGON2", .species_index = PORYGON, .evolution_index = PORYGON2, .evolution_item = UPGRADE},
    [SEADRA] = {.species_name = "SEADRA", .evolution_name = "KINGDRA", .species_index = SEADRA, .evolution_index = KINGDRA, .evolution_item = DRAGON_SCALE}};

static bool disable_random_DVs_on_trade = false;
static bool item_required_evolutions = true;

int error_handler(enum pksav_error error, const char *message);
void swap_party_pkmn_at_indices(struct pksav_gen2_save *pkmn_save, uint8_t pkmn_index1, uint8_t pkmn_index2); // TODO: Update for cross-generation
void swap_pkmn_at_index_between_saves(PokemonSave *player1_save, PokemonSave *player2_save, uint8_t pkmn_party_index1, uint8_t pkmn_party_index2);
void swap_pkmn_at_index_between_saves_cross_gen(PokemonSave *player1_save, PokemonSave *player2_save, uint8_t pkmn_party_index1, uint8_t pkmn_party_index2);
void create_trainer(PokemonSave *pkmn_save, struct TrainerInfo *trainer);
void update_seen_owned_pkmn(PokemonSave *pkmn_save, uint8_t pkmn_party_index);
enum eligible_evolution_status check_trade_evolution_gen1(PokemonSave *pkmn_save, uint8_t pkmn_party_index);
enum eligible_evolution_status check_trade_evolution_gen2(PokemonSave *pkmn_save, uint8_t pkmn_party_index);
void evolve_party_pokemon_at_index(PokemonSave *pkmn_save, uint8_t pkmn_party_index);
void generate_random_number_step(void);
void update_pkmn_DVs(PokemonSave *pkmn_save, uint8_t pkmn_party_index);
bool get_is_random_DVs_disabled(void);
void set_is_random_DVs_disabled(bool is_disabled);
bool get_is_item_required(void);
void set_is_item_required(bool is_required);
void generate_rand_num_step(SaveGenerationType save_generation_type);
enum eligible_trade_status check_trade_eligibility(struct TrainerInfo *trainer, uint8_t pkmn_party_index);

#endif /* PKSAVHELPER_H */
