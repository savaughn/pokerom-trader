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
#define METAL_COAT 143      // 0x8F
#define KING_ROCK 82        // 0x52
#define DRAGON_SCALE 151    // 0x97
#define UPGRADE 172         // 0xAC 

typedef struct {
    uint8_t species;
    int item;
} EvolutionConditionGen2;

// Generation 1 Pokémon data
static const struct pksav_gen1_party_pokemon alakazam_base_stats = {
    .party_data = {
        .max_hp = 55,
        .atk = 50,
        .def = 45,
        .spd = 120,
        .spcl = 135,
    },
    .pc_data = {
        .catch_rate = 50,
        .types = {PKSAV_GEN1_TYPE_PSYCHIC, PKSAV_GEN1_TYPE_PSYCHIC}
    }
};

static const struct pksav_gen1_party_pokemon machamp_base_stats = {
    .party_data = {
        .max_hp = 90,
        .atk = 130,
        .def = 80,
        .spd = 55,
        .spcl = 65,
    },
    .pc_data = {
        .catch_rate = 45,
        .types = {PKSAV_GEN1_TYPE_FIGHTING, PKSAV_GEN1_TYPE_FIGHTING}
    }
};

static const struct pksav_gen1_party_pokemon golem_base_stats = {
    .party_data = {
        .max_hp = 80,
        .atk = 110,
        .def = 130,
        .spd = 45,
        .spcl = 55,
    },
    .pc_data = {
        .catch_rate = 45,
        .types = {PKSAV_GEN1_TYPE_ROCK, PKSAV_GEN1_TYPE_GROUND}
    }
};

static const struct pksav_gen1_party_pokemon gengar_base_stats = {
    .party_data = {
        .max_hp = 60,
        .atk = 65,
        .def = 60,
        .spd = 110,
        .spcl = 130,
    },
    .pc_data = {
        .catch_rate = 45,
        .types = {PKSAV_GEN1_TYPE_GHOST, PKSAV_GEN1_TYPE_POISON}
    }
};

// Generation 2 Pokémon data
static const struct pksav_gen2_party_pokemon scizor = {
    .party_data = {
        .max_hp = 70,
        .atk = 130,
        .def = 100,
        .spd = 55,
        .spatk = 80,
        .spdef = 65,
        .current_hp = 70,
        .condition = PKSAV_CONDITION_NONE,
    }
};

static const struct pksav_gen2_party_pokemon steelix = {
    .party_data = {
        .max_hp = 75,
        .atk = 85,
        .def = 200,
        .spd = 30,
        .spatk = 65,
        .spdef = 55,
        .current_hp = 75,
        .condition = PKSAV_CONDITION_NONE,
    }
};

static const struct pksav_gen2_party_pokemon politoed = {
    .party_data = {
        .max_hp = 90,
        .atk = 75,
        .def = 75,
        .spd = 70,
        .spatk = 100,
        .spdef = 90,
        .current_hp = 90,
        .condition = PKSAV_CONDITION_NONE,
    }
};

static const struct pksav_gen2_party_pokemon slowking = {
    .party_data = {
        .max_hp = 95,
        .atk = 75,
        .def = 80,
        .spd = 30,
        .spatk = 110,
        .spdef = 100,
        .current_hp = 95,
        .condition = PKSAV_CONDITION_NONE,
    }
};

static const struct pksav_gen2_party_pokemon porygon2 = {
    .party_data = {
        .max_hp = 85,
        .atk = 80,
        .def = 90,
        .spd = 60,
        .spatk = 95,
        .spdef = 105,
        .current_hp = 85,
        .condition = PKSAV_CONDITION_NONE,
    }
};

static const struct pksav_gen2_party_pokemon kingdra = {
    .party_data = {
        .max_hp = 75,
        .atk = 95,
        .def = 95,
        .spd = 85,
        .spatk = 95,
        .spdef = 95,
        .current_hp = 75,
        .condition = PKSAV_CONDITION_NONE,
    }
};

static bool disable_random_DVs_on_trade = false;

int error_handler(enum pksav_error error, const char *message);
void swap_party_pkmn_at_indices(struct pksav_gen2_save *save, int pokemon_index1, int pokemon_index2); // TODO: Update for cross-generation
void swap_pkmn_at_index_between_saves(PokemonSave *player1_save, PokemonSave *player2_save, int selected_index1, int selected_index2);
void create_trainer(PokemonSave *save, struct TrainerInfo *trainer);
void update_seen_owned_pkmn(PokemonSave *save, int pokemon_party_index);
void create_trainer_name_str(const struct TrainerInfo *trainer, char *trainer_name, bool show_gender);
void create_trainer_id_str(const struct TrainerInfo *trainer, char *trainer_id);
int check_trade_evolution_gen1(PokemonSave *pokemon_save, int pokemon_index);
int check_trade_evolution_gen2(PokemonSave *pokemon_save, int pokemon_index);
void evolve_party_pokemon_at_index(PokemonSave *pokemon_save, int pokemon_index);
void generate_random_number_step(void);
void update_pkmn_DVs(PokemonSave *pokemon_save, int pokemon_index);
bool get_is_random_DVs_disabled(void);
void set_is_random_DVs_disabled(bool is_disabled);
void generate_rand_num_step(SaveGenerationType save_generation_type);

#endif /* PKSAVHELPER_H */
