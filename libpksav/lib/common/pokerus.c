/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/common/pokerus.h>

enum pksav_error pksav_pokerus_get_strain(
    const uint8_t* p_pokerus,
    enum pksav_pokerus_strain* p_strain_out
)
{
    if(!p_pokerus || !p_strain_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    *p_strain_out = (enum pksav_pokerus_strain)((((*p_pokerus) & PKSAV_POKERUS_STRAIN_MASK) >> 4) % 4);

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_pokerus_set_strain(
    uint8_t* p_pokerus,
    enum pksav_pokerus_strain strain
)
{
    if(!p_pokerus)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    *p_pokerus = (strain << 4) | ((strain % 4) + 1);

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_pokerus_get_duration(
    const uint8_t* p_pokerus,
    uint8_t* p_duration_out
)
{
    if(!p_pokerus || !p_duration_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    *p_duration_out = ((*p_pokerus) & PKSAV_POKERUS_DURATION_MASK);

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_pokerus_set_duration(
    uint8_t* p_pokerus,
    uint8_t duration
)
{
    if(!p_pokerus)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if(duration > 15)
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    *p_pokerus &= ~PKSAV_POKERUS_DURATION_MASK;
    *p_pokerus |= duration;

    // Adjust the strain if the duration is incompatible.
    if(duration >= 12)
    {
        *p_pokerus &= ~PKSAV_POKERUS_STRAIN_MASK;
        *p_pokerus |= ((duration % 4) << 4);
    }

    return PKSAV_ERROR_NONE;
}
