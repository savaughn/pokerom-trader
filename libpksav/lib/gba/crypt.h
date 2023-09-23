/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKSAV_GBA_CRYPT_H
#define PKSAV_GBA_CRYPT_H

#include <pksav/gba/items.h>
#include <pksav/gba/pokemon.h>
#include <pksav/gba/save.h>

#include <stdbool.h>
#include <stdint.h>

void pksav_gba_crypt_pokemon(
    struct pksav_gba_pc_pokemon* p_gba_pokemon,
    bool should_encrypt
);

void pksav_gba_save_crypt_items(
    union pksav_gba_item_bag* p_gba_item_bag,
    uint32_t security_key,
    enum pksav_gba_save_type save_type
);

#endif /* PKSAV_GBA_CRYPT_H */
