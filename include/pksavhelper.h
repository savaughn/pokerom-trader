#ifndef PKSAVHELPER_H
#define PKSAVHELPER_H

#include <pksav.h>
#include "common.h"

//Generation 1 Pokémon that evolve through trade
#define KADABRA (uint8_t)38
#define ALAKAZAM (uint8_t)149
#define MACHOKE (uint8_t)41
#define MACHAMP (uint8_t)126
#define GRAVELER (uint8_t)39
#define GOLEM (uint8_t)49
#define HAUNTER (uint8_t)147
#define GENGAR (uint8_t)14

// Generation 2 Pokémon that evolve through trade with specific items
#define SCYTHER (uint8_t)123
#define SCIZOR (uint8_t)212
#define POLIWHIRL (uint8_t)61
#define POLITOED (uint8_t)186
#define SLOWPOKE (uint8_t)79
#define SLOWKING (uint8_t)199
#define ONIX (uint8_t)95
#define STEELIX (uint8_t)208
#define PORYGON (uint8_t)137
#define PORYGON2 (uint8_t)233
#define SEADRA (uint8_t)117
#define KINGDRA (uint8_t)230

// Items required for evolution
#define METAL_COAT 143   // 0x8F
#define KING_ROCK 82     // 0x52
#define DRAGON_SCALE 151 // 0x97
#define UPGRADE 172      // 0xAC

typedef struct {
    uint8_t species;
    int item;
} EvolutionConditionGen2;

// Pokémon Base Stats
struct pkmn_base_stats
{
    uint8_t max_hp;
    uint8_t atk;
    uint8_t def;
    uint8_t spd;
    uint8_t spcl;
    uint8_t spatk;
    uint8_t spdef;
    uint8_t catch_rate;
    uint8_t types[2];
};

// Pokémon Base Stats
static const struct pkmn_base_stats pkmn_base_stats[PORYGON2 + 1] = {
    [ALAKAZAM] = {.max_hp = 55, .atk = 50, .def = 45, .spd = 120, .spcl = 135, .catch_rate = 50, .types = {PKSAV_GEN1_TYPE_PSYCHIC, PKSAV_GEN1_TYPE_PSYCHIC}},
    [MACHAMP] = {.max_hp = 90, .atk = 130, .def = 80, .spd = 55, .spcl = 65, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_FIGHTING, PKSAV_GEN1_TYPE_FIGHTING}},
    [GOLEM] = {.max_hp = 80, .atk = 110, .def = 130, .spd = 45, .spcl = 55, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_ROCK, PKSAV_GEN1_TYPE_GROUND}},
    [GENGAR] = {.max_hp = 60, .atk = 65, .def = 60, .spd = 110, .spcl = 130, .catch_rate = 45, .types = {PKSAV_GEN1_TYPE_GHOST, PKSAV_GEN1_TYPE_POISON}},
    [SCIZOR] = {.max_hp = 70, .atk = 130, .def = 100, .spd = 55, .spatk = 80, .spdef = 65},
    [STEELIX] = {.max_hp = 75, .atk = 85, .def = 200, .spd = 30, .spatk = 65, .spdef = 55},
    [POLITOED] = {.max_hp = 90, .atk = 75, .def = 75, .spd = 70, .spatk = 100, .spdef = 90},
    [SLOWKING] = {.max_hp = 95, .atk = 75, .def = 80, .spd = 30, .spatk = 110, .spdef = 100},
    [PORYGON2] = {.max_hp = 85, .atk = 80, .def = 90, .spd = 60, .spatk = 95, .spdef = 105},
    [KINGDRA] = {.max_hp = 75, .atk = 95, .def = 95, .spd = 85, .spatk = 95, .spdef = 95}};

struct pkmn_evolution_pair_data
{
    char species_name[11];
    char evolution_name[11];
    uint8_t species_index;
    uint8_t evolution_index;
};

// Pokémon Evolution Pair Lookup Table
static const struct pkmn_evolution_pair_data pkmn_evolution_pairs[HAUNTER + 1] = {
    [KADABRA] = {
        .species_name = "KADABRA",
        .evolution_name = "ALAKAZAM",
        .species_index = KADABRA,
        .evolution_index = ALAKAZAM},
    [MACHOKE] = {
        .species_name = "MACHOKE",
        .evolution_name = "MACHAMP",
        .species_index = MACHOKE,
        .evolution_index = MACHAMP},
    [GRAVELER] = {
        .species_name = "GRAVELER",
        .evolution_name = "GOLEM",
        .species_index = GRAVELER,
        .evolution_index = GOLEM},
    [HAUNTER] = {
        .species_name = "HAUNTER",
        .evolution_name = "GENGAR",
        .species_index = HAUNTER,
        .evolution_index = GENGAR},
    [SCYTHER] = {
        .species_name = "SCYTHER",
        .evolution_name = "SCIZOR",
        .species_index = SCYTHER,
        .evolution_index = SCIZOR},
    [POLIWHIRL] = {
        .species_name = "POLIWHIRL",
        .evolution_name = "POLITOED",
        .species_index = POLIWHIRL,
        .evolution_index = POLITOED},
    [SLOWPOKE] = {
        .species_name = "SLOWPOKE",
        .evolution_name = "SLOWKING",
        .species_index = SLOWPOKE,
        .evolution_index = SLOWKING},
    [ONIX] = {
        .species_name = "ONIX",
        .evolution_name = "STEELIX",
        .species_index = ONIX,
        .evolution_index = STEELIX},
    [PORYGON] = {
        .species_name = "PORYGON",
        .evolution_name = "PORYGON2",
        .species_index = PORYGON,
        .evolution_index = PORYGON2},
    [SEADRA] = {
        .species_name = "SEADRA",
        .evolution_name = "KINGDRA",
        .species_index = SEADRA,
        .evolution_index = KINGDRA}
};

static bool disable_random_DVs_on_trade = false;

int error_handler(enum pksav_error error, const char *message);
void swap_party_pkmn_at_indices(struct pksav_gen2_save *pkmn_save, uint8_t pkmn_index1, uint8_t pkmn_index2); // TODO: Update for cross-generation
void swap_pkmn_at_index_between_saves(PokemonSave *player1_save, PokemonSave *player2_save, int selected_index1, int selected_index2);
void create_trainer(PokemonSave *pkmn_save, struct TrainerInfo *trainer);
void update_seen_owned_pkmn(PokemonSave *pkmn_save, uint8_t pkmn_party_index);
void create_trainer_name_str(const struct TrainerInfo *trainer, char *trainer_name, bool show_gender);
void create_trainer_id_str(const struct TrainerInfo *trainer, char *trainer_id);
int check_trade_evolution_gen1(PokemonSave *pkmn_save, uint8_t pkmn_party_index);
int check_trade_evolution_gen2(PokemonSave *pkmn_save, uint8_t pkmn_party_index);
void evolve_party_pokemon_at_index(PokemonSave *pkmn_save, uint8_t pkmn_party_index);
void generate_random_number_step(void);
void update_pkmn_DVs(PokemonSave *pkmn_save, uint8_t pkmn_party_index);
bool get_is_random_DVs_disabled(void);
void set_is_random_DVs_disabled(bool is_disabled);
void generate_rand_num_step(SaveGenerationType save_generation_type);

#endif /* PKSAVHELPER_H */
