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

void update_seen_owned_pkmn(PokemonSave *pkmn_save, uint8_t pokemon_party_index)
{
    enum pksav_error err;
    if (pkmn_save->save_generation_type == SAVE_GENERATION_1)
    {
        uint8_t pokemon_species = pkmn_save->save.gen1_save.pokemon_storage.p_party->species[pokemon_party_index];
        err = pksav_set_pokedex_bit(pkmn_save->save.gen1_save.pokedex_lists.p_seen, pokemon_species, true);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error setting seen pokedex bit");
        }

        err = pksav_set_pokedex_bit(pkmn_save->save.gen1_save.pokedex_lists.p_owned, pokemon_species, true);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error setting owned pokedex bit");
        }
    }
    else
    {
        uint8_t pokemon_species = pkmn_save->save.gen2_save.pokemon_storage.p_party->species[pokemon_party_index];
        err = pksav_set_pokedex_bit(pkmn_save->save.gen2_save.pokedex_lists.p_seen, pokemon_species, true);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error setting seen pokedex bit");
        }

        err = pksav_set_pokedex_bit(pkmn_save->save.gen2_save.pokedex_lists.p_owned, pokemon_species, true);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error setting owned pokedex bit");
        }
    }
}

void swap_party_pkmn_at_indices(struct pksav_gen2_save *save, uint8_t pkmn_party_index1, uint8_t pkmn_party_index2)
{
    // swap nickname
    char tmp_nickname1[11];
    char tmp_nickname2[11];
    pksav_gen2_import_text(save->pokemon_storage.p_party->nicknames[pkmn_party_index1], tmp_nickname1, 10);
    pksav_gen2_import_text(save->pokemon_storage.p_party->nicknames[pkmn_party_index2], tmp_nickname2, 10);
    pksav_gen2_export_text(tmp_nickname2, save->pokemon_storage.p_party->nicknames[pkmn_party_index1], 10);
    pksav_gen2_export_text(tmp_nickname1, save->pokemon_storage.p_party->nicknames[pkmn_party_index2], 10);
    save->pokemon_storage.p_party->nicknames[pkmn_party_index1][strlen(tmp_nickname2)] = 0x50;
    save->pokemon_storage.p_party->nicknames[pkmn_party_index2][strlen(tmp_nickname1)] = 0x50;

    // swap party data
    struct pksav_gen2_party_pokemon tmp_pokemon = save->pokemon_storage.p_party->party[pkmn_party_index1];
    save->pokemon_storage.p_party->party[pkmn_party_index1] = save->pokemon_storage.p_party->party[pkmn_party_index2];
    save->pokemon_storage.p_party->party[pkmn_party_index2] = tmp_pokemon;

    // swap species
    uint8_t tmp_species = save->pokemon_storage.p_party->species[pkmn_party_index1];
    save->pokemon_storage.p_party->species[pkmn_party_index1] = save->pokemon_storage.p_party->species[pkmn_party_index2];
    save->pokemon_storage.p_party->species[pkmn_party_index2] = tmp_species;

    // swap otnames
    char tmp_otname1[8];
    char tmp_otname2[8];
    pksav_gen2_import_text(save->pokemon_storage.p_party->otnames[pkmn_party_index1], tmp_otname1, 7);
    pksav_gen2_import_text(save->pokemon_storage.p_party->otnames[pkmn_party_index2], tmp_otname2, 7);
    pksav_gen2_export_text(tmp_otname2, save->pokemon_storage.p_party->otnames[pkmn_party_index1], 7);
    pksav_gen2_export_text(tmp_otname1, save->pokemon_storage.p_party->otnames[pkmn_party_index2], 7);
    save->pokemon_storage.p_party->otnames[pkmn_party_index1][strlen(tmp_otname2)] = 0x50;
    save->pokemon_storage.p_party->otnames[pkmn_party_index2][strlen(tmp_otname1)] = 0x50;
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
void create_trainer(PokemonSave *pkmn_save, struct TrainerInfo *trainer)
{
    SaveGenerationType save_generation_type = pkmn_save->save_generation_type;

    switch (save_generation_type)
    {
    case SAVE_GENERATION_1:
    {
        // Trainer name
        char trainer_name[8];
        pksav_gen1_import_text(pkmn_save->save.gen1_save.trainer_info.p_name, trainer_name, 7);

        // Trainer Id
        uint16_t trainer_id = pksav_bigendian16(*pkmn_save->save.gen1_save.trainer_info.p_id);

        // Update the trainer struct
        strcpy(trainer->trainer_name, trainer_name);
        trainer->trainer_id = trainer_id;

        trainer->trainer_badges[0] = *pkmn_save->save.gen1_save.trainer_info.p_badges;

        // Update the trainer's pokemon party
        trainer->pokemon_party.gen1_pokemon_party = *pkmn_save->save.gen1_save.pokemon_storage.p_party;
        trainer->trainer_generation = SAVE_GENERATION_1;
        break;
    }
    case SAVE_GENERATION_2:
    {
        // Trainer name
        char trainer_name[8];
        pksav_gen2_import_text(pkmn_save->save.gen2_save.trainer_info.p_name, trainer_name, 7);

        // Update the trainer struct
        strcpy(trainer->trainer_name, trainer_name);
        trainer->trainer_id = pksav_bigendian16(*pkmn_save->save.gen2_save.trainer_info.p_id);
        trainer->trainer_badges[EBadge_Region_Johto] = *pkmn_save->save.gen2_save.trainer_info.p_johto_badges;
        trainer->trainer_badges[EBadge_Region_Kanto] = *pkmn_save->save.gen2_save.trainer_info.p_kanto_badges;
        // Trainer Gender (Crystal and later games only)
        if (pkmn_save->save.gen2_save.save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL)
        {
            trainer->trainer_gender = *pkmn_save->save.gen2_save.trainer_info.p_gender;
        }

        // Update the trainer's pokemon party
        trainer->pokemon_party.gen2_pokemon_party = *pkmn_save->save.gen2_save.pokemon_storage.p_party;
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
int check_trade_evolution_gen1(PokemonSave *pkmn_save, uint8_t pkmn_party_index)
{
    // Species index of pokemon being checked
    uint8_t species = pkmn_save->save.gen1_save.pokemon_storage.p_party->species[pkmn_party_index];

    // Pokemon species eligible for trade evolution in Gen 1
    uint8_t gen1_evolutions[4] = {
        (uint8_t)KADABRA,
        (uint8_t)MACHOKE,
        (uint8_t)GRAVELER,
        (uint8_t)HAUNTER};

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
enum eligible_evolution_status check_trade_evolution_gen2(PokemonSave *pkmn_save, uint8_t pkmn_party_index)
{
    // Species index of pokemon being checked
    int species = pkmn_save->save.gen2_save.pokemon_storage.p_party->species[pkmn_party_index];
    // Item held index by pokemon being checked
    int item = pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.held_item;

    struct pkmn_evolution_pair_data evo_pair = pkmn_evolution_pairs[species];
    // If the pkmn species has an initialized evolution pair
    if (species == evo_pair.species_index)
    {
        // Pokemon eligible for trade evolution but missing item
        if (evo_pair.evolution_item != 0 && evo_pair.evolution_item != item)
        {
            return E_EVO_STATUS_MISSING_ITEM;
        }

        // Pokemon eligible for trade evolution
        if (evo_pair.evolution_item == item)
        {
            return E_EVO_STATUS_ELIGIBLE;
        }
    }

    return E_EVO_STATUS_NOT_ELIGIBLE;
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
// Simulated hardware registers
uint8_t h_random_add = 0; // h_random_add register
uint8_t h_random_sub = 0; // h_random_sub register

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

// Get a random byte from the simulated random number generation process
uint8_t get_random_byte(SaveGenerationType save_generation_type)
{
    if (save_generation_type == SAVE_GENERATION_1)
    {
        return add_byte; // Return the add byte as the generated random number
    }
    else
    {
        return h_random_add; // Return the simulated hRandomAdd register
    }
}

// Gen 2 used hardware registers to generate random numbers
// Perform the random number generation step
void generate_random_number_step_gen2(void)
{
    r_div++; // Increment rDiv (simulated as an 8-bit counter)

    // Simulate the addition step
    uint16_t sum = r_div + h_random_add;
    h_random_add = (uint8_t)sum;

    // Simulate the subtraction step
    uint16_t diff = r_div - h_random_sub;
    h_random_sub = (uint8_t)diff;
}

void generate_rand_num_step(SaveGenerationType save_generation_type)
{
    if (save_generation_type == SAVE_GENERATION_1)
    {
        generate_random_number_step();
    }
    else
    {
        generate_random_number_step_gen2();
    }
}

void randomize_dvs(uint8_t *dv_array, SaveGenerationType save_generation_type)
{
    for (int i = 0; i < PKSAV_NUM_GB_IVS - 1; i++)
    {
        // random int between 0 and 15
        dv_array[i] = (uint8_t)(get_random_byte(save_generation_type) & 0xF);
        generate_rand_num_step(save_generation_type);
    }

    // Generate HP dv from other dvs (string LSBs together)
    dv_array[PKSAV_GB_IV_HP] = (((dv_array[PKSAV_GB_IV_ATTACK] & 0x01) << 3) |
                                ((dv_array[PKSAV_GB_IV_DEFENSE] & 0x01) << 2) |
                                ((dv_array[PKSAV_GB_IV_SPEED] & 0x01) << 1) |
                                (dv_array[PKSAV_GB_IV_SPECIAL] & 0x01));
}
/*************************************************************************/

// Randomize the DVs of a pokemon on trade
void update_pkmn_DVs(PokemonSave *pkmn_save, uint8_t pkmn_party_index)
{
    // From settings menu to prevent changing DVs on trade
    if (disable_random_DVs_on_trade)
    {
        printf("Random IVs on trade disabled\n");
        return;
    }
    // randomize the dvs on trade except for HP
    uint8_t traded_pkmn_rand_dvs[PKSAV_NUM_GB_IVS] = {0};
    randomize_dvs(traded_pkmn_rand_dvs, pkmn_save->save_generation_type);

    // set the ivs to pokemon at index
    for (int i = PKSAV_GB_IV_ATTACK; i < PKSAV_NUM_GB_IVS; i++)
    {
        pksav_set_gb_IV(i, traded_pkmn_rand_dvs[i], &pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.iv_data);
    }
}

// Calculate and update the pokemon's stats based on its level, base stats, IVs, and EVs
void update_pkmn_stats(PokemonSave *pkmn_save, uint8_t pkmn_party_index, const uint8_t evolution_index)
{
    // Get the pokemon's DVs
    uint8_t pkmn_dvs[PKSAV_NUM_GB_IVS];
    pksav_get_gb_IVs(&pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.iv_data, pkmn_dvs, sizeof(pkmn_dvs));

    if (pkmn_save->save_generation_type == SAVE_GENERATION_1)
    {
        // Get the pokemon's EVs
        struct pksav_gen1_pc_pokemon pkmn_ev_data = pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data;
        const uint16_t pkmn_evs[PKSAV_NUM_GB_IVS] = {
            [PKSAV_GB_IV_ATTACK] = pksav_bigendian16(pkmn_ev_data.ev_atk),
            [PKSAV_GB_IV_DEFENSE] = pksav_bigendian16(pkmn_ev_data.ev_def),
            [PKSAV_GB_IV_SPEED] = pksav_bigendian16(pkmn_ev_data.ev_spd),
            [PKSAV_GB_IV_SPECIAL] = pksav_bigendian16(pkmn_ev_data.ev_spcl),
            [PKSAV_GB_IV_HP] = pksav_bigendian16(pkmn_ev_data.ev_hp)};

        int8_t pkmn_stats[PKSAV_GEN1_STAT_COUNT] = {
            // Calculate the pokemon's stats
            [PKSAV_GEN1_STAT_ATTACK] = calculate_stat(pkmn_ev_data.level, pkmn_base_stats[evolution_index].atk, pkmn_dvs[PKSAV_GB_IV_ATTACK], pkmn_evs[PKSAV_GB_IV_ATTACK]),
            [PKSAV_GEN1_STAT_DEFENSE] = calculate_stat(pkmn_ev_data.level, pkmn_base_stats[evolution_index].def, pkmn_dvs[PKSAV_GB_IV_DEFENSE], pkmn_evs[PKSAV_GB_IV_DEFENSE]),
            [PKSAV_GEN1_STAT_SPEED] = calculate_stat(pkmn_ev_data.level, pkmn_base_stats[evolution_index].spd, pkmn_dvs[PKSAV_GB_IV_SPEED], pkmn_evs[PKSAV_GB_IV_SPEED]),
            [PKSAV_GEN1_STAT_SPECIAL] = calculate_stat(pkmn_ev_data.level, pkmn_base_stats[evolution_index].spcl, pkmn_dvs[PKSAV_GB_IV_SPECIAL], pkmn_evs[PKSAV_GB_IV_SPECIAL]),
            // Calculate the pokemon's HP stat
            [PKSAV_GEN1_STAT_HP] = calculate_hp(pkmn_ev_data.level, pkmn_base_stats[evolution_index].max_hp, pkmn_dvs[PKSAV_GB_IV_HP], pkmn_evs[PKSAV_GB_IV_HP])};

        // Update the pokemon's stats
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.atk = pksav_bigendian16(pkmn_stats[PKSAV_GEN1_STAT_ATTACK]);
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.def = pksav_bigendian16(pkmn_stats[PKSAV_GEN1_STAT_DEFENSE]);
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.spd = pksav_bigendian16(pkmn_stats[PKSAV_GEN1_STAT_SPEED]);
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.spcl = pksav_bigendian16(pkmn_stats[PKSAV_GEN1_STAT_SPECIAL]);
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.max_hp = pksav_bigendian16(pkmn_stats[PKSAV_GEN1_STAT_HP]);
    }
    else
    {
        // Get the pokemon's EVs
        struct pksav_gen2_pc_pokemon pkmn_ev_data = pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data;
        const uint16_t pkmn_evs[PKSAV_NUM_GB_IVS] = {
            [PKSAV_GB_IV_ATTACK] = pksav_bigendian16(pkmn_ev_data.ev_atk),
            [PKSAV_GB_IV_DEFENSE] = pksav_bigendian16(pkmn_ev_data.ev_def),
            [PKSAV_GB_IV_SPEED] = pksav_bigendian16(pkmn_ev_data.ev_spd),
            [PKSAV_GB_IV_SPECIAL] = pksav_bigendian16(pkmn_ev_data.ev_spcl),
            [PKSAV_GB_IV_HP] = pksav_bigendian16(pkmn_ev_data.ev_hp)};

        int8_t pkmn_stats[PKSAV_GEN2_STAT_COUNT] = {
            // Calculate the pokemon's stats
            [PKSAV_GEN2_STAT_ATTACK] = calculate_stat(pkmn_ev_data.level, pkmn_base_stats[evolution_index].atk, pkmn_dvs[PKSAV_GB_IV_ATTACK], pkmn_evs[PKSAV_GB_IV_ATTACK]),
            [PKSAV_GEN2_STAT_DEFENSE] = calculate_stat(pkmn_ev_data.level, pkmn_base_stats[evolution_index].def, pkmn_dvs[PKSAV_GB_IV_DEFENSE], pkmn_evs[PKSAV_GB_IV_DEFENSE]),
            [PKSAV_GEN2_STAT_SPEED] = calculate_stat(pkmn_ev_data.level, pkmn_base_stats[evolution_index].spd, pkmn_dvs[PKSAV_GB_IV_SPEED], pkmn_evs[PKSAV_GB_IV_SPEED]),
            [PKSAV_GEN2_STAT_SPATK] = calculate_stat(pkmn_ev_data.level, pkmn_base_stats[evolution_index].spatk, pkmn_dvs[PKSAV_GB_IV_SPECIAL], pkmn_evs[PKSAV_GB_IV_SPECIAL]),
            [PKSAV_GEN2_STAT_SPDEF] = calculate_stat(pkmn_ev_data.level, pkmn_base_stats[evolution_index].spdef, pkmn_dvs[PKSAV_GB_IV_SPECIAL], pkmn_evs[PKSAV_GB_IV_SPECIAL]),
            // Calculate the pokemon's HP stat
            [PKSAV_GEN2_STAT_HP] = calculate_hp(pkmn_ev_data.level, pkmn_base_stats[evolution_index].max_hp, pkmn_dvs[PKSAV_GB_IV_HP], pkmn_evs[PKSAV_GB_IV_HP])};

        // Update the pokemon's stats
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.atk = pksav_bigendian16(pkmn_stats[PKSAV_GEN2_STAT_ATTACK]);
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.def = pksav_bigendian16(pkmn_stats[PKSAV_GEN2_STAT_DEFENSE]);
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.spd = pksav_bigendian16(pkmn_stats[PKSAV_GEN2_STAT_SPEED]);
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.spatk = pksav_bigendian16(pkmn_stats[PKSAV_GEN2_STAT_SPATK]);
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.spdef = pksav_bigendian16(pkmn_stats[PKSAV_GEN2_STAT_SPDEF]);
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.max_hp = pksav_bigendian16(pkmn_stats[PKSAV_GEN2_STAT_HP]);
    }
}

// Convert party pokemon to evolution pokemon with updated stats and properties
void evolve_party_pokemon_at_index(PokemonSave *pkmn_save, uint8_t pkmn_party_index)
{
    if (pkmn_save->save_generation_type == SAVE_GENERATION_1)
    {
        // Get the species index of the pokemon being evolved
        uint8_t pkmn_species_index = pkmn_save->save.gen1_save.pokemon_storage.p_party->species[pkmn_party_index];
        // Get the data for the pokemon's evolution
        char evolution_name[11];
        strcpy(evolution_name, pkmn_evolution_pairs[pkmn_species_index].evolution_name);
        char species_name[11];
        strcpy(species_name, pkmn_evolution_pairs[pkmn_species_index].species_name);
        uint8_t evolution_index = pkmn_evolution_pairs[pkmn_species_index].evolution_index;

        // Calculates and updates stats
        update_pkmn_stats(pkmn_save, pkmn_party_index, evolution_index);

        // Get the pokemon's nickname
        char pkmn_save_nickname[11];
        pksav_gen1_import_text(pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[pkmn_party_index], pkmn_save_nickname, 10);

        // Check if pokemon does not have custom nickname
        // if the nickname is not custom, then update the pokemon nickname
        // else do not modify the nickname
        if (strcmp(pkmn_save_nickname, species_name) == 0)
        {
            // Write default nickname for species to pokemon
            pksav_gen1_export_text(evolution_name, pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[pkmn_party_index], 10);
            // Set the last character to 0x50 to terminate the string
            pkmn_save->save.gen1_save.pokemon_storage.p_party->nicknames[pkmn_party_index][strlen(evolution_name)] = 0x50;
        }

        // Update species index
        pkmn_save->save.gen1_save.pokemon_storage.p_party->species[pkmn_party_index] = evolution_index;
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.species = evolution_index;
        // Update types
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.types[0] = pkmn_base_stats[evolution_index].types[0];
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.types[1] = pkmn_base_stats[evolution_index].types[1];
        // Update catch rate
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.catch_rate = pkmn_base_stats[evolution_index].catch_rate;
        // Update condition to none
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.condition = PKSAV_GB_CONDITION_NONE;
        // Update health to max hp
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.current_hp = pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.max_hp;
        // TODO: Update moves based on learn set and level
        // engine/pokemon/evos_moves.asm
        // pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.moves;
    }
    else
    {
        // Get the species index of the pokemon being evolved
        uint8_t pkmn_species_index = pkmn_save->save.gen2_save.pokemon_storage.p_party->species[pkmn_party_index];
        // Get the data for the pokemon's evolution
        char evolution_name[11];
        strcpy(evolution_name, pkmn_evolution_pairs[pkmn_species_index].evolution_name);
        char species_name[11];
        strcpy(species_name, pkmn_evolution_pairs[pkmn_species_index].species_name);
        uint8_t evolution_index = pkmn_evolution_pairs[pkmn_species_index].evolution_index;

        // Calculates and updates stats
        update_pkmn_stats(pkmn_save, pkmn_party_index, evolution_index);

        // Get the pokemon's nickname
        char pkmn_save_nickname[11];
        pksav_gen2_import_text(pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[pkmn_party_index], pkmn_save_nickname, 10);

        // Check if pokemon does not have custom nickname
        // if the nickname is not custom, then update the pokemon nickname
        // else do not modify the nickname
        if (strcmp(pkmn_save_nickname, species_name) == 0)
        {
            // Write default nickname for species to pokemon
            pksav_gen2_export_text(evolution_name, pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[pkmn_party_index], 10);
            // Set the last character to 0x50 to terminate the string
            pkmn_save->save.gen2_save.pokemon_storage.p_party->nicknames[pkmn_party_index][strlen(evolution_name)] = 0x50;
        }

        // Update species index
        pkmn_save->save.gen2_save.pokemon_storage.p_party->species[pkmn_party_index] = evolution_index;
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.species = evolution_index;
        // Update condition to none
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.condition = PKSAV_CONDITION_NONE;
        // Update health to max hp
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.current_hp = pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.max_hp;
        // TODO: Update moves based on learn set and level
        // engine/pokemon/evos_moves.asm
        // pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.moves;
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
