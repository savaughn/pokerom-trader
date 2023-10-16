#ifndef COMMON_H
#define COMMON_H

#include <pksav.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// Footer Buttons
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 75
#define BACK_BUTTON_X 100
#define BACK_BUTTON_Y SCREEN_HEIGHT - 50
#define NEXT_BUTTON_X SCREEN_WIDTH - 200
#define NEXT_BUTTON_Y SCREEN_HEIGHT - 50

#define SHOW_BILLS_PC 0
#define MAX_FILE_PATH_CHAR 1001
#define MAX_FILE_PATH_COUNT 100

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
    int pkmn_party_index;
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
    SCREEN_EVOLVE,
    SCREEN_LEGAL
} GameScreen;

struct SaveFileData {
    // current saves file directory string
    char save_dir[MAX_FILE_PATH_CHAR];
    // array of saves file paths in the current saves file directory
    char *saves_file_path[MAX_FILE_PATH_COUNT];
    int num_saves;
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
