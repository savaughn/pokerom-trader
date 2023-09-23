/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_POKEDEX_H
#define PKSAV_COMMON_POKEDEX_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdbool.h>
#include <stdint.h>

#define PKSAV_POKEDEX_BUFFER_SIZE_BYTES(num_pokemon) ((num_pokemon / 8) + 1)

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Check if a given Pokémon has been seen/caught in the Pokédex's native representation.
 *
 * In a seen/caught buffer, each Pokémon is represented by a single bit. This
 * function calculates which bit that is based on the National Pokédex number and
 * checks it.
 *
 * \param raw Pokédex buffer
 * \param pokedex_num Which Pokémon to check
 * \param result_out where result is returned
 * \returns if Pokémon with the given Pokédex number has been seen/caught
 */
PKSAV_API enum pksav_error pksav_get_pokedex_bit(
    const uint8_t* p_buffer,
    uint16_t pokedex_num,
    bool* p_result_out
);

/*!
 * @brief Set whether or not a given Pokémon has been seen/caught.
 *
 * In a seen/caught buffer, each Pokémon is represented by a single bit. This
 * function calculates which bit that is based on the National Pokédex number and
 * checks it.
 *
 * \param raw Pokédex buffer
 * \param pokedex_num Which Pokémon to set or unset
 * \param set Set whether or not Pokémon has been seen/caught
 */
PKSAV_API enum pksav_error pksav_set_pokedex_bit(
    uint8_t* p_buffer,
    uint16_t pokedex_num,
    bool set
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_COMMON_POKEDEX_H */
