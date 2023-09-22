/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pksav/gen2/time.h>

pksav_error_t pksav_gen2_set_caught_data_time_field(
    const time_t* time_in,
    uint16_t* caught_data
) {
    if(!time_in || !caught_data) {
        return PKSAV_ERROR_NULL_POINTER;
    }

    (*caught_data) &= ~PKSAV_GEN2_TIME_OF_DAY_MASK;

    struct tm* gmtm = localtime(time_in);
    if(gmtm->tm_hour >= 2 && gmtm->tm_hour <= 8) {
        (*caught_data) |= (PKSAV_GEN2_MORNING << PKSAV_GEN2_TIME_OF_DAY_OFFSET);
    } else if(gmtm->tm_hour >= 9 && gmtm->tm_hour <= 16) {
        (*caught_data) |= (PKSAV_GEN2_DAY << PKSAV_GEN2_TIME_OF_DAY_OFFSET);
    } else {
        (*caught_data) |= (PKSAV_GEN2_NIGHT << PKSAV_GEN2_TIME_OF_DAY_OFFSET);
    }

    return PKSAV_ERROR_NONE;
}
