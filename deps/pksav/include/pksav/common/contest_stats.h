/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_COMMON_CONTEST_STATS_H
#define PKSAV_COMMON_CONTEST_STATS_H

#include <stdint.h>

#pragma pack(push,1)

//! Native format for storing a Pokémon's contest stats.
struct pksav_contest_stats
{
    //! Coolness.
    uint8_t cool;
    //! Beauty.
    uint8_t beauty;
    //! Cuteness.
    uint8_t cute;
    //! Smartness.
    uint8_t smart;
    //! Toughness.
    uint8_t tough;
    union
    {
        //! Feel (Generation IV only).
        uint8_t feel;
        //! Sheen (Generation V+).
        uint8_t sheen;
    };
};

#pragma pack(pop)

#endif /* PKSAV_COMMON_CONTEST_STATS_H */
