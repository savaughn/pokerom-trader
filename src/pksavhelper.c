#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "pksavhelper.h"

int error_handler(enum pksav_error error, const char *message)
{
    printf("%s\n", message);
    exit(1);
}

void updateSeenOwnedPokemon(PokemonSave *pokemon_save, int pokemon_party_index)
{
    enum pksav_error err;
    if (pokemon_save->save_generation_type == SAVE_GENERATION_1)
    {
        uint8_t pokemon_species = pokemon_save->save.gen1_save.pokemon_storage.p_party->species[pokemon_party_index];
        err = pksav_set_pokedex_bit(pokemon_save->save.gen1_save.pokedex_lists.p_seen, pokemon_species, true);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error setting seen pokedex bit");
        }

        err = pksav_set_pokedex_bit(pokemon_save->save.gen1_save.pokedex_lists.p_owned, pokemon_species, true);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error setting owned pokedex bit");
        }
    }
    else
    {
        uint8_t pokemon_species = pokemon_save->save.gen2_save.pokemon_storage.p_party->species[pokemon_party_index];
        err = pksav_set_pokedex_bit(pokemon_save->save.gen2_save.pokedex_lists.p_seen, pokemon_species, true);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error setting seen pokedex bit");
        }

        err = pksav_set_pokedex_bit(pokemon_save->save.gen2_save.pokedex_lists.p_owned, pokemon_species, true);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error setting owned pokedex bit");
        }
    }
}

enum pksav_error detectSaveFileGeneration(const char *path, SaveGenerationType *save_generation_type)
{
    enum pksav_error err = PKSAV_ERROR_NONE;

    enum pksav_gen1_save_type gen1_save_type = PKSAV_GEN1_SAVE_TYPE_NONE;
    enum pksav_gen2_save_type gen2_save_type = PKSAV_GEN2_SAVE_TYPE_NONE;
    err = pksav_gen1_get_file_save_type(path, &gen1_save_type);

    if (gen1_save_type == PKSAV_GEN1_SAVE_TYPE_NONE)
    {
        pksav_gen2_get_file_save_type(path, &gen2_save_type);
        *save_generation_type = SAVE_GENERATION_2;
    }
    else
    {
        *save_generation_type = SAVE_GENERATION_1;
    }

    return err;
}

PokemonSave loadSaveFromFile(const char *path)
{
    enum pksav_error err = PKSAV_ERROR_NONE;
    PokemonSave pokemon_save;
    SaveGenerationType save_generation_type = SAVE_GENERATION_NONE;

    err = detectSaveFileGeneration(path, &save_generation_type);
    if (err != PKSAV_ERROR_NONE)
    {
        error_handler(err, "Error detecting save file generation");
    }

    pokemon_save.save_generation_type = save_generation_type;

    switch (save_generation_type)
    {
    case SAVE_GENERATION_1:
    {
        enum pksav_gen1_save_type gen1_save_type;
        err = pksav_gen1_get_file_save_type(path, &gen1_save_type);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error getting save type");
        }
        struct pksav_gen1_save save;
        err = pksav_gen1_load_save_from_file(path, &save);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error loading save");
        }
        pokemon_save.save.gen1_save = save;
        break;
    }
    case SAVE_GENERATION_2:
    {
        enum pksav_gen2_save_type gen2_save_type;
        err = pksav_gen2_get_file_save_type(path, &gen2_save_type);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error getting save type");
        }
        struct pksav_gen2_save save;
        err = pksav_gen2_load_save_from_file(path, &save);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error loading save");
        }
        pokemon_save.save.gen2_save = save;
        break;
    }
    }
    return pokemon_save;
}
void printTrainerBadges(struct pksav_gen2_save *save)
{
    enum pksav_gen2_johto_badge_mask johto_badges = *save->trainer_info.p_johto_badges;
    enum pksav_gen2_kanto_badge_mask kanto_badges = *save->trainer_info.p_kanto_badges;
    printf("Johto badges: ");
    if (johto_badges & PKSAV_GEN2_JOHTO_ZEPHYR_BADGE)
    {
        printf("Zephyr ");
    }

    if (johto_badges & PKSAV_GEN2_JOHTO_HIVE_BADGE)
    {
        printf("Hive ");
    }

    if (johto_badges & PKSAV_GEN2_JOHTO_PLAIN_BADGE)
    {
        printf("Plain ");
    }

    if (johto_badges & PKSAV_GEN2_JOHTO_FOG_BADGE)
    {
        printf("Fog ");
    }

    if (johto_badges & PKSAV_GEN2_JOHTO_STORM_BADGE)
    {
        printf("Storm ");
    }

    if (johto_badges & PKSAV_GEN2_JOHTO_MINERAL_BADGE)
    {
        printf("Mineral ");
    }

    if (johto_badges & PKSAV_GEN2_JOHTO_GLACIER_BADGE)
    {
        printf("Glacier ");
    }

    if (johto_badges & PKSAV_GEN2_JOHTO_RISING_BADGE)
    {
        printf("Rising ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_BOULDER_BADGE)
    {
        printf("Boulder ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_CASCADE_BADGE)
    {
        printf("Cascade ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_THUNDER_BADGE)
    {
        printf("Thunder ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_RAINBOW_BADGE)
    {
        printf("Rainbow ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_SOUL_BADGE)
    {
        printf("Soul ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_MARSH_BADGE)
    {
        printf("Marsh ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_VOLCANO_BADGE)
    {
        printf("Volcano ");
    }

    if (kanto_badges & PKSAV_GEN2_KANTO_EARTH_BADGE)
    {
        printf("Earth ");
    }

    printf("\n");
}
void printTrainerData(struct pksav_gen2_save *save)
{
    // print trainer name
    char trainer_name[8];
    pksav_gen2_import_text(save->trainer_info.p_name, trainer_name, 7);
    printf("Trainer name: %s\n", trainer_name);

    // print trainer id
    uint16_t trainer_id = pksav_bigendian16(*save->trainer_info.p_id);
    printf("Trainer ID: %u\n", trainer_id);

    // Gender is only in Crystal and later games
    if (save->save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL)
    {
        // print trainer gender
        uint8_t trainer_gender = *save->trainer_info.p_gender;
        printf("Trainer Gender: %s\n", trainer_gender ? "F" : "M");
    }

    // print trainer badges
    printTrainerBadges(save);
}
// void changePartyPokemonNicknameAtIndex(pksav_gen2_save_t *save, int pokemon_index, char *new_name)
// {
//     char pokemon_name[11];
//     pksav_text_to_gen2(new_name, save->pokemon_party->nicknames[pokemon_index], 10);
//     pksav_text_from_gen2(save->pokemon_party->nicknames[pokemon_index], pokemon_name, 10);
//     printf("Pokemon new name: %s\n", pokemon_name);
// }
void saveToFile(PokemonSave *pokemon_save, char *path)
{
    enum pksav_error err = PKSAV_ERROR_NONE;
    if (pokemon_save->save_generation_type == SAVE_GENERATION_1)
    {
        err = pksav_gen1_save_save(path, &pokemon_save->save.gen1_save);
    }
    else
    {
        err = pksav_gen2_save_save(path, &pokemon_save->save.gen2_save);
    }

    if (err != PKSAV_ERROR_NONE)
    {
        error_handler(err, "Error saving save");
    }
    else
    {
        printf("Saved to %s\n", path);
    }
}

void printPartyPokemonAtIndex(struct pksav_gen2_save *save, int pokemon_index)
{
    // print nickname
    char pokemon_nickname[11];
    pksav_gen2_import_text(save->pokemon_storage.p_party->nicknames[pokemon_index], pokemon_nickname, 10);
    printf("%d. %s\n", pokemon_index + 1, pokemon_nickname);

    // // print species
    // uint8_t species = save->pokemon_storage.p_party->species[pokemon_index];
    // printf("%d. %d\n", pokemon_index + 1, species);
}
void printParty(struct pksav_gen2_save *save)
{
    printf("Party:\n");
    for (int i = 0; i < 6; i++)
    {
        printPartyPokemonAtIndex(save, i);
    }
}
void swapPartyPokemonAtIndices(struct pksav_gen2_save *save, int pokemon_index1, int pokemon_index2)
{
    // swap nickname
    char tmp_nickname1[11];
    char tmp_nickname2[11];
    pksav_gen2_import_text(save->pokemon_storage.p_party->nicknames[pokemon_index1], tmp_nickname1, 10);
    pksav_gen2_import_text(save->pokemon_storage.p_party->nicknames[pokemon_index2], tmp_nickname2, 10);
    pksav_gen2_export_text(tmp_nickname2, save->pokemon_storage.p_party->nicknames[pokemon_index1], 10);
    pksav_gen2_export_text(tmp_nickname1, save->pokemon_storage.p_party->nicknames[pokemon_index2], 10);
    save->pokemon_storage.p_party->nicknames[pokemon_index1][strlen(tmp_nickname2)] = 0x50;
    save->pokemon_storage.p_party->nicknames[pokemon_index2][strlen(tmp_nickname1)] = 0x50;

    // swap party data
    struct pksav_gen2_party_pokemon tmp_pokemon = save->pokemon_storage.p_party->party[pokemon_index1];
    save->pokemon_storage.p_party->party[pokemon_index1] = save->pokemon_storage.p_party->party[pokemon_index2];
    save->pokemon_storage.p_party->party[pokemon_index2] = tmp_pokemon;

    // swap species
    uint8_t tmp_species = save->pokemon_storage.p_party->species[pokemon_index1];
    save->pokemon_storage.p_party->species[pokemon_index1] = save->pokemon_storage.p_party->species[pokemon_index2];
    save->pokemon_storage.p_party->species[pokemon_index2] = tmp_species;

    // swap otnames
    char tmp_otname1[8];
    char tmp_otname2[8];
    pksav_gen2_import_text(save->pokemon_storage.p_party->otnames[pokemon_index1], tmp_otname1, 7);
    pksav_gen2_import_text(save->pokemon_storage.p_party->otnames[pokemon_index2], tmp_otname2, 7);
    pksav_gen2_export_text(tmp_otname2, save->pokemon_storage.p_party->otnames[pokemon_index1], 7);
    pksav_gen2_export_text(tmp_otname1, save->pokemon_storage.p_party->otnames[pokemon_index2], 7);
    save->pokemon_storage.p_party->otnames[pokemon_index1][strlen(tmp_otname2)] = 0x50;
    save->pokemon_storage.p_party->otnames[pokemon_index2][strlen(tmp_otname1)] = 0x50;
}

void swapPokemonAtIndexBetweenSaves(PokemonSave *player1_save, PokemonSave *player2_save, int selected_index1, int selected_index2)
{
    // swap nickname
    char tmp_nickname1[11];
    char tmp_nickname2[11];
    // TODO: Update for cross-gen
    if (player1_save->save_generation_type == SAVE_GENERATION_1)
    {
        pksav_gen1_import_text(player1_save->save.gen1_save.pokemon_storage.p_party->nicknames[selected_index1], tmp_nickname1, 10);
        pksav_gen1_import_text(player2_save->save.gen1_save.pokemon_storage.p_party->nicknames[selected_index2], tmp_nickname2, 10);
        pksav_gen1_export_text(tmp_nickname2, player1_save->save.gen1_save.pokemon_storage.p_party->nicknames[selected_index1], 10);
        pksav_gen1_export_text(tmp_nickname1, player2_save->save.gen1_save.pokemon_storage.p_party->nicknames[selected_index2], 10);
        player1_save->save.gen1_save.pokemon_storage.p_party->nicknames[selected_index1][strlen(tmp_nickname2)] = 0x50;
        player2_save->save.gen1_save.pokemon_storage.p_party->nicknames[selected_index2][strlen(tmp_nickname1)] = 0x50;
    }
    else
    {
        pksav_gen2_import_text(player1_save->save.gen2_save.pokemon_storage.p_party->nicknames[selected_index1], tmp_nickname1, 10);
        pksav_gen2_import_text(player2_save->save.gen2_save.pokemon_storage.p_party->nicknames[selected_index2], tmp_nickname2, 10);
        pksav_gen2_export_text(tmp_nickname2, player1_save->save.gen2_save.pokemon_storage.p_party->nicknames[selected_index1], 10);
        pksav_gen2_export_text(tmp_nickname1, player2_save->save.gen2_save.pokemon_storage.p_party->nicknames[selected_index2], 10);
        player1_save->save.gen2_save.pokemon_storage.p_party->nicknames[selected_index1][strlen(tmp_nickname2)] = 0x50;
        player2_save->save.gen2_save.pokemon_storage.p_party->nicknames[selected_index2][strlen(tmp_nickname1)] = 0x50;
    }

    // swap party
    if (player1_save->save_generation_type == SAVE_GENERATION_1)
    {
        struct pksav_gen1_party_pokemon tmp_pokemon = player1_save->save.gen1_save.pokemon_storage.p_party->party[selected_index1];
        player1_save->save.gen1_save.pokemon_storage.p_party->party[selected_index1] = player2_save->save.gen1_save.pokemon_storage.p_party->party[selected_index2];
        player2_save->save.gen1_save.pokemon_storage.p_party->party[selected_index2] = tmp_pokemon;
    }
    else
    {
        struct pksav_gen2_party_pokemon tmp_pokemon = player1_save->save.gen2_save.pokemon_storage.p_party->party[selected_index1];
        player1_save->save.gen2_save.pokemon_storage.p_party->party[selected_index1] = player2_save->save.gen2_save.pokemon_storage.p_party->party[selected_index2];
        player2_save->save.gen2_save.pokemon_storage.p_party->party[selected_index2] = tmp_pokemon;
    }

    // swap species
    if (player1_save->save_generation_type == SAVE_GENERATION_1)
    {
        uint8_t tmp_species = player1_save->save.gen1_save.pokemon_storage.p_party->species[selected_index1];
        player1_save->save.gen1_save.pokemon_storage.p_party->species[selected_index1] = player2_save->save.gen1_save.pokemon_storage.p_party->species[selected_index2];
        player2_save->save.gen1_save.pokemon_storage.p_party->species[selected_index2] = tmp_species;
    }
    else
    {
        uint8_t tmp_species = player1_save->save.gen2_save.pokemon_storage.p_party->species[selected_index1];
        player1_save->save.gen2_save.pokemon_storage.p_party->species[selected_index1] = player2_save->save.gen2_save.pokemon_storage.p_party->species[selected_index2];
        player2_save->save.gen2_save.pokemon_storage.p_party->species[selected_index2] = tmp_species;
    }

    // swap otnames
    char tmp_otname1[8];
    char tmp_otname2[8];
    if (player1_save->save_generation_type == SAVE_GENERATION_1)
    {
        pksav_gen1_import_text(player1_save->save.gen1_save.pokemon_storage.p_party->otnames[selected_index1], tmp_otname1, 7);
        pksav_gen1_import_text(player2_save->save.gen1_save.pokemon_storage.p_party->otnames[selected_index2], tmp_otname2, 7);
        pksav_gen1_export_text(tmp_otname2, player1_save->save.gen1_save.pokemon_storage.p_party->otnames[selected_index1], 7);
        pksav_gen1_export_text(tmp_otname1, player2_save->save.gen1_save.pokemon_storage.p_party->otnames[selected_index2], 7);
        player1_save->save.gen1_save.pokemon_storage.p_party->otnames[selected_index1][strlen(tmp_otname2)] = 0x50;
        player2_save->save.gen1_save.pokemon_storage.p_party->otnames[selected_index2][strlen(tmp_otname1)] = 0x50;
    }
    else
    {
        pksav_gen2_import_text(player1_save->save.gen2_save.pokemon_storage.p_party->otnames[selected_index1], tmp_otname1, 7);
        pksav_gen2_import_text(player2_save->save.gen2_save.pokemon_storage.p_party->otnames[selected_index2], tmp_otname2, 7);
        pksav_gen2_export_text(tmp_otname2, player1_save->save.gen2_save.pokemon_storage.p_party->otnames[selected_index1], 7);
        pksav_gen2_export_text(tmp_otname1, player2_save->save.gen2_save.pokemon_storage.p_party->otnames[selected_index2], 7);
        player1_save->save.gen2_save.pokemon_storage.p_party->otnames[selected_index1][strlen(tmp_otname2)] = 0x50;
        player2_save->save.gen2_save.pokemon_storage.p_party->otnames[selected_index2][strlen(tmp_otname1)] = 0x50;
    }
}

void create_trainer(PokemonSave *pokemon_save, struct TrainerInfo *trainer)
{
    SaveGenerationType save_generation_type = pokemon_save->save_generation_type;

    switch (save_generation_type)
    {
    case SAVE_GENERATION_1:
    {
        // Trainer name
        char trainer_name[8];
        pksav_gen1_import_text(pokemon_save->save.gen1_save.trainer_info.p_name, trainer_name, 7);

        // Trainer Id
        uint16_t trainer_id = pksav_bigendian16(*pokemon_save->save.gen1_save.trainer_info.p_id);

        // Update the trainer struct
        strcpy(trainer->trainer_name, trainer_name);
        trainer->trainer_id = trainer_id;

        trainer->trainer_badges[0] = *pokemon_save->save.gen1_save.trainer_info.p_badges;

        // Update the trainer's pokemon party
        trainer->pokemon_party.gen1_pokemon_party = *pokemon_save->save.gen1_save.pokemon_storage.p_party;
        trainer->trainer_generation = SAVE_GENERATION_1;
        break;
    }
    case SAVE_GENERATION_2:
    {
        // Trainer name
        char trainer_name[8];
        pksav_gen2_import_text(pokemon_save->save.gen2_save.trainer_info.p_name, trainer_name, 7);

        // Update the trainer struct
        strcpy(trainer->trainer_name, trainer_name);
        trainer->trainer_id = pksav_bigendian16(*pokemon_save->save.gen2_save.trainer_info.p_id);
        trainer->trainer_badges[EBadge_Region_Johto] = *pokemon_save->save.gen2_save.trainer_info.p_johto_badges;
        trainer->trainer_badges[EBadge_Region_Kanto] = *pokemon_save->save.gen2_save.trainer_info.p_kanto_badges;
        // Trainer Gender (Crystal and later games only)
        if (pokemon_save->save.gen2_save.save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL)
        {
            trainer->trainer_gender = *pokemon_save->save.gen2_save.trainer_info.p_gender;
        }

        // Update the trainer's pokemon party
        trainer->pokemon_party.gen2_pokemon_party = *pokemon_save->save.gen2_save.pokemon_storage.p_party;
        trainer->trainer_generation = SAVE_GENERATION_2;
        break;
    }
    }
}

void createTrainerNameStr(struct TrainerInfo *trainer, char *trainer_name, bool showGender)
{
    strcpy(trainer_name, "NAME/");
    strcat(trainer_name, trainer->trainer_name);
    if (trainer->trainer_generation == SAVE_GENERATION_2 && showGender)
    {
        strcat(trainer_name, " ");
        strcat(trainer_name, trainer->trainer_gender == PKSAV_GEN2_GENDER_FEMALE ? "F" : "M");
    }
}

void createTrainerIdStr(struct TrainerInfo *trainer, char *trainer_id)
{
    char id_str[6];
    strcpy(trainer_id, "IDNo ");
    snprintf(id_str, sizeof(id_str), "%u", trainer->trainer_id); // "IDNo %u" loses 3 chars even with enough space?
    strcat(trainer_id, id_str);
}

int check_trade_evolution_gen1(PokemonSave *pokemon_save, int pokemon_index)
{
    uint8_t species = pokemon_save->save.gen1_save.pokemon_storage.p_party->species[pokemon_index];

    EvolutionCondition gen1Evolutions[] = {
        {KADABRA},
        {MACHOKE},
        {GRAVELER},
        {HAUNTER}};

    int i;
    for (i = 0; i < sizeof(gen1Evolutions) / sizeof(gen1Evolutions[0]); i++)
    {
        // Pokemon eligible for trade evolution
        if (gen1Evolutions[i].species == species)
        {
            return 1;
        }

        // No pokemon eligible for trade evolution
        if (i == sizeof(gen1Evolutions) / sizeof(gen1Evolutions[0]))
        {
            return 0;
        }
    }

    return 0;
}

int check_trade_evolution_gen2(PokemonSave *pokemon_save, int pokemon_index)
{
    int species = pokemon_save->save.gen2_save.pokemon_storage.p_party->species[pokemon_index];
    int item = pokemon_save->save.gen2_save.pokemon_storage.p_party->party[pokemon_index].pc_data.held_item;
    EvolutionCondition gen2Evolutions[] = {
        {SCYTHER, METAL_COAT},
        {POLIWHIRL, KING_ROCK},
        {SLOWPOKE, KING_ROCK},
        {ONIX, METAL_COAT},
        {PORYGON, UPGRADE},
        {SEADRA, DRAGON_SCALE}};

    int i;
    for (i = 0; i < sizeof(gen2Evolutions) / sizeof(gen2Evolutions[0]); i++)
    {
        // Pokemon eligible for trade evolution but missing item
        if (gen2Evolutions[i].species == species && gen2Evolutions[i].item != item)
        {
            return 2;
        }

        // Pokemon eligible for trade evolution
        if (gen2Evolutions[i].species == species && gen2Evolutions[i].item == item)
        {
            return 1;
        }

        // No pokemon eligible for trade evolution
        if (i == sizeof(gen2Evolutions) / sizeof(gen2Evolutions[0]))
        {
            return 0;
        }
    }

    return 0;
}

// Function to calculate HP based on base stat, IV, Stat Exp, and level
int calculateHP(int baseHP, int dvHP, int statExp, int level)
{
    float hpCalc = (baseHP + dvHP) * 2 + floor(ceil(sqrt(statExp)) / 4);
    hpCalc = hpCalc * level / 100.0;
    hpCalc = floor(hpCalc) + level + 10;

    return (int)hpCalc;
}

// Function to calculate other stats (Attack, Defense, Special, Speed)
// based on base stat, IV, Stat Exp, and level
int calculateStat(int level, int baseStat, int dv, int statExp)
{
    float statCalc = (baseStat + dv) * 2 + floor(ceil(sqrt(statExp)) / 4);
    statCalc = statCalc * level / 100.0;
    statCalc = floor(statCalc) + 5;

    return (int)statCalc;
}
/************************************************************************
 * Simulate the random number generation for Generation 1
 */
uint8_t rDiv = 0;
uint8_t carryBit = 0;
uint8_t addByte = 0;
uint8_t subtractByte = 0;

// Simulate one step of the random number generation process
void generateRandomNumberStep()
{
    // Increment rDiv (simulated as an 8-bit counter)
    rDiv++;

    // Simulate the addition step
    uint16_t sum = rDiv + carryBit + addByte;
    carryBit = (sum > 255) ? 1 : 0;
    addByte = sum & 0xFF;

    // Simulate the subtraction step
    uint16_t difference = rDiv + carryBit - subtractByte;
    carryBit = (difference > 255) ? 1 : 0;
    subtractByte = difference & 0xFF;
}

// Function to get a random byte
uint8_t getRandomByte()
{
    return addByte; // Return the add byte as the generated random number
}

void randomize_gen1_IVs(uint8_t *ivs, size_t num_IVs)
{
    for (int i = 0; i < num_IVs; i++)
    {
        // random int between 0 and 15
        uint8_t random_IV = getRandomByte() & 0xF;
        generateRandomNumberStep();

        // set the IV
        ivs[i] = random_IV;
    }
}
/*************************************************************************/

void evolve_party_pokemon_at_index(PokemonSave *pokemon_save, int pokemon_index)
{
    if (pokemon_save->save_generation_type == SAVE_GENERATION_1)
    {
        int species = pokemon_save->save.gen1_save.pokemon_storage.p_party->species[pokemon_index];
        char pokemon_name[11];
        pksav_gen1_import_text(pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index], pokemon_name, 10);

        switch (species)
        {
        case KADABRA:
            pokemon_save->save.gen1_save.pokemon_storage.p_party->species[pokemon_index] = (uint8_t)ALAKAZAM;
            if (strcmp(pokemon_name, "KADABRA") == 0)
            {
                pksav_gen1_export_text("ALAKAZAM", pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index], 10);
                pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index][strlen("ALAKAZAM")] = 0x50;
            }
            pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].party_data = alakazam.party_data;
            break;
        case MACHOKE:
            pokemon_save->save.gen1_save.pokemon_storage.p_party->species[pokemon_index] = MACHAMP;
            if (strcmp(pokemon_name, "MACHOKE") == 0)
            {
                pksav_gen1_export_text("MACHAMP", pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index], 10);
                pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index][strlen("MACHAMP")] = 0x50;
            }
            pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].party_data = machamp.party_data;
            break;
        case GRAVELER:
            pokemon_save->save.gen1_save.pokemon_storage.p_party->species[pokemon_index] = GOLEM;
            if (strcmp(pokemon_name, "GRAVELER") == 0)
            {
                pksav_gen1_export_text("GOLEM", pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index], 10);
                pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index][strlen("GOLEM")] = 0x50;
            }
            pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].party_data = golem.party_data;
            break;
        case HAUNTER:
            pokemon_save->save.gen1_save.pokemon_storage.p_party->species[pokemon_index] = (uint8_t)GENGAR;
            if (strcmp(pokemon_name, "HAUNTER") == 0)
            {
                pksav_gen1_export_text("GENGAR", pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index], 10);
                pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index][strlen("GENGAR")] = 0x50;
            }

            // level
            uint8_t level = pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.level;

            // Randomize DVs on trade
            uint8_t traded_haunter_rand_dvs[PKSAV_NUM_GB_IVS - 1];
            randomize_gen1_IVs(traded_haunter_rand_dvs, sizeof(traded_haunter_rand_dvs));
            printf("DV_ATTACK: %u\n", traded_haunter_rand_dvs[PKSAV_GB_IV_ATTACK]);
            printf("DV_DEFENSE: %u\n", traded_haunter_rand_dvs[PKSAV_GB_IV_DEFENSE]);
            printf("DV_SPEED: %u\n", traded_haunter_rand_dvs[PKSAV_GB_IV_SPEED]);
            printf("DV_SPECIAL: %u\n", traded_haunter_rand_dvs[PKSAV_GB_IV_SPECIAL]);

            uint8_t traded_haunter_hpiv = (((traded_haunter_rand_dvs[PKSAV_GB_IV_ATTACK] & 0x01) << 3) |
                               ((traded_haunter_rand_dvs[PKSAV_GB_IV_DEFENSE] & 0x01) << 2) |
                               ((traded_haunter_rand_dvs[PKSAV_GB_IV_SPEED] & 0x01) << 1) |
                               (traded_haunter_rand_dvs[PKSAV_GB_IV_SPECIAL] & 0x01));
            printf("DV_HP: %u\n", traded_haunter_rand_dvs[PKSAV_GB_IV_HP]);

            uint16_t ev_hp = pksav_bigendian16(pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.ev_hp);
            uint16_t ev_atk = pksav_bigendian16(pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.ev_atk);
            uint16_t ev_def = pksav_bigendian16(pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.ev_def);
            uint16_t ev_spd = pksav_bigendian16(pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.ev_spd);
            uint16_t ev_spcl = pksav_bigendian16(pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.ev_spcl);
            uint8_t gengar_max_hp = calculateHP(gengar_base.party_data.max_hp, traded_haunter_hpiv, ev_hp, level);

            // Calculate Gengar stats from DVs and haunter stats
            uint8_t gengar_atk = calculateStat(level, gengar_base.party_data.atk, traded_haunter_rand_dvs[PKSAV_GB_IV_ATTACK], ev_atk);
            uint8_t gengar_def = calculateStat(level, gengar_base.party_data.def, traded_haunter_rand_dvs[PKSAV_GB_IV_DEFENSE], ev_def);
            uint8_t gengar_spd = calculateStat(level, gengar_base.party_data.spd, traded_haunter_rand_dvs[PKSAV_GB_IV_SPEED], ev_spd);
            uint8_t gengar_spcl = calculateStat(level, gengar_base.party_data.spcl, traded_haunter_rand_dvs[PKSAV_GB_IV_SPECIAL], ev_spcl);

            break;
        }

        // TODO: Update pokedex seen/owned
    }
    else
    {
        int species = pokemon_save->save.gen2_save.pokemon_storage.p_party->species[pokemon_index];
        char pokemon_name[11];
        pksav_gen2_import_text(pokemon_save->save.gen2_save.pokemon_storage.p_party->nicknames[pokemon_index], pokemon_name, 10);

        switch (species)
        {
        case SCYTHER:
            pokemon_save->save.gen2_save.pokemon_storage.p_party->species[pokemon_index] = SCIZOR;
            if (strcmp(pokemon_name, "SCYTHER") == 0)
            {
                pksav_gen2_export_text("SCIZOR", pokemon_save->save.gen2_save.pokemon_storage.p_party->nicknames[pokemon_index], 10);
                pokemon_save->save.gen2_save.pokemon_storage.p_party->nicknames[pokemon_index][strlen("SCIZOR")] = 0x50;
            }
            pokemon_save->save.gen2_save.pokemon_storage.p_party->party[pokemon_index].party_data = scizor.party_data;
            break;
        case POLIWHIRL:
            pokemon_save->save.gen2_save.pokemon_storage.p_party->species[pokemon_index] = POLITOED;
            if (strcmp(pokemon_name, "POLIWHIRL") == 0)
            {
                pksav_gen2_export_text("POLITOED", pokemon_save->save.gen2_save.pokemon_storage.p_party->nicknames[pokemon_index], 10);
                pokemon_save->save.gen2_save.pokemon_storage.p_party->nicknames[pokemon_index][strlen("POLITOED")] = 0x50;
            }
            pokemon_save->save.gen2_save.pokemon_storage.p_party->party[pokemon_index].party_data = politoed.party_data;
            break;
        case SLOWPOKE:
            pokemon_save->save.gen2_save.pokemon_storage.p_party->species[pokemon_index] = SLOWKING;
            if (strcmp(pokemon_name, "SLOWPOKE") == 0)
            {
                pksav_gen2_export_text("SLOWKING", pokemon_save->save.gen2_save.pokemon_storage.p_party->nicknames[pokemon_index], 10);
                pokemon_save->save.gen2_save.pokemon_storage.p_party->nicknames[pokemon_index][strlen("SLOWKING")] = 0x50;
            }
            pokemon_save->save.gen2_save.pokemon_storage.p_party->party[pokemon_index].party_data = slowking.party_data;
            break;
        case ONIX:
            pokemon_save->save.gen2_save.pokemon_storage.p_party->species[pokemon_index] = STEELIX;
            if (strcmp(pokemon_name, "ONIX") == 0)
            {
                pksav_gen2_export_text("STEELIX", pokemon_save->save.gen2_save.pokemon_storage.p_party->nicknames[pokemon_index], 10);
                pokemon_save->save.gen2_save.pokemon_storage.p_party->nicknames[pokemon_index][strlen("STEELIX")] = 0x50;
            }
            pokemon_save->save.gen2_save.pokemon_storage.p_party->party[pokemon_index].party_data = steelix.party_data;
            break;
        case PORYGON:
            pokemon_save->save.gen2_save.pokemon_storage.p_party->species[pokemon_index] = PORYGON2;
            if (strcmp(pokemon_name, "PORYGON") == 0)
            {
                pksav_gen2_export_text("PORYGON2", pokemon_save->save.gen2_save.pokemon_storage.p_party->nicknames[pokemon_index], 10);
                pokemon_save->save.gen2_save.pokemon_storage.p_party->nicknames[pokemon_index][strlen("PORYGON2")] = 0x50;
            }
            pokemon_save->save.gen2_save.pokemon_storage.p_party->party[pokemon_index].party_data = porygon2.party_data;
            break;
        case SEADRA:
            pokemon_save->save.gen2_save.pokemon_storage.p_party->species[pokemon_index] = KINGDRA;
            if (strcmp(pokemon_name, "SEADRA") == 0)
            {
                pksav_gen2_export_text("KINGDRA", pokemon_save->save.gen2_save.pokemon_storage.p_party->nicknames[pokemon_index], 10);
                pokemon_save->save.gen2_save.pokemon_storage.p_party->nicknames[pokemon_index][strlen("KINGDRA")] = 0x50;
            }
            pokemon_save->save.gen2_save.pokemon_storage.p_party->party[pokemon_index].party_data = kingdra.party_data;
            break;
        }
    }
}

/**
 * NOTES
 *
 *             uint8_t haunter_ivs[PKSAV_NUM_GB_IVS];
            uint16_t haunter_iv_data = pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.iv_data;
            pksav_get_gb_IVs(&haunter_iv_data, haunter_ivs, sizeof(haunter_ivs));

            uint8_t atk = haunter_ivs[PKSAV_GB_IV_ATTACK];
            printf("atk DV: %u\n", atk);

            uint8_t def = haunter_ivs[PKSAV_GB_IV_DEFENSE];
            printf("def DV: %u\n", def);

            uint8_t spd = haunter_ivs[PKSAV_GB_IV_SPEED];
            printf("spd DV: %u\n", spd);

            uint8_t spcl = haunter_ivs[PKSAV_GB_IV_SPECIAL];
            printf("spcl DV: %u\n", spcl);

            uint8_t max_hp = haunter_ivs[PKSAV_GB_IV_HP];
            printf("max_hp DV: %u\n", max_hp);

*/