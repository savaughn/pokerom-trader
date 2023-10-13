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

void update_seen_owned_pkmn(PokemonSave *pokemon_save, int pokemon_party_index)
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

void swap_party_pkmn_at_indices(struct pksav_gen2_save *save, int pokemon_index1, int pokemon_index2)
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

void swap_pkmn_at_index_between_saves(PokemonSave *player1_save, PokemonSave *player2_save, int selected_index1, int selected_index2)
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

    // TODO: Update DVs for both traded pokemon
}

// Extracts the trainer info from the save file and updates the trainer struct
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
    default:
        break;
    }
}

// Concantenate the trainer's name and id into a string for Raylib to draw
void create_trainer_name_str(const struct TrainerInfo *trainer, char *trainer_name, bool show_gender)
{
    strcpy(trainer_name, "NAME/");
    strcat(trainer_name, trainer->trainer_name);
    if (trainer->trainer_generation == SAVE_GENERATION_2 && show_gender)
    {
        strcat(trainer_name, " ");
        strcat(trainer_name, trainer->trainer_gender == PKSAV_GEN2_GENDER_FEMALE ? "F" : "M");
    }
}

// Concantenate the trainer's id into a string for Raylib to draw
void create_trainer_id_str(const struct TrainerInfo *trainer, char *trainer_id)
{
    char id_str[6];
    strcpy(trainer_id, "IDNo ");
    snprintf(id_str, sizeof(id_str), "%u", trainer->trainer_id); // "IDNo %u" loses 3 chars even with enough space?
    strcat(trainer_id, id_str);
}

// Checks if supplied Gen 1 pokemon is eligible for trade evolution returns 1 if true, 0 if false
int check_trade_evolution_gen1(PokemonSave *pokemon_save, int pokemon_index)
{
    // Species index of pokemon being checked
    uint8_t species = pokemon_save->save.gen1_save.pokemon_storage.p_party->species[pokemon_index];

    // Pokemon species eligible for trade evolution in Gen 1
    uint8_t gen1_evolutions[4] = {
        (uint8_t)KADABRA,
        (uint8_t)MACHOKE,
        (uint8_t)GRAVELER,
        (uint8_t)HAUNTER
    };

    int i;
    for (i = 0; i < (int)(sizeof(gen1_evolutions) / sizeof(gen1_evolutions[0])); i++)
    {
        // Pokemon eligible for trade evolution
        if (gen1_evolutions[i] == species)
        {
            return 1;
        }

        // No pokemon eligible for trade evolution
        if (i == (int)(sizeof(gen1_evolutions) / sizeof(gen1_evolutions[0])))
        {
            return 0;
        }
    }

    return 0;
}

// Checks if supplied Gen 2 pokemon is eligible for trade evolution returns 1 if true, 0 if false, or 2 if eligible but required missing item
int check_trade_evolution_gen2(PokemonSave *pokemon_save, int pokemon_index)
{
    // Species index of pokemon being checked
    int species = pokemon_save->save.gen2_save.pokemon_storage.p_party->species[pokemon_index];
    // Item held index by pokemon being checked
    int item = pokemon_save->save.gen2_save.pokemon_storage.p_party->party[pokemon_index].pc_data.held_item;

    // Pokemon species eligible for trade evolution in Gen 2 with required item
    EvolutionConditionGen2 gen2_evolutions[] = {
        {SCYTHER, METAL_COAT},
        {POLIWHIRL, KING_ROCK},
        {SLOWPOKE, KING_ROCK},
        {ONIX, METAL_COAT},
        {PORYGON, UPGRADE},
        {SEADRA, DRAGON_SCALE}};

    int i;
    for (i = 0; i < (int)(sizeof(gen2_evolutions) - 1); i++)
    {
        // Pokemon eligible for trade evolution but missing item
        if (gen2_evolutions[i].species == species && gen2_evolutions[i].item != item)
        {
            return 2;
        }

        // Pokemon eligible for trade evolution
        if (gen2_evolutions[i].species == species && gen2_evolutions[i].item == item)
        {
            return 1;
        }

        // No pokemon eligible for trade evolution
        if (i == sizeof(gen2_evolutions) - 1)
        {
            return 0;
        }
    }

    return 0;
}

// Function to calculate HP based on base stat, IV, Stat Exp, and level
uint8_t calculate_hp(uint8_t level, int base_hp, int dv_hp, int stat_exp)
{
    float hp_calc = (base_hp + dv_hp) * 2 + floor(ceil(sqrt(stat_exp)) / 4);
    hp_calc = hp_calc * level / 100.0;
    hp_calc = floor(hp_calc) + level + 10;

    return (int)hp_calc;
}

// Function to calculate stats (Attack, Defense, Special, Speed)
// based on base stat, IV, Stat Exp, and level
uint8_t calculate_stat(uint8_t level, int base_stat, int dv, int stat_exp)
{
    float stat_calc = (base_stat + dv) * 2 + floor(ceil(sqrt(stat_exp)) / 4);
    stat_calc = stat_calc * level / 100.0;
    stat_calc = floor(stat_calc) + 5;

    return (int)stat_calc;
}
/************************************************************************
 * Simulate the random number generation for Generation 1
 */
uint8_t r_div = 0;
uint8_t carry_bit = 0;
uint8_t add_byte = 0;
uint8_t subtract_byte = 0;

// Simulate one step of the random number generation process
void generate_random_number_step(void)
{
    // Increment r_div (simulated as an 8-bit counter)
    r_div++;

    // Simulate the addition step
    uint16_t sum = r_div + carry_bit + add_byte;
    carry_bit = (sum > 255) ? 1 : 0;
    add_byte = sum & 0xFF;

    // Simulate the subtraction step
    uint16_t difference = r_div + carry_bit - subtract_byte;
    carry_bit = (difference > 255) ? 1 : 0;
    subtract_byte = difference & 0xFF;
}

// Function to get a random byte
uint8_t get_random_byte(void)
{
    return add_byte; // Return the add byte as the generated random number
}

// Simulated hardware registers
uint8_t hRandomAdd = 0;  // hRandomAdd register
uint8_t hRandomSub = 0;  // hRandomSub register

// Gen 2 used hardware registers to generate random numbers
// Perform the random number generation step
void generateRandomNumberStepGen2(void) {
    rDiv++; // Increment rDiv (simulated as an 8-bit counter)

    // Simulate the addition step
    uint16_t sum = rDiv + hRandomAdd;
    hRandomAdd = (uint8_t)sum;

    // Simulate the subtraction step
    uint16_t diff = rDiv - hRandomSub;
    hRandomSub = (uint8_t)diff;
}

// Function to get a random byte
uint8_t getRandomByteGen2(void) {
    return hRandomAdd; // Return the simulated hRandomAdd register
}

void generate_rand_num_step(SaveGenerationType save_generation_type)
{
    if (save_generation_type == SAVE_GENERATION_1)
    {
        generateRandomNumberStep();
    }
    else
    {
        generateRandomNumberStepGen2();
    }
}

void randomize_gen1_DVs(uint8_t *dv_array)
{
    for (int i = 0; i < PKSAV_NUM_GB_IVS - 1; i++)
    {
        // random int between 0 and 15
        dv_array[i] = (uint8_t)(get_random_byte() & 0xF);
        generate_random_number_step();
    }

    // Generate HP dv from other dvs (string LSBs together)
    dv_array[PKSAV_GB_IV_HP] = (((dv_array[PKSAV_GB_IV_ATTACK] & 0x01) << 3) |
                                ((dv_array[PKSAV_GB_IV_DEFENSE] & 0x01) << 2) |
                                ((dv_array[PKSAV_GB_IV_SPEED] & 0x01) << 1) |
                                (dv_array[PKSAV_GB_IV_SPECIAL] & 0x01));
}

void randomize_gen2_DVs(uint8_t *dv_array)
{
    for (int i = 0; i < PKSAV_NUM_GB_IVS - 1; i++)
    {
        // random int between 0 and 15
        dv_array[i] = (uint8_t)(getRandomByteGen2() & 0xF);
        generateRandomNumberStepGen2();
    }

    // Generate HP dv from other dvs (string LSBs together)
    dv_array[PKSAV_GB_IV_HP] = (((dv_array[PKSAV_GB_IV_ATTACK] & 0x01) << 3) |
                                ((dv_array[PKSAV_GB_IV_DEFENSE] & 0x01) << 2) |
                                ((dv_array[PKSAV_GB_IV_SPEED] & 0x01) << 1) |
                                (dv_array[PKSAV_GB_IV_SPECIAL] & 0x01));
}
/*************************************************************************/

// Randomize the DVs of a pokemon on trade
void update_pkmn_DVs(PokemonSave *pokemon_save, int pokemon_index)
{
    // From settings menu to prevent changing DVs on trade
    if (disable_random_DVs_on_trade)
    {
        printf("Random IVs on trade disabled\n");
        return;
    }
    // randomize the dvs on trade except for HP
    uint8_t traded_pkmn_rand_dvs[PKSAV_NUM_GB_IVS] = {0};
    randomize_gen1_DVs(traded_pkmn_rand_dvs);

    // set the ivs to pokemon at index
    for (int i = PKSAV_GB_IV_ATTACK; i < PKSAV_NUM_GB_IVS; i++)
    {
        pksav_set_gb_IV(i, traded_pkmn_rand_dvs[i], &pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.iv_data);
    }
}

// Calculate and update the pokemon's stats based on its level, base stats, IVs, and EVs
void update_pkmn_stats_gen1(PokemonSave *pokemon_save, int pokemon_index, const struct pksav_gen1_pokemon_party_data *pkmn_base)
{
    // Get the pokemon's DVs
    uint8_t pkmn_dvs[PKSAV_NUM_GB_IVS];
    pksav_get_gb_IVs(&pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.iv_data, pkmn_dvs, sizeof(pkmn_dvs));

    // Get the pokemon's EVs
    struct pksav_gen1_pc_pokemon pkmn_ev_data = pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data;
    const uint16_t pkmn_evs[PKSAV_NUM_GB_IVS] = {
        [PKSAV_GB_IV_ATTACK] = pksav_bigendian16(pkmn_ev_data.ev_atk),
        [PKSAV_GB_IV_DEFENSE] = pksav_bigendian16(pkmn_ev_data.ev_def),
        [PKSAV_GB_IV_SPEED] = pksav_bigendian16(pkmn_ev_data.ev_spd),
        [PKSAV_GB_IV_SPECIAL] = pksav_bigendian16(pkmn_ev_data.ev_spcl),
        [PKSAV_GB_IV_HP] = pksav_bigendian16(pkmn_ev_data.ev_hp),
    };

    // Calculate the pokemon's HP stat
    uint8_t pkmn_stats[PKSAV_NUM_GB_IVS] = {
        [PKSAV_GB_IV_HP] = calculate_hp(pkmn_ev_data.level, pkmn_base->max_hp, pkmn_dvs[PKSAV_GB_IV_HP], pkmn_evs[PKSAV_GB_IV_HP]),
    };

    // Calculate the pokemon's other stats
    pkmn_stats[PKSAV_GB_IV_ATTACK] = calculate_stat(pkmn_ev_data.level, pkmn_base->atk, pkmn_dvs[PKSAV_GB_IV_ATTACK], pkmn_evs[PKSAV_GB_IV_ATTACK]);
    pkmn_stats[PKSAV_GB_IV_DEFENSE] = calculate_stat(pkmn_ev_data.level, pkmn_base->def, pkmn_dvs[PKSAV_GB_IV_DEFENSE], pkmn_evs[PKSAV_GB_IV_DEFENSE]);
    pkmn_stats[PKSAV_GB_IV_SPEED] = calculate_stat(pkmn_ev_data.level, pkmn_base->spd, pkmn_dvs[PKSAV_GB_IV_SPEED], pkmn_evs[PKSAV_GB_IV_SPEED]);
    pkmn_stats[PKSAV_GB_IV_SPECIAL] = calculate_stat(pkmn_ev_data.level, pkmn_base->spcl, pkmn_dvs[PKSAV_GB_IV_SPECIAL], pkmn_evs[PKSAV_GB_IV_SPECIAL]);

    // Update the pokemon's stats
    pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].party_data.max_hp = pksav_bigendian16(pkmn_stats[PKSAV_GB_IV_HP]);
    pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].party_data.atk = pksav_bigendian16(pkmn_stats[PKSAV_GB_IV_ATTACK]);
    pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].party_data.def = pksav_bigendian16(pkmn_stats[PKSAV_GB_IV_DEFENSE]);
    pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].party_data.spd = pksav_bigendian16(pkmn_stats[PKSAV_GB_IV_SPEED]);
    pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].party_data.spcl = pksav_bigendian16(pkmn_stats[PKSAV_GB_IV_SPECIAL]);
}

// Convert party pokemon to evolution pokemon with updated stats and properties
void generate_pkmn_evolution(PokemonSave *pokemon_save, int pokemon_index, struct pksav_gen1_party_pokemon pkmn_base, int species_index)
{
    // Calculates and updates stats
    update_pkmn_stats_gen1(pokemon_save, pokemon_index, &pkmn_base.party_data);

    // Update species index
    pokemon_save->save.gen1_save.pokemon_storage.p_party->species[pokemon_index] = (uint8_t)species_index;
    pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.species = (uint8_t)species_index;

    // Update types
    pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.types[0] = pkmn_base.pc_data.types[0];
    pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.types[1] = pkmn_base.pc_data.types[1];

    // Update catch rate
    pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.catch_rate = pkmn_base.pc_data.catch_rate;

    // Update condition to none
    pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.condition = PKSAV_GB_CONDITION_NONE;

    // Update health to max hp
    pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.current_hp = pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].party_data.max_hp;

    // TODO: Update moves based on learn set and level
    // engine/pokemon/evos_moves.asm
    // pokemon_save->save.gen1_save.pokemon_storage.p_party->party[pokemon_index].pc_data.moves;
}

// Execute evolution for party pokemon at index
void evolve_party_pokemon_at_index(PokemonSave *pokemon_save, int pokemon_index)
{
    // Handle Generation 1 Evolutions
    if (pokemon_save->save_generation_type == SAVE_GENERATION_1)
    {
        // Get the pokemon's species index
        int species = pokemon_save->save.gen1_save.pokemon_storage.p_party->species[pokemon_index];

        // Get the pokemon's nickname
        char pokemon_name[11];
        pksav_gen1_import_text(pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index], pokemon_name, 10);

        switch (species)
        {
        // Kadabra -> Alakazam
        case KADABRA:
            // Check if pokemon does not have custom nickname
            // if the nickname is not custom, then update the pokemon nickname
            // else do not modify the nickname
            if (strcmp(pokemon_name, "KADABRA") == 0)
            {
                // Write default nickname for species to pokemon
                pksav_gen1_export_text("ALAKAZAM", pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index], 10);
                // Set the last character to 0x50 to terminate the string
                pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index][strlen("ALAKAZAM")] = 0x50;
            }
            // Update the pokemon's stats and properties
            generate_pkmn_evolution(pokemon_save, pokemon_index, alakazam_base_stats, ALAKAZAM);
            break;
        // Machoke -> Machamp
        case MACHOKE:
            // Check if pokemon does not have custom nickname
            // if the nickname is not custom, then update the pokemon nickname
            // else do not modify the nickname
            if (strcmp(pokemon_name, "MACHOKE") == 0)
            {
                // Write default nickname for species to pokemon
                pksav_gen1_export_text("MACHAMP", pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index], 10);
                // Set the last character to 0x50 to terminate the string
                pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index][strlen("MACHAMP")] = 0x50;
            }
            // Update the pokemon's stats and properties
            generate_pkmn_evolution(pokemon_save, pokemon_index, machamp_base_stats, MACHAMP);
            break;
        // Graveler -> Golem
        case GRAVELER:
            // Check if pokemon does not have custom nickname
            // if the nickname is not custom, then update the pokemon nickname
            // else do not modify the nickname
            if (strcmp(pokemon_name, "GRAVELER") == 0)
            {
                // Write default nickname for species to pokemon
                pksav_gen1_export_text("GOLEM", pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index], 10);
                // Set the last character to 0x50 to terminate the string
                pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index][strlen("GOLEM")] = 0x50;
            }
            // Update the pokemon's stats and properties
            generate_pkmn_evolution(pokemon_save, pokemon_index, golem_base_stats, GOLEM);
            break;
        // Haunter -> Gengar
        case HAUNTER:
            // Check if pokemon does not have custom nickname
            // if the nickname is not custom, then update the pokemon nickname
            // else do not modify the nickname
            if (strcmp(pokemon_name, "HAUNTER") == 0)
            {
                // Write default nickname for species to pokemon
                pksav_gen1_export_text("GENGAR", pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index], 10);
                // Set the last character to 0x50 to terminate the string
                pokemon_save->save.gen1_save.pokemon_storage.p_party->nicknames[pokemon_index][strlen("GENGAR")] = 0x50;
            }
            // Update the pokemon's stats and properties
            generate_pkmn_evolution(pokemon_save, pokemon_index, gengar_base_stats, GENGAR);
            break;
        default:
            break;
        }
    }
    else
    {
        // TODO: Implement trade evolutions for gen 2
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
// Settings getter for random DVs on trade
bool get_is_random_DVs_disabled(void)
{
    return disable_random_DVs_on_trade;
}

// Settings setter for random DVs on trade
void set_is_random_DVs_disabled(bool is_disabled)
{
    disable_random_DVs_on_trade = is_disabled;
}
