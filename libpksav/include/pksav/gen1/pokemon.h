/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_POKEMON_H
#define PKSAV_GEN1_POKEMON_H

#include <pksav/gen1/common.h>

#include <pksav/common/constants.h>

#include <stdint.h>

#define PKSAV_GEN1_BOX_NUM_POKEMON   (20)
#define PKSAV_GEN1_PARTY_NUM_POKEMON PKSAV_STANDARD_POKEMON_PARTY_SIZE
#define PKSAV_GEN1_POKEMON_NUM_MOVES PKSAV_STANDARD_POKEMON_NUM_MOVES

#define PKSAV_GEN1_POKEMON_NICKNAME_LENGTH       PKSAV_STANDARD_NICKNAME_LENGTH
#define PKSAV_GEN1_POKEMON_OTNAME_LENGTH         PKSAV_GEN1_TRAINER_NAME_LENGTH
#define PKSAV_GEN1_POKEMON_OTNAME_STORAGE_LENGTH (10)

#define PKSAV_GEN1_POKEMON_EXPERIENCE_BUFFER_SIZE (3)
#define PKSAV_GEN1_POKEMON_NUM_TYPES              (2)

#define PKSAV_GEN1_POKEMON_MOVE_PP_MASK   ((uint8_t)0x3F)
#define PKSAV_GEN1_POKEMON_MOVE_PP(field) ((field) & PKSAV_GEN1_POKEMON_MOVE_PP_MASK)

#define PKSAV_GEN1_POKEMON_MOVE_PP_UP_OFFSET (6)
#define PKSAV_GEN1_POKEMON_MOVE_PP_UP(field) ((field) >> PKSAV_GEN1_POKEMON_MOVE_PP_UP_OFFSET)

#pragma pack(push,1)

/*!
 * @brief Native format for the persistent data of a Pokémon in Generation I.
 *
 * This data is available both when the Pokémon is in the trainer's party or in the
 * PC.
 */
struct pksav_gen1_pc_pokemon
{
    //! Species index.
    uint8_t species;
    /*!
     * @brief The Pokémon's current HP.
     *
     * This value should never be higher than pksav_gen1_pokemon_party_data_t.max_hp.
     */
    uint16_t current_hp;
    /*!
     * @brief The Pokémon's level.
     *
     * This value is also stored in pksav_gen1_pokemon_party_data_t.level.
     */
    uint8_t level;
    /*!
     * @brief The Pokémon's status ailments, if any.
     *
     * The enum ::pksav_gb_condition_t contains all valid values for this field.
     */
    uint8_t condition;
    /*!
     * @brief Indices for each of this Pokémon's types.
     *
     * If the Pokémon has a single type, then both of these values are equal to that
     * type's index.
     *
     * These fields have no real use in a save editing program and should have simply been
     * in a lookup table on the ROM itself.
     *
     * The enum ::pksav_gen1_type_t contains all valid values for this field.
     */
    uint8_t types[PKSAV_GEN1_POKEMON_NUM_TYPES];
    /*!
     * @brief The Pokémon's catch rate.
     *
     * This value has no real use in a save editing program and should have simply been
     * in a lookup table on the ROM itself.
     */
    uint8_t catch_rate;
    /*!
     * @brief Indices for each of this Pokémon's moves.
     */
    uint8_t moves[PKSAV_GEN1_POKEMON_NUM_MOVES];
    /*!
     * @brief The Pokémon's original trainer's ID (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ot_id;
    /*!
     * @brief The Pokémon's current total experience points (stored in Base-256).
     *
     * This value should be accessed with ::pksav_import_base256 (with a num_bytes
     * value of 3) and set with ::pksav_export_base256.
     */
    uint8_t exp[PKSAV_GEN1_POKEMON_EXPERIENCE_BUFFER_SIZE];
    /*!
     * @brief The Pokémon's HP EV stat (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_hp;
    /*!
     * @brief The Pokémon's Attack EV stat (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_atk;
    /*!
     * @brief The Pokémon's Defense EV stat (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_def;
    /*!
     * @brief The Pokémon's Speed EV stat (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_spd;
    /*!
     * @brief The Pokémon's Special EV stat (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ev_spcl;
    /*!
     * @brief The Pokémon's IV's, stored as a bitset.
     *
     * These values should be accessed with ::pksav_get_gb_IV and set
     * with ::pksav_set_gb_IV.
     */
    uint16_t iv_data;
    /*!
     * @brief The Pokémon's PPs and number of PP Ups used for each move.
     *
     * Mask an index with ::PKSAV_GEN1_MOVE_PP_MASK to get the move PP.
     *
     * Mask an index with ::PKSAV_GEN1_MOVE_PP_UP_MASK to get the number of
     * PP Ups applied to the move.
     */
    uint8_t move_pps[PKSAV_GEN1_POKEMON_NUM_MOVES];
};

/*!
 * @brief Data generated when a Pokémon is added to a trainer's party in Generation I.
 *
 * All of this information is generated from values stored in
 * pksav_gen1_pc_pokemon_t.
 */
struct pksav_gen1_pokemon_party_data
{
    /*!
     * @brief The Pokémon's level.
     *
     * This value is also stored in pksav_gen1_pc_pokemon_t.level.
     */
    uint8_t level;
    /*!
     * @brief The Pokémon's maximum HP.
     *
     * This value is calculated using the Pokémon's base HP stat, HP
     * EV, and HP IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t max_hp;
    /*!
     * @brief The Pokémon's Attack stat.
     *
     * This value is calculated using the Pokémon's base Attack stat,
     * Attack EV, and Attack IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t atk;
    /*!
     * @brief The Pokémon's Defense stat.
     *
     * This value is calculated using the Pokémon's base Defense stat,
     * Defense EV, and Defense IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t def;
    /*!
     * @brief The Pokémon's Speed stat.
     *
     * This value is calculated using the Pokémon's base Speed stat,
     * Speed EV, and Speed IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t spd;
    /*!
     * @brief The Pokémon's Special stat.
     *
     * This value is calculated using the Pokémon's base Special stat,
     * Special EV, and Special IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t spcl;
};

//! Native format for a Pokémon in the trainer's party in Generation I.
struct pksav_gen1_party_pokemon
{
    /*!
     * @brief PC data.
     *
     * This data is accessible whether the Pokémon is in the PC or party.
     */
    struct pksav_gen1_pc_pokemon pc_data;

    /*!
     * @brief Party data.
     *
     * This data is generated when the Pokémon is moved into the party. As
     * all of it can be generated from the PC data, it is not stored until
     * it needs to be used.
     */
    struct pksav_gen1_pokemon_party_data party_data;
};

//! Native format for a trainer's Pokémon party in Generation I.
struct pksav_gen1_pokemon_party
{
    //! The actual number of Pokémon in the party (0-6).
    uint8_t count;
    /*!
     * @brief The species indices of the Pokémon in the party.
     *
     * When the party is viewed in-game, it is this value that determines
     * what Pokémon is shown, not the pksav_gen1_pc_pokemon_t.species value.
     *
     * The final index of this field should always be set to 0xFF.
     */
    uint8_t species[PKSAV_GEN1_PARTY_NUM_POKEMON + 1];
    //! The actual Pokémon in the party.
    struct pksav_gen1_party_pokemon party[PKSAV_GEN1_PARTY_NUM_POKEMON];
    /*!
     * @brief The names of each Pokémon's original trainer.
     *
     * To access this value, you should use the function ::pksav_text_from_gen1
     * with a num_chars value of 10.
     */
    uint8_t otnames[PKSAV_GEN1_PARTY_NUM_POKEMON][PKSAV_GEN1_POKEMON_OTNAME_STORAGE_LENGTH + 1];
    /*!
     * @brief The nicknames of each Pokémon in the party.
     *
     * To access this value, you should use the function ::pksav_text_from_gen1
     * with a num_chars value of 10.
     */
    uint8_t nicknames[PKSAV_GEN1_PARTY_NUM_POKEMON][PKSAV_GEN1_POKEMON_NICKNAME_LENGTH + 1];
};

//! Native format for a Pokémon PC box in Generation I.
struct pksav_gen1_pokemon_box
{
    //! The actual number of Pokémon in the box (0-20).
    uint8_t count;
    /*!
     * @brief The species indices of the Pokémon in the box.
     *
     * When the box is viewed in-game, it is this value that determines
     * what Pokémon is shown, not the pksav_gen1_pc_pokemon_t.species value.
     *
     * The first index after the last Pokémon in the box should always be
     * set to 0xFF.
     */
    uint8_t species[PKSAV_GEN1_BOX_NUM_POKEMON + 1];
    //! The actual Pokémon in the box.
    struct pksav_gen1_pc_pokemon entries[PKSAV_GEN1_BOX_NUM_POKEMON];
    /*!
     * @brief The names of each Pokémon's original trainer.
     *
     * To access this value, you should use the function ::pksav_text_from_gen1
     * with a num_chars value of 10.
     */
    uint8_t otnames[PKSAV_GEN1_BOX_NUM_POKEMON][PKSAV_GEN1_POKEMON_OTNAME_STORAGE_LENGTH + 1];
    /*!
     * @brief The nicknames of each Pokémon in the box.
     *
     * To access this value, you should use the function ::pksav_text_from_gen1
     * with a num_chars value of 10).
     */
    uint8_t nicknames[PKSAV_GEN1_BOX_NUM_POKEMON][PKSAV_GEN1_POKEMON_NICKNAME_LENGTH + 1];
};

#pragma pack(pop)

#endif /* PKSAV_GEN1_POKEMON_H */
