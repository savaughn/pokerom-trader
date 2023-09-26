/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_DAYCARE_H
#define PKSAV_GBA_DAYCARE_H

#include <pksav/common/trainer_id.h>
#include <pksav/gba/mail.h>
#include <pksav/gba/pokemon.h>

#include <stdint.h>

#define PKSAV_GBA_DAYCARE_NUM_POKEMON (2)

#pragma pack(push,1)

//
// Ruby/Sapphire
//

struct pksav_gba_rs_daycare_mail
{
    struct pksav_gba_mail mail;
    uint8_t names[19];
};

struct pksav_gba_rs_daycare
{
    struct pksav_gba_pc_pokemon pokemon[PKSAV_GBA_DAYCARE_NUM_POKEMON];
    struct pksav_gba_rs_daycare_mail mail[PKSAV_GBA_DAYCARE_NUM_POKEMON];

    uint32_t steps[PKSAV_GBA_DAYCARE_NUM_POKEMON];

    // Remaining 16 bits generated when egg taken from daycare.
    uint16_t pending_egg_personality;

    uint8_t remaining_egg_cycle_steps;
};

//
// Emerald/FireRed/LeafGreen
//

struct pksav_gba_emerald_frlg_daycare_mail
{
    struct pksav_gba_mail mail;
    uint8_t otname[8];
    uint8_t nickname[11];
    uint8_t language_info; // TODO: bitfield enum
};

struct pksav_gba_emerald_frlg_daycare_pokemon
{
    struct pksav_gba_pc_pokemon pokemon;
    struct pksav_gba_emerald_frlg_daycare_mail mail;

    uint32_t steps;
};

struct pksav_gba_emerald_frlg_daycare
{
    struct pksav_gba_emerald_frlg_daycare_pokemon pokemon[PKSAV_GBA_DAYCARE_NUM_POKEMON];
    uint32_t egg_personality;
    uint8_t remaining_egg_cycle_steps;
};

//
// Convenience union
//

union pksav_gba_daycare
{
    struct pksav_gba_rs_daycare rs;
    struct pksav_gba_emerald_frlg_daycare emerald_frlg;
};

#pragma pack(pop)

#endif /* PKSAV_GBA_DAYCARE_H */
