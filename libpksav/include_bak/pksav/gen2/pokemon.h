/*!
 * @file    pksav/gen2/pokemon.h
 * @ingroup PKSav
 * @brief   Native formats and convenience functions for Pokémon in Generation II games.
 *
 * Copyright (c) 2015-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_POKEMON_H
#define PKSAV_GEN2_POKEMON_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdint.h>
#include <time.h>

/*!
 * @brief The mask for a move's PP in the PP field.
 *
 * Mask the value of one of the indices of pksav_gen2_pc_pokemon_t.move_pps to
 * get the PP of that move.
 */
#define PKSAV_GEN2_MOVE_PP_MASK ((uint8_t)0x3F)

/*!
 * @brief The mask for the number of PP Ups used on a move (0-3).
 *
 * Mask the value of one of the indices of pksav_gen2_pc_pokemon_t.move_pps
 * and shift it right by ::PKSAV_GEN2_MOVE_PP_UP_MASK to get the number of PP
 * Ups used.
 */
#define PKSAV_GEN2_MOVE_PP_UP_MASK ((uint8_t)0xC0)

/*!
 * @brief The offset of the number of PP UPs used on a move.
 *
 * Mask the value of one of the indices of pksav_gen2_pc_pokemon_t.move_pps
 * with ::PKSAV_GEN2_MOVE_PP_UP_OFFSET and shift it right by this to get the
 * number of PP Ups used.
 */
#define PKSAV_GEN2_MOVE_PP_UP_OFFSET 6

/*!
 * @brief The mask for the level at which a Pokémon was caught.
 *
 * Mask pksav_gen2_pc_pokemon_t.caught_data with this mask and shift it
 * right by ::PKSAV_GEN2_LEVEL_CAUGHT_OFFSET to get the value.
 */
#define PKSAV_GEN2_LEVEL_CAUGHT_MASK   ((uint16_t)0x3F00)

/*!
 * @brief The offset for the level at which a Pokémon was caught.
 *
 * Mask pksav_gen2_pc_pokemon_t.caught_data with ::PKSAV_GEN2_LEVEL_CAUGHT_MASK
 * and shift it right by this offset to get the value.
 */
#define PKSAV_GEN2_LEVEL_CAUGHT_OFFSET 8

/*!
 * @brief The mask for setting a Pokémon's original trainer's gender.
 *
 * Mask pksav_gen2_pc_pokemon_t.caught_data with this mask to set the trainer's
 * gender to female. Unmask it to set the trainer's gender to male.
 */
#define PKSAV_GEN2_OT_GENDER_MASK ((uint16_t)0x0080)

/*!
 * @brief The mask for the index of the location at which the Pokémon was caught.
 *
 * Mask pksav_gen2_pc_pokemon_t.caught_data with this mask to get or set the
 * location index.
 */
#define PKSAV_GEN2_LOCATION_MASK ((uint16_t)0x007F)

#pragma pack(push,1)

/*!
 * @brief Native format for the persistent data of a Pokémon in Generation IIII.
 *
 * This data is available both when the Pokémon is in the trainer's party or in the
 * PC.
 */
typedef struct {
    /*!
     * @brief Species index.
     *
     * This value is the same whether or not the Pokémon is in an egg. If this is
     * the case, the value is reflected in pksav_gen2_pokemon_party_t.species or
     * pksav_gen2_pokemon_box_t.species.
     */
    uint8_t species;
    /*!
     * @brief Held item index.
     *
     * When a Generation I Pokémon is traded into a Generation II game, the
     * pksav_gen2_pc_pokemon_t.catch_rate field is placed here. As such, there is
     * a set held item corresponding to each species when trading up.
     */
    uint8_t held_item;
    /*!
     * @brief Indices for each of this Pokémon's moves.
     */
    uint8_t moves[4];
    /*!
     * @brief The Pokémon's original trainer's ID (stored in big-endian).
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t ot_id;
    /*!
     * @brief The Pokémon's current total experience points (stored in Base-256).
     *
     * This value should be accessed with ::pksav_from_base256 (with a num_bytes
     * value of 3) and set with ::pksav_to_base256.
     */
    uint8_t exp[3];
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
     * The Special stat was removed in Generation II, but this field is kept in
     * for compatibility with Generation I.
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
     * Mask an index with ::PKSAV_GEN2_MOVE_PP_MASK to get the move PP.
     *
     * Mask an index with ::PKSAV_GEN2_MOVE_PP_UP_MASK to get the number of
     * PP Ups applied to the move.
     */
    uint8_t move_pps[4];
    /*!
     * @brief A Pokémon's friendship/happiness value.
     *
     * This value is used in certain in-game events and is used to determine the power
     * of Return and Frustration.
     */
    uint8_t friendship;
    /*!
     * @brief The Pokémon's Pokérus strain and duration.
     *
     * See <pksav/common/pokerus.h> for more details.
     */
    uint8_t pokerus;
    /*!
     * @brief Data about where and when a Pokémon was caught.
     *
     * The bitfield is arranged as follows:
     *  * 0-6: location caught
     *  * 7: 0 if original trainer is male, 1 if original trainer is female
     *  * 8-13: level caught
     *  * 14-15: time caught
     *    * 1: 4:00 AM - 9:59 AM
     *    * 2: 10:00 AM - 5:59 PM
     *    * 3: 6:00 PM - 3:59 AM
     *
     * This data is only set in Crystal. Pokémon caught in Gold/Silver or traded up from
     * Generation I will have this value set to 0.
     */
    uint16_t caught_data;
    /*!
     * @brief The Pokémon's level.
     */
    uint8_t level;
} pksav_gen2_pc_pokemon_t;

/*!
 * @brief Data generated when a Pokémon is added to a trainer's party in Generation III.
 *
 * All of this information is generated from values stored in
 * pksav_gen2_pc_pokemon_t.
 */
typedef struct {
    /*!
     * @brief The Pokémon's status ailments, if any.
     *
     * The enum ::pksav_gb_condition_t contains all valid values for this field.
     */
    uint8_t condition;
    //! Unused.
    uint8_t unused;
    /*!
     * @brief The Pokémon's current HP.
     *
     * This value should never be higher than pksav_gen2_pokemon_party_data_t.max_hp.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t current_hp;
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
     * @brief The Pokémon's Attack.
     *
     * This value is calculated using the Pokémon's base Attack stat, Attack
     * EV, and Attack IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t atk;
    /*!
     * @brief The Pokémon's Defense.
     *
     * This value is calculated using the Pokémon's base Defense stat, Defense
     * EV, and Defense IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t def;
    /*!
     * @brief The Pokémon's Speed.
     *
     * This value is calculated using the Pokémon's base Speed stat, Speed
     * EV, and Speed IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t spd;
    /*!
     * @brief The Pokémon's Special Attack.
     *
     * This value is calculated using the Pokémon's base Special Attack stat,
     * Special EV, and Special IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t spatk;
    /*!
     * @brief The Pokémon's Special Defense.
     *
     * This value is calculated using the Pokémon's base Special Defense stat,
     * Special EV, and Special IV.
     *
     * This value should be accessed and set with ::pksav_bigendian16.
     */
    uint16_t spdef;
} pksav_gen2_pokemon_party_data_t;

//! Native format for a Pokémon in the trainer's party in Generation III.
typedef struct {
    /*!
     * @brief PC data.
     *
     * This data is accessible whether the Pokémon is in the PC or party.
     */
    pksav_gen2_pc_pokemon_t pc;

    /*!
     * @brief Party data.
     *
     * This data is generated when the Pokémon is moved into the party. As
     * all of it can be generated from the PC data, it is not stored until
     * it needs to be used.
     */
    pksav_gen2_pokemon_party_data_t party_data;
} pksav_gen2_party_pokemon_t;

//! Native format for a trainer's Pokémon party in Generation II.
typedef struct {
    //! The actual number of Pokémon in the party (0-6).
    uint8_t count;
    /*!
     * @brief The species indices of the Pokémon in the party.
     *
     * When the party is viewed in-game, it is this value that determines
     * what Pokémon is shown, not the pksav_gen2_pc_pokemon_t.species value.
     *
     * If this field is set to 0xFD, the Pokémon will be in an egg. Its field
     * in pksav_gen2_pc_pokemon_t will correspond to its species when hatched.
     *
     * The final index of this field should always be set to 0xFF.
     */
    uint8_t species[7];
    //! The actual Pokémon in the party.
    pksav_gen2_party_pokemon_t party[6];
    /*!
     * @brief The names of each Pokémon's original trainer.
     *
     * To access this value, you should use the function ::pksav_text_from_gen2
     * with a num_chars value of 10.
     */
    uint8_t otnames[6][11];
    /*!
     * @brief The nicknames of each Pokémon in the party.
     *
     * To access this value, you should use the function ::pksav_text_from_gen2
     * with a num_chars value of 10.
     */
    uint8_t nicknames[6][11];
} pksav_gen2_pokemon_party_t;

//! Native format for a Pokémon PC box in Generation II.
typedef struct {
    //! The actual number of Pokémon in the box (0-20).
    uint8_t count;
    /*!
     * @brief The species indices of the Pokémon in the box.
     *
     * When the box is viewed in-game, it is this value that determines
     * what Pokémon is showed, not the pksav_gen2_pc_pokemon_t.species value.
     *
     * If this field is set to 0xFD, the Pokémon will be in an egg. Its field
     * in pksav_gen2_pc_pokemon_t will correspond to its species when hatched.
     *
     * The final index of this field should always be set to 0xFF.
     */
    uint8_t species[21];
    //! The actual Pokémon in the box.
    pksav_gen2_pc_pokemon_t entries[20];
    /*!
     * @brief The names of each Pokémon's original trainer.
     *
     * To access this value, you should use the function ::pksav_text_from_gen2
     * with a num_chars value of 10.
     */
    uint8_t otnames[20][11];
    /*!
     * @brief The nicknames of each Pokémon in the box.
     *
     * To access this value, you should use the function ::pksav_text_from_gen2
     * with a num_chars value of 10.
     */
    uint8_t nicknames[20][11];
    //! Padding.
    uint8_t padding[2];
} pksav_gen2_pokemon_box_t;

//! List of Pokémon box names in Generation II.
typedef struct {
    /*!
     * @brief Each box name.
     *
     * To access a name, you should use the function ::pksav_text_from_gen2
     * with a num_chars value of 9.
     */
    uint8_t names[14][9];
} pksav_gen2_pokemon_box_names_t;

#pragma pack(pop)

#endif /* PKSAV_GEN2_POKEMON_H */
