#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "pksavhelper.h"

int error_handler(enum pksav_error error, const char *message)
{
    printf("%s\n", message);
    // TODO: Print error to log file
    return 0;
}

void update_seen_owned_pkmn(PokemonSave *pkmn_save, uint8_t pokemon_party_index)
{
    enum pksav_error err;
    if (pkmn_save->save_generation_type == SAVE_GENERATION_1)
    {
        uint8_t pkmn_species = pkmn_save->save.gen1_save.pokemon_storage.p_party->species[pokemon_party_index];
        uint8_t pkdex_entry = species_gen1_to_gen2[pkmn_species];    // Gen2 is pokedex ordered
        err = pksav_set_pokedex_bit(pkmn_save->save.gen1_save.pokedex_lists.p_seen, pkdex_entry, true);
        if (err != PKSAV_ERROR_NONE)
        {
            error_handler(err, "Error setting seen pokedex bit");
        }

        err = pksav_set_pokedex_bit(pkmn_save->save.gen1_save.pokedex_lists.p_owned, pkdex_entry, true);
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

// Checks if supplied Gen 1 pokemon is eligible for trade evolution
enum eligible_evolution_status check_trade_evolution_gen1(PokemonSave *pkmn_save, uint8_t pkmn_party_index)
{
    // Species index of pokemon being checked
    uint8_t species = pkmn_save->save.gen1_save.pokemon_storage.p_party->species[pkmn_party_index];
    struct pkmn_evolution_pair_data evo_pair = pkmn_evolution_pairs_gen1[species];

    // Check if the pokemon has an initialized evolution pair
    return species == evo_pair.species_index;
}

// Checks if supplied Gen 2 pokemon is eligible for trade evolution
enum eligible_evolution_status check_trade_evolution_gen2(PokemonSave *pkmn_save, uint8_t pkmn_party_index)
{
    // Species index of pokemon being checked
    int species = pkmn_save->save.gen2_save.pokemon_storage.p_party->species[pkmn_party_index];
    // Item held index by pokemon being checked
    int item = pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.held_item;

    struct pkmn_evolution_pair_data evo_pair = pkmn_evolution_pairs_gen2[species];
    // If the pkmn species has an initialized evolution pair
    if (species == evo_pair.species_index)
    {
        // Pokemon eligible for trade evolution but missing item
        if (evo_pair.evolution_item != item && get_is_item_required())
        {
            return E_EVO_STATUS_MISSING_ITEM;
        }

        // Pokemon eligible for trade evolution
        if (evo_pair.evolution_item == item || !get_is_item_required())
        {
            return E_EVO_STATUS_ELIGIBLE;
        }
    }

    return E_EVO_STATUS_NOT_ELIGIBLE;
}

// Function to calculate HP based on base stat, IV, Stat Exp, and level
uint8_t calculate_hp(uint8_t level, uint8_t base_hp, uint8_t dv_hp, uint16_t stat_exp)
{
    float hp_calc = (base_hp + dv_hp) * 2 + floor(ceil(sqrt(stat_exp)) / 4);
    hp_calc = hp_calc * level / 100.0;
    hp_calc = floor(hp_calc) + level + 10;

    return (uint8_t)hp_calc;
}

// Function to calculate stats (Attack, Defense, Special, Speed)
// based on base stat, IV, Stat Exp, and level
uint8_t calculate_stat(uint8_t level, uint8_t base_stat, uint8_t dv, uint16_t stat_exp)
{
    float stat_calc = (base_stat + dv) * 2 + floor(ceil(sqrt(stat_exp)) / 4);
    stat_calc = (stat_calc * level) / 100.0;
    stat_calc = floor(stat_calc) + 5;

    return (uint8_t)stat_calc;
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
        printf("DVs not randomized on trade\n");
        return;
    }
    // randomize the dvs on trade except for HP
    uint8_t traded_pkmn_rand_dvs[PKSAV_NUM_GB_IVS] = {0};
    randomize_dvs(traded_pkmn_rand_dvs, pkmn_save->save_generation_type);

    // set the ivs to pokemon at index
    for (int i = PKSAV_GB_IV_ATTACK; i < PKSAV_NUM_GB_IVS; i++)
    {
        if (pkmn_save->save_generation_type == SAVE_GENERATION_1) pksav_set_gb_IV(i, traded_pkmn_rand_dvs[i], &pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.iv_data);
        else pksav_set_gb_IV(i, traded_pkmn_rand_dvs[i], &pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.iv_data);
    }
}

// Calculate and update the pokemon's stats based on its level, base stats, IVs, and EVs
void update_pkmn_stats(PokemonSave *pkmn_save, uint8_t pkmn_party_index)
{
    // From settings menu to prevent changing DVs on trade
    if (disable_random_DVs_on_trade)
    {
        printf("DVs not randomized on trade\n");
        return;
    }
    // Get the pokemon's DVs
    uint8_t pkmn_dvs[PKSAV_NUM_GB_IVS];
    pksav_get_gb_IVs(&pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.iv_data, pkmn_dvs, sizeof(pkmn_dvs));

    if (pkmn_save->save_generation_type == SAVE_GENERATION_1)
    {
        // Get species index
        uint8_t species_index = pkmn_save->save.gen1_save.pokemon_storage.p_party->species[pkmn_party_index];
        uint8_t pkmn_level = pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.level;

        // Get the pokemon's EVs
        struct pksav_gen1_pc_pokemon pkmn_ev_data = pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data;
        const uint16_t pkmn_evs[PKSAV_NUM_GB_IVS] = {
            [PKSAV_GB_IV_ATTACK] = pksav_bigendian16(pkmn_ev_data.ev_atk),
            [PKSAV_GB_IV_DEFENSE] = pksav_bigendian16(pkmn_ev_data.ev_def),
            [PKSAV_GB_IV_SPEED] = pksav_bigendian16(pkmn_ev_data.ev_spd),
            [PKSAV_GB_IV_SPECIAL] = pksav_bigendian16(pkmn_ev_data.ev_spcl),
            [PKSAV_GB_IV_HP] = pksav_bigendian16(pkmn_ev_data.ev_hp)};

        uint8_t pkmn_stats[PKSAV_GEN1_STAT_COUNT] = {
            // Calculate the pokemon's stats
            [PKSAV_GEN1_STAT_ATTACK] = calculate_stat(pkmn_level, pkmn_base_stats_gen1[species_index].atk, pkmn_dvs[PKSAV_GB_IV_ATTACK], pkmn_evs[PKSAV_GB_IV_ATTACK]),
            [PKSAV_GEN1_STAT_DEFENSE] = calculate_stat(pkmn_level, pkmn_base_stats_gen1[species_index].def, pkmn_dvs[PKSAV_GB_IV_DEFENSE], pkmn_evs[PKSAV_GB_IV_DEFENSE]),
            [PKSAV_GEN1_STAT_SPEED] = calculate_stat(pkmn_level, pkmn_base_stats_gen1[species_index].spd, pkmn_dvs[PKSAV_GB_IV_SPEED], pkmn_evs[PKSAV_GB_IV_SPEED]),
            [PKSAV_GEN1_STAT_SPECIAL] = calculate_stat(pkmn_level, pkmn_base_stats_gen1[species_index].spcl, pkmn_dvs[PKSAV_GB_IV_SPECIAL], pkmn_evs[PKSAV_GB_IV_SPECIAL]),
            // Calculate the pokemon's HP stat
            [PKSAV_GEN1_STAT_HP] = calculate_hp(pkmn_level, pkmn_base_stats_gen1[species_index].max_hp, pkmn_dvs[PKSAV_GB_IV_HP], pkmn_evs[PKSAV_GB_IV_HP])};

        // Update the pokemon's stats
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.atk = pksav_bigendian16(pkmn_stats[PKSAV_GEN1_STAT_ATTACK]);
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.def = pksav_bigendian16(pkmn_stats[PKSAV_GEN1_STAT_DEFENSE]);
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.spd = pksav_bigendian16(pkmn_stats[PKSAV_GEN1_STAT_SPEED]);
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.spcl = pksav_bigendian16(pkmn_stats[PKSAV_GEN1_STAT_SPECIAL]);
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.max_hp = pksav_bigendian16(pkmn_stats[PKSAV_GEN1_STAT_HP]);
    }
    else
    {
        // Get the species index
        uint8_t species_index = pkmn_save->save.gen2_save.pokemon_storage.p_party->species[pkmn_party_index];
        uint8_t pkmn_level = pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.level;

        // Get the pokemon's EVs
        struct pksav_gen2_pc_pokemon pkmn_ev_data = pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data;
        const uint16_t pkmn_evs[PKSAV_NUM_GB_IVS] = {
            [PKSAV_GB_IV_ATTACK] = pksav_bigendian16(pkmn_ev_data.ev_atk),
            [PKSAV_GB_IV_DEFENSE] = pksav_bigendian16(pkmn_ev_data.ev_def),
            [PKSAV_GB_IV_SPEED] = pksav_bigendian16(pkmn_ev_data.ev_spd),
            [PKSAV_GB_IV_SPECIAL] = pksav_bigendian16(pkmn_ev_data.ev_spcl),
            [PKSAV_GB_IV_HP] = pksav_bigendian16(pkmn_ev_data.ev_hp)};

        uint8_t pkmn_stats[PKSAV_GEN2_STAT_COUNT] = {
            // Calculate the pokemon's stats
            [PKSAV_GEN2_STAT_ATTACK] = calculate_stat(pkmn_level, pkmn_base_stats_gen2[species_index].atk, pkmn_dvs[PKSAV_GB_IV_ATTACK], pkmn_evs[PKSAV_GB_IV_ATTACK]),
            [PKSAV_GEN2_STAT_DEFENSE] = calculate_stat(pkmn_level, pkmn_base_stats_gen2[species_index].def, pkmn_dvs[PKSAV_GB_IV_DEFENSE], pkmn_evs[PKSAV_GB_IV_DEFENSE]),
            [PKSAV_GEN2_STAT_SPEED] = calculate_stat(pkmn_level, pkmn_base_stats_gen2[species_index].spd, pkmn_dvs[PKSAV_GB_IV_SPEED], pkmn_evs[PKSAV_GB_IV_SPEED]),
            [PKSAV_GEN2_STAT_SPATK] = calculate_stat(pkmn_level, pkmn_base_stats_gen2[species_index].spatk, pkmn_dvs[PKSAV_GB_IV_SPECIAL], pkmn_evs[PKSAV_GB_IV_SPECIAL]),
            [PKSAV_GEN2_STAT_SPDEF] = calculate_stat(pkmn_level, pkmn_base_stats_gen2[species_index].spdef, pkmn_dvs[PKSAV_GB_IV_SPECIAL], pkmn_evs[PKSAV_GB_IV_SPECIAL]),
            // Calculate the pokemon's HP stat
            [PKSAV_GEN2_STAT_HP] = calculate_hp(pkmn_level, pkmn_base_stats_gen2[species_index].max_hp, pkmn_dvs[PKSAV_GB_IV_HP], pkmn_evs[PKSAV_GB_IV_HP])};

        // Update the pokemon's stats
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.atk = pksav_bigendian16(pkmn_stats[PKSAV_GEN2_STAT_ATTACK]);
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.def = pksav_bigendian16(pkmn_stats[PKSAV_GEN2_STAT_DEFENSE]);
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.spd = pksav_bigendian16(pkmn_stats[PKSAV_GEN2_STAT_SPEED]);
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.spatk = pksav_bigendian16(pkmn_stats[PKSAV_GEN2_STAT_SPATK]);
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.spdef = pksav_bigendian16(pkmn_stats[PKSAV_GEN2_STAT_SPDEF]);
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].party_data.max_hp = pksav_bigendian16(pkmn_stats[PKSAV_GEN2_STAT_HP]);
    }
}

bool is_move_HM(uint8_t move_index)
{
    return move_index == MOVE_INDEX_HM01 || move_index == MOVE_INDEX_HM02 || move_index == MOVE_INDEX_HM03 || move_index == MOVE_INDEX_HM04 || move_index == MOVE_INDEX_HM05 || move_index == MOVE_INDEX_HM06;
}

enum eligible_trade_status check_trade_eligibility(struct TrainerInfo *trainer, uint8_t pkmn_party_index)
{
    // All Gen1 saves are eligible for trade except with HM moves
    if (trainer->trainer_generation == SAVE_GENERATION_1)
    {
        for (int move_index = 0; move_index < 4; move_index++)
        {
            uint8_t move = trainer->pokemon_party.gen1_pokemon_party.party[pkmn_party_index].pc_data.moves[move_index];
            if (is_move_HM(move))
            {
                return E_TRADE_STATUS_HM_MOVE;
            }
        }
        return E_TRADE_STATUS_ELIGIBLE;
    }

    // Prevent Gen2 only pkmn from going to Gen1
    uint8_t gen2_species = trainer->pokemon_party.gen2_pokemon_party.species[pkmn_party_index];
    if (gen2_species > 151)
    {
        return E_TRADE_STATUS_GEN2_PKMN;
    }

    // Prevent Gen2 only moves from going to Gen1
    struct pksav_gen2_party_pokemon gen2_pkmn = trainer->pokemon_party.gen2_pokemon_party.party[pkmn_party_index];
    for (int i = 0; i < 4; i++)
    {
        uint8_t move_index = gen2_pkmn.pc_data.moves[i];
        if (move_index > MAX_GEN1_MOVE_INDEX)
        {
            return E_TRADE_STATUS_GEN2_MOVE;
        }
        // flag HM moves
        if (is_move_HM(move_index))
        {
            return E_TRADE_STATUS_HM_MOVE;
        }
    }

    return E_TRADE_STATUS_ELIGIBLE;
}

bool check_if_reds_pikachu(const PokemonSave *pkmn_save, const uint8_t pkmn_party_index)
{
    char tmp_otname_pkmn[11];
    pksav_gen1_import_text(pkmn_save->save.gen1_save.pokemon_storage.p_party->otnames[pkmn_party_index], tmp_otname_pkmn, 10);
    char tmp_otname_trainer[11];
    pksav_gen1_import_text(pkmn_save->save.gen1_save.trainer_info.p_name, tmp_otname_trainer, 10);
    return pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.species == SI_PIKACHU &&   // Is a Pikachu
        pkmn_save->save.gen1_save.save_type == PKSAV_GEN1_SAVE_TYPE_YELLOW &&   // Is from Yellow
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.ot_id == pkmn_save->save.gen1_save.trainer_info.p_id &&   // has the same OT ID as the trainer
        strcmp(tmp_otname_pkmn, tmp_otname_trainer) == 0; // // has the same OT name as the trainer
}

void swap_pkmn_at_index_between_saves_cross_gen(PokemonSave *player1_save, PokemonSave *player2_save, uint8_t pkmn_party_index1, uint8_t pkmn_party_index2)
{
    // Since this is cross-gen assign the player by generation
    PokemonSave *player_gen1;
    PokemonSave *player_gen2;

    if (player1_save->save_generation_type == SAVE_GENERATION_1)
    {
        player_gen1 = player1_save;
        player_gen2 = player2_save;
    }
    else
    {
        player_gen1 = player2_save;
        player_gen2 = player1_save;
        // swap the indexes
        uint8_t tmp_index = pkmn_party_index1;
        pkmn_party_index1 = pkmn_party_index2;
        pkmn_party_index2 = tmp_index;
    }

    // swap nickname
    char tmp_nickname1[11];
    char tmp_nickname2[11];
    pksav_gen1_import_text(player_gen1->save.gen1_save.pokemon_storage.p_party->nicknames[pkmn_party_index1], tmp_nickname1, 10);
    pksav_gen2_import_text(player_gen2->save.gen2_save.pokemon_storage.p_party->nicknames[pkmn_party_index2], tmp_nickname2, 10);
    pksav_gen1_export_text(tmp_nickname2, player_gen1->save.gen1_save.pokemon_storage.p_party->nicknames[pkmn_party_index1], 10);
    pksav_gen2_export_text(tmp_nickname1, player_gen2->save.gen2_save.pokemon_storage.p_party->nicknames[pkmn_party_index2], 10);
    player_gen1->save.gen1_save.pokemon_storage.p_party->nicknames[pkmn_party_index1][strlen(tmp_nickname2)] = 0x50;
    player_gen2->save.gen2_save.pokemon_storage.p_party->nicknames[pkmn_party_index2][strlen(tmp_nickname1)] = 0x50;

    // swap party
    // party_data
    struct pksav_gen1_party_pokemon tmp_pokemon_gen1 = player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1];
    struct pksav_gen2_party_pokemon tmp_pokemon_gen2 = player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2];

    // gen1 -> gen2 party_data
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.level = tmp_pokemon_gen1.party_data.level;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].party_data.max_hp = tmp_pokemon_gen1.party_data.max_hp;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].party_data.atk = tmp_pokemon_gen1.party_data.atk;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].party_data.def = tmp_pokemon_gen1.party_data.def;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].party_data.spd = tmp_pokemon_gen1.party_data.spd;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].party_data.spatk = tmp_pokemon_gen1.party_data.spcl;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].party_data.spdef = tmp_pokemon_gen1.party_data.spcl;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].party_data.condition = PKSAV_CONDITION_NONE;

    // gen2 -> gen1 party_data
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].party_data.level = tmp_pokemon_gen2.pc_data.level;
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].party_data.max_hp = tmp_pokemon_gen2.party_data.max_hp;
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].party_data.atk = tmp_pokemon_gen2.party_data.atk;
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].party_data.def = tmp_pokemon_gen2.party_data.def;
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].party_data.spd = tmp_pokemon_gen2.party_data.spd;
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].party_data.spcl = tmp_pokemon_gen2.party_data.spatk;

    // pc_data
    // gen1 -> gen2 pc_data
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.species = species_gen1_to_gen2[tmp_pokemon_gen1.pc_data.species];
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.ot_id = tmp_pokemon_gen1.pc_data.ot_id;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.exp[0] = tmp_pokemon_gen1.pc_data.exp[0];
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.exp[1] = tmp_pokemon_gen1.pc_data.exp[1];
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.exp[2] = tmp_pokemon_gen1.pc_data.exp[2];
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.ev_hp = tmp_pokemon_gen1.pc_data.ev_hp;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.ev_atk = tmp_pokemon_gen1.pc_data.ev_atk;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.ev_def = tmp_pokemon_gen1.pc_data.ev_def;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.ev_spd = tmp_pokemon_gen1.pc_data.ev_spd;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.ev_spcl = tmp_pokemon_gen1.pc_data.ev_spcl;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.iv_data = tmp_pokemon_gen1.pc_data.iv_data;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.move_pps[0] = tmp_pokemon_gen1.pc_data.move_pps[0];
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.move_pps[1] = tmp_pokemon_gen1.pc_data.move_pps[1];
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.move_pps[2] = tmp_pokemon_gen1.pc_data.move_pps[2];
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.move_pps[3] = tmp_pokemon_gen1.pc_data.move_pps[3];
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.moves[0] = tmp_pokemon_gen1.pc_data.moves[0];
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.moves[1] = tmp_pokemon_gen1.pc_data.moves[1];
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.moves[2] = tmp_pokemon_gen1.pc_data.moves[2];
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.moves[3] = tmp_pokemon_gen1.pc_data.moves[3];

    // gen2 -> gen1 pc_data
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.species = species_gen2_to_gen1[tmp_pokemon_gen2.pc_data.species];
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.level = tmp_pokemon_gen2.pc_data.level;
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.condition = PKSAV_CONDITION_NONE;
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.moves[0] = tmp_pokemon_gen2.pc_data.moves[0];
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.moves[1] = tmp_pokemon_gen2.pc_data.moves[1];
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.moves[2] = tmp_pokemon_gen2.pc_data.moves[2];
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.moves[3] = tmp_pokemon_gen2.pc_data.moves[3];
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.ot_id = tmp_pokemon_gen2.pc_data.ot_id;
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.exp[0] = tmp_pokemon_gen2.pc_data.exp[0];
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.exp[1] = tmp_pokemon_gen2.pc_data.exp[1];
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.exp[2] = tmp_pokemon_gen2.pc_data.exp[2];
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.ev_hp = tmp_pokemon_gen2.pc_data.ev_hp;
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.ev_atk = tmp_pokemon_gen2.pc_data.ev_atk;
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.ev_def = tmp_pokemon_gen2.pc_data.ev_def;
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.ev_spd = tmp_pokemon_gen2.pc_data.ev_spd;
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.ev_spcl = tmp_pokemon_gen2.pc_data.ev_spcl;
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.iv_data = tmp_pokemon_gen2.pc_data.iv_data;
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.move_pps[0] = tmp_pokemon_gen2.pc_data.move_pps[0];
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.move_pps[1] = tmp_pokemon_gen2.pc_data.move_pps[1];
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.move_pps[2] = tmp_pokemon_gen2.pc_data.move_pps[2];
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.move_pps[3] = tmp_pokemon_gen2.pc_data.move_pps[3];

    // data not found in gen1
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.held_item = tmp_pokemon_gen1.pc_data.catch_rate;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.caught_data = (uint16_t)0;
    bool is_reds_pikachu_from_yellow = check_if_reds_pikachu(player_gen1, pkmn_party_index1);
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.friendship = is_reds_pikachu_from_yellow ? GEN2_FRIENDSHIP_BASE_REDS_PIKACHU : GEN2_FRIENDSHIP_BASE;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].pc_data.pokerus = (uint8_t)0;

    // data not found in gen2
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.types[0] = pkmn_base_stats_gen1[player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.species].types[0];
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.types[1] = pkmn_base_stats_gen1[player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.species].types[1];
    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.catch_rate = pkmn_base_stats_gen1[player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.species].catch_rate;

    // swap species
    uint8_t tmp_species_gen1 = player_gen1->save.gen1_save.pokemon_storage.p_party->species[pkmn_party_index1];
    uint8_t tmp_species_gen2 = player_gen2->save.gen2_save.pokemon_storage.p_party->species[pkmn_party_index2];

    // swap the indexes
    player_gen1->save.gen1_save.pokemon_storage.p_party->species[pkmn_party_index1] = species_gen2_to_gen1[tmp_species_gen2];
    player_gen2->save.gen2_save.pokemon_storage.p_party->species[pkmn_party_index2] = species_gen1_to_gen2[tmp_species_gen1];

    // swap otnames
    char tmp_otname_gen1[8];
    char tmp_otname_gen2[8];
    pksav_gen1_import_text(player_gen1->save.gen1_save.pokemon_storage.p_party->otnames[pkmn_party_index1], tmp_otname_gen1, 7);
    pksav_gen2_import_text(player_gen2->save.gen2_save.pokemon_storage.p_party->otnames[pkmn_party_index2], tmp_otname_gen2, 7);
    pksav_gen1_export_text(tmp_otname_gen2, player_gen1->save.gen1_save.pokemon_storage.p_party->otnames[pkmn_party_index1], 7);
    pksav_gen2_export_text(tmp_otname_gen1, player_gen2->save.gen2_save.pokemon_storage.p_party->otnames[pkmn_party_index2], 7);
    player_gen1->save.gen1_save.pokemon_storage.p_party->otnames[pkmn_party_index1][strlen(tmp_otname_gen2)] = 0x50;
    player_gen2->save.gen2_save.pokemon_storage.p_party->otnames[pkmn_party_index2][strlen(tmp_otname_gen1)] = 0x50;

    // Generate random DVs and assign them to the traded pokemen
    update_pkmn_DVs(player_gen1, pkmn_party_index1);
    update_pkmn_DVs(player_gen2, pkmn_party_index2);

    // Update each pokemon's stats based on its level, base stats, DVs, and EVs
    update_pkmn_stats(player_gen1, pkmn_party_index1);
    update_pkmn_stats(player_gen2, pkmn_party_index2);

    player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].pc_data.current_hp = player_gen1->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1].party_data.max_hp;
    player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].party_data.current_hp = player_gen2->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2].party_data.max_hp;
}

void swap_pkmn_at_index_between_saves(PokemonSave *player1_save, PokemonSave *player2_save, uint8_t pkmn_party_index1, uint8_t pkmn_party_index2)
{
    // swap nickname
    char tmp_nickname1[11];
    char tmp_nickname2[11];
    if (player1_save->save_generation_type == SAVE_GENERATION_1)
    {
        pksav_gen1_import_text(player1_save->save.gen1_save.pokemon_storage.p_party->nicknames[pkmn_party_index1], tmp_nickname1, 10);
        pksav_gen1_import_text(player2_save->save.gen1_save.pokemon_storage.p_party->nicknames[pkmn_party_index2], tmp_nickname2, 10);
        pksav_gen1_export_text(tmp_nickname2, player1_save->save.gen1_save.pokemon_storage.p_party->nicknames[pkmn_party_index1], 10);
        pksav_gen1_export_text(tmp_nickname1, player2_save->save.gen1_save.pokemon_storage.p_party->nicknames[pkmn_party_index2], 10);
        player1_save->save.gen1_save.pokemon_storage.p_party->nicknames[pkmn_party_index1][strlen(tmp_nickname2)] = 0x50;
        player2_save->save.gen1_save.pokemon_storage.p_party->nicknames[pkmn_party_index2][strlen(tmp_nickname1)] = 0x50;
    }
    else
    {
        pksav_gen2_import_text(player1_save->save.gen2_save.pokemon_storage.p_party->nicknames[pkmn_party_index1], tmp_nickname1, 10);
        pksav_gen2_import_text(player2_save->save.gen2_save.pokemon_storage.p_party->nicknames[pkmn_party_index2], tmp_nickname2, 10);
        pksav_gen2_export_text(tmp_nickname2, player1_save->save.gen2_save.pokemon_storage.p_party->nicknames[pkmn_party_index1], 10);
        pksav_gen2_export_text(tmp_nickname1, player2_save->save.gen2_save.pokemon_storage.p_party->nicknames[pkmn_party_index2], 10);
        player1_save->save.gen2_save.pokemon_storage.p_party->nicknames[pkmn_party_index1][strlen(tmp_nickname2)] = 0x50;
        player2_save->save.gen2_save.pokemon_storage.p_party->nicknames[pkmn_party_index2][strlen(tmp_nickname1)] = 0x50;
    }

    // swap party
    if (player1_save->save_generation_type == SAVE_GENERATION_1)
    {
        struct pksav_gen1_party_pokemon tmp_pokemon = player1_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1];
        player1_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index1] = player2_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index2];
        player2_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index2] = tmp_pokemon;
    }
    else
    {
        struct pksav_gen2_party_pokemon tmp_pokemon = player1_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index1];
        player1_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index1] = player2_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2];
        player2_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index2] = tmp_pokemon;
    }

    // swap species
    if (player1_save->save_generation_type == SAVE_GENERATION_1)
    {
        uint8_t tmp_species = player1_save->save.gen1_save.pokemon_storage.p_party->species[pkmn_party_index1];
        player1_save->save.gen1_save.pokemon_storage.p_party->species[pkmn_party_index1] = player2_save->save.gen1_save.pokemon_storage.p_party->species[pkmn_party_index2];
        player2_save->save.gen1_save.pokemon_storage.p_party->species[pkmn_party_index2] = tmp_species;
    }
    else
    {
        uint8_t tmp_species = player1_save->save.gen2_save.pokemon_storage.p_party->species[pkmn_party_index1];
        player1_save->save.gen2_save.pokemon_storage.p_party->species[pkmn_party_index1] = player2_save->save.gen2_save.pokemon_storage.p_party->species[pkmn_party_index2];
        player2_save->save.gen2_save.pokemon_storage.p_party->species[pkmn_party_index2] = tmp_species;
    }

    // swap otnames
    char tmp_otname1[8];
    char tmp_otname2[8];
    if (player1_save->save_generation_type == SAVE_GENERATION_1)
    {
        pksav_gen1_import_text(player1_save->save.gen1_save.pokemon_storage.p_party->otnames[pkmn_party_index1], tmp_otname1, 7);
        pksav_gen1_import_text(player2_save->save.gen1_save.pokemon_storage.p_party->otnames[pkmn_party_index2], tmp_otname2, 7);
        pksav_gen1_export_text(tmp_otname2, player1_save->save.gen1_save.pokemon_storage.p_party->otnames[pkmn_party_index1], 7);
        pksav_gen1_export_text(tmp_otname1, player2_save->save.gen1_save.pokemon_storage.p_party->otnames[pkmn_party_index2], 7);
        player1_save->save.gen1_save.pokemon_storage.p_party->otnames[pkmn_party_index1][strlen(tmp_otname2)] = 0x50;
        player2_save->save.gen1_save.pokemon_storage.p_party->otnames[pkmn_party_index2][strlen(tmp_otname1)] = 0x50;
    }
    else
    {
        pksav_gen2_import_text(player1_save->save.gen2_save.pokemon_storage.p_party->otnames[pkmn_party_index1], tmp_otname1, 7);
        pksav_gen2_import_text(player2_save->save.gen2_save.pokemon_storage.p_party->otnames[pkmn_party_index2], tmp_otname2, 7);
        pksav_gen2_export_text(tmp_otname2, player1_save->save.gen2_save.pokemon_storage.p_party->otnames[pkmn_party_index1], 7);
        pksav_gen2_export_text(tmp_otname1, player2_save->save.gen2_save.pokemon_storage.p_party->otnames[pkmn_party_index2], 7);
        player1_save->save.gen2_save.pokemon_storage.p_party->otnames[pkmn_party_index1][strlen(tmp_otname2)] = 0x50;
        player2_save->save.gen2_save.pokemon_storage.p_party->otnames[pkmn_party_index2][strlen(tmp_otname1)] = 0x50;
    }

    // Generate random DVs and assign them to the traded pokemen
    update_pkmn_DVs(player1_save, pkmn_party_index1);
    update_pkmn_DVs(player2_save, pkmn_party_index2);

    // Update each pokemon's stats based on its level, base stats, DVs, and EVs
    update_pkmn_stats(player1_save, pkmn_party_index1);
    update_pkmn_stats(player2_save, pkmn_party_index2);
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
        strcpy(evolution_name, pkmn_evolution_pairs_gen1[pkmn_species_index].evolution_name);
        char species_name[11];
        strcpy(species_name, pkmn_evolution_pairs_gen1[pkmn_species_index].species_name);
        uint8_t evolution_index = pkmn_evolution_pairs_gen1[pkmn_species_index].evolution_index;

        // Update species index to evolution index to access evolution base stats
        pkmn_save->save.gen1_save.pokemon_storage.p_party->species[pkmn_party_index] = evolution_index;
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.species = evolution_index;

        // Calculates and updates stats
        update_pkmn_stats(pkmn_save, pkmn_party_index);

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

        // Update types
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.types[0] = pkmn_base_stats_gen1[evolution_index].types[0];
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.types[1] = pkmn_base_stats_gen1[evolution_index].types[1];
        // Update catch rate
        pkmn_save->save.gen1_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.catch_rate = pkmn_base_stats_gen1[evolution_index].catch_rate;
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
        strcpy(evolution_name, pkmn_evolution_pairs_gen2[pkmn_species_index].evolution_name);
        char species_name[11];
        strcpy(species_name, pkmn_evolution_pairs_gen2[pkmn_species_index].species_name);
        uint8_t evolution_index = pkmn_evolution_pairs_gen2[pkmn_species_index].evolution_index;

        // Update species index to evolution index to access evolution base stats
        pkmn_save->save.gen2_save.pokemon_storage.p_party->species[pkmn_party_index] = evolution_index;
        pkmn_save->save.gen2_save.pokemon_storage.p_party->party[pkmn_party_index].pc_data.species = evolution_index;

        // Calculates and updates stats
        update_pkmn_stats(pkmn_save, pkmn_party_index);

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

// Settings getter disable item required for trade
bool get_is_item_required(void)
{
    return item_required_evolutions;
}

// Settings setter disable item required for trade
void set_is_item_required(bool is_required)
{
    item_required_evolutions = is_required;
}
