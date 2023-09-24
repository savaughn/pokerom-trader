/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GBA_MAIL_H
#define PKSAV_GBA_MAIL_H

#include <pksav/common/trainer_id.h>

#include <stdint.h>

#pragma pack(push,1)

struct pksav_gba_mail
{
    uint16_t words[9];
    uint8_t player_name[8];
    union pksav_trainer_id player_id;
    uint16_t species;
    uint16_t item_id;
};

#pragma pack(pop)

#endif /* PKSAV_GBA_MAIL_H */
