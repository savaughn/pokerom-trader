/*!
 * @file    pksav/common/pokerus.h
 * @ingroup PKSav
 * @brief   Functions for getting and setting Pokérus values.
 *
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_POKERUS_H
#define PKSAV_COMMON_POKERUS_H

#include <pksav/config.h>

#include <stdint.h>

typedef enum {
    PKSAV_POKERUS_STRAIN_A = 0,
    PKSAV_POKERUS_STRAIN_B,
    PKSAV_POKERUS_STRAIN_C,
    PKSAV_POKERUS_STRAIN_D
} pksav_pokerus_strain_t;

#define PKSAV_POKERUS_STRAIN_MASK   ((uint8_t)0xF0)
#define PKSAV_POKERUS_DURATION_MASK ((uint8_t)0x0F)

#ifdef __cplusplus
extern "C" {
#endif

static PKSAV_INLINE pksav_error_t pksav_pokerus_get_strain(
    const uint8_t* pokerus_ptr,
    pksav_pokerus_strain_t* strain_out
)
{
    if(!pokerus_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if(!strain_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    *strain_out = (pksav_pokerus_strain_t)((((*pokerus_ptr) & PKSAV_POKERUS_STRAIN_MASK) >> 4) % 4);

    return PKSAV_ERROR_NONE;
}

static PKSAV_INLINE pksav_error_t pksav_pokerus_set_strain(
    uint8_t* pokerus_ptr,
    pksav_pokerus_strain_t strain
)
{
    if(!pokerus_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    *pokerus_ptr = (strain << 4) | ((strain % 4) + 1);

    return PKSAV_ERROR_NONE;
}

static PKSAV_INLINE pksav_error_t pksav_pokerus_get_duration(
    const uint8_t* pokerus_ptr,
    uint8_t* duration_out
)
{
    if(!pokerus_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if(!duration_out)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    *duration_out = ((*pokerus_ptr) & PKSAV_POKERUS_DURATION_MASK);

    return PKSAV_ERROR_NONE;
}

static PKSAV_INLINE pksav_error_t pksav_pokerus_set_duration(
    uint8_t* pokerus_ptr,
    uint8_t duration
)
{
    if(!pokerus_ptr)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }
    if(duration > 15)
    {
        return PKSAV_ERROR_PARAM_OUT_OF_RANGE;
    }

    *pokerus_ptr &= ~PKSAV_POKERUS_DURATION_MASK;
    *pokerus_ptr |= duration;

    // Adjust the strain if the duration is incompatible.
    if(duration >= 12)
    {
        *pokerus_ptr &= ~PKSAV_POKERUS_STRAIN_MASK;
        *pokerus_ptr |= ((duration % 4) << 4);
    }

    return PKSAV_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_COMMON_POKERUS_H */
