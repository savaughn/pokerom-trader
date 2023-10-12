#ifndef PKSAVHELPER_H
#define PKSAVHELPER_H

#include <pksav.h>
#include "common.h"

//Generation 1 Pokémon that evolve through trade
#define KADABRA 38
#define ALAKAZAM 149
#define MACHOKE 41
#define MACHAMP 126
#define GRAVELER 39
#define GOLEM 49
#define HAUNTER 147
#define GENGAR 14

// Generation 2 Pokémon that evolve through trade with specific items
#define SCYTHER 123
#define SCIZOR 212
#define POLIWHIRL 61
#define POLITOED 186
#define SLOWPOKE 79
#define SLOWKING 199
#define ONIX 95
#define STEELIX 208
#define PORYGON 137
#define PORYGON2 233
#define SEADRA 117
#define KINGDRA 230

// Items required for evolution
#define METAL_COAT 143      // 0x8F
#define KING_ROCK 82        // 0x52
#define DRAGON_SCALE 151    // 0x97
#define UPGRADE 172         // 0xAC 

typedef struct {
    int species;
    int item;
} EvolutionCondition;

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

int error_handler(enum pksav_error error, const char *message);
void swapPartyPokemonAtIndices(struct pksav_gen2_save *save, int pokemon_index1, int pokemon_index2); // TODO: Update for cross-generation
void swapPokemonAtIndexBetweenSaves(PokemonSave *player1_save, PokemonSave *player2_save, int selected_index1, int selected_index2);
void create_trainer(PokemonSave *save, struct TrainerInfo *trainer);
void updateSeenOwnedPokemon(PokemonSave *save, int pokemon_party_index);
void createTrainerNameStr(struct TrainerInfo *trainer, char *trainer_name, bool showGender);
void createTrainerIdStr(struct TrainerInfo *trainer, char *trainer_id);
int check_trade_evolution_gen1(PokemonSave *pokemon_save, int pokemon_index);
int check_trade_evolution_gen2(PokemonSave *pokemon_save, int pokemon_index);
void evolve_party_pokemon_at_index(PokemonSave *pokemon_save, int pokemon_index);
void generateRandomNumberStep(void);
#endif /* PKSAVHELPER_H */
