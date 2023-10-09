#ifndef COMMON_H
#define COMMON_H

#include <pksav.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

// Footer Buttons
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 75
#define BACK_BUTTON_X 100
#define BACK_BUTTON_Y SCREEN_HEIGHT - 50
#define NEXT_BUTTON_X SCREEN_WIDTH - 200
#define NEXT_BUTTON_Y SCREEN_HEIGHT - 50

#define SHOW_BILLS_PC 0

// File Edit Screen text input box
#define MAX_INPUT_CHARS 1001

//Generation 1 Pokémon that evolve through trade
#define KADABRA 64
#define MACHOKE 67
#define GRAVELER 75
#define HAUNTER 93

// Bitmask for Generation 1 Pokémon that evolve through trade
#define GEN1_BITMASK ((1ULL << KADABRA) | (1ULL << MACHOKE) | (1ULL << GRAVELER) | (1ULL << HAUNTER))

// Generation 2 Pokémon that evolve through trade with specific items
#define SCIZOR 212
#define POLITOED 186
#define SLOWKING 199
#define STEELIX 208
#define PORYGON2 233
#define KINGDRA 230

// Items required for evolution
#define METAL_COAT 69
#define KING_ROCK 221
#define DRAGON_SCALE 88
#define UP_GRADE 88
#define DRAGON_SCALE 108

typedef struct {
    int species;
    int item;
} EvolutionCondition;

// Bitmask for Generation 2 Pokémon that evolve through trade with specific items
#define GEN2_BITMASK ((1ULL << SCIZOR) | (1ULL << POLITOED) | (1ULL << SLOWKING) | (1ULL << STEELIX) | (1ULL << PORYGON2) | (1ULL << KINGDRA))

typedef int (*Error_Handler)(enum pksav_error, const char *);

enum single_player_menu_types {
    SINGLE_PLAYER_MENU_TYPE_EVOLVE,
    SINGLE_PLAYER_MENU_TYPE_BILLS_PC,
    SINGLE_PLAYER_MENU_TYPE_EXIT
};

union PokemonPartyData {
    struct pksav_gen1_pokemon_party gen1_pokemon_party;
    struct pksav_gen2_pokemon_party gen2_pokemon_party;
};

typedef enum
{
    EBadge_Region_Johto,
    EBadge_Region_Kanto
} BadgeRegionGen2;

struct TrainerSelection
{
    int trainer_id;
    int pokemon_index;
    int trainer_index;
};

typedef enum {
    SCREEN_FILE_SELECT,
    SCREEN_TRADE,
    SCREEN_MAIN_MENU,
    SCREEN_SETTINGS,
    SCREEN_FILE_EDIT,
    SCREEN_ABOUT,
    SCREEN_BILLS_PC_FILE_SELECT,
    SCREEN_BILLS_PC,
    SCREEN_EVOLVE_FILE_SELECT,
    SCREEN_EVOLVE
} GameScreen;

struct SaveFileData {
    char *saveDir[MAX_INPUT_CHARS];
    char* saves_file_path[100];
    int numSaves;
};

typedef enum
{
    SAVE_GENERATION_NONE,
    SAVE_GENERATION_1,
    SAVE_GENERATION_2
} SaveGenerationType;

typedef union
{
    struct pksav_gen1_save gen1_save;
    struct pksav_gen2_save gen2_save;
} SaveGeneration;

typedef struct {
    SaveGenerationType save_generation_type;
    SaveGeneration save;
} PokemonSave;

struct TrainerInfo
{
    char trainer_name[8];
    uint16_t trainer_id;
    uint8_t trainer_gender;
    uint8_t trainer_badges[2];
    union PokemonPartyData pokemon_party;
    SaveGenerationType trainer_generation;
};

#endif // COMMON_H
