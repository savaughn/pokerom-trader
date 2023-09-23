/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "util/text_common.h"

#include <pksav/gba/text.h>

#include <assert.h>
#include <string.h>

#define PKSAV_GBA_LAST_CHAR  0xF9 // Just control characters past here
#define PKSAV_GBA_TERMINATOR 0xFF

/*
 * Character map for Generation III (English)
 *
 * Source: http://bulbapedia.bulbagarden.net/wiki/Character_encoding_in_Generation_III
 *
 * There are no Unicode values for certain characters, so we will use substitutes, as listed below:
 *
 *  * "$" -> Pokemon dollar
 *  * "<" -> "PK"
 *  * ">" -> "MN"
 *  * "*" -> "PO"
 *  * "~" -> "KE"
 *
 *  Any application displaying this text will need to graphically substitute in these characters.
 */

static const wchar_t PKSAV_GBA_CHAR_MAP[] =
{
    0x0020,0x00C0,0x00C1,0x00C2,0x00C7,0x00C8,0x00C9,0x00CA,0x00CB,0x00CC,0x3053,0x00CD,0x00CE,0x00D2,0x00D3,0x00D4,
    0x0152,0x00D9,0x00DA,0x00DB,0x00D1,0x1E9E,0x00E0,0x00E1,0x306D,0x00E7,0x00E8,0x00E9,0x00EA,0x00EB,0x00EC,0x307E,
    0x00EE,0x00EF,0x00F2,0x00F3,0x00F4,0x0152,0x00F9,0x00FA,0x00FB,0x00F1,0x00BA,0x00AA,0x003F,0x0026,0x002B,0x3042,
    0x3043,0x3045,0x3047,0x3049,0x002F,0x003D,0x3087,0x304C,0x304E,0x3050,0x3052,0x3054,0x3056,0x3058,0x305A,0x305C,
    0x305E,0x3060,0x3062,0x3065,0x3067,0x3069,0x3070,0x3073,0x3076,0x3079,0x307C,0x3071,0x3074,0x3077,0x307A,0x307D,
    0x3063,0x00BF,0x00A1,0x003C,0x003E,0x002A,0x007E,0x003F,0x003F,0x003F,0x00CD,0x0025,0x0028,0x0029,0x30BB,0x30BD,
    0x30BF,0x30C1,0x30C4,0x30C6,0x30C8,0x30CA,0x30CB,0x30CC,0x00C2,0x30CE,0x30CF,0x30D2,0x30D5,0x30D8,0x30DB,0x00CD,
    0x30DF,0x30E0,0x30E1,0x30E2,0x30E4,0x30E6,0x30E8,0x30E9,0x30EA,0x2B06,0x2B07,0x2B05,0x27A1,0x30F2,0x30F3,0x30A1,
    0x30A3,0x30A5,0x30A7,0x30A9,0x30E3,0x30E5,0x30E7,0x30AC,0x30AE,0x30B0,0x30B2,0x30B4,0x30B6,0x30B8,0x30BA,0x30BC,
    0x30BE,0x30C0,0x30C2,0x30C5,0x30C7,0x30C9,0x30D0,0x30D3,0x30D6,0x30D9,0x30DC,0x30D1,0x30D4,0x30D7,0x30DA,0x30DD,
    0x30C3,0x0030,0x0031,0x0032,0x0033,0x0034,0x0035,0x0036,0x0037,0x0038,0x0039,0x0021,0x003F,0x002E,0x002D,0x30FB,
    0x2026,0x201C,0x201D,0x2018,0x2019,0x2642,0x2640,0x0024,0x002C,0x00D7,0x002F,0x0041,0x0042,0x0043,0x0044,0x0045,
    0x0046,0x0047,0x0048,0x0049,0x004A,0x004B,0x004C,0x004D,0x004E,0x004F,0x0050,0x0051,0x0052,0x0053,0x0054,0x0055,
    0x0056,0x0057,0x0058,0x0059,0x005A,0x0061,0x0062,0x0063,0x0064,0x0065,0x0066,0x0067,0x0068,0x0069,0x006A,0x006B,
    0x006C,0x006D,0x006E,0x006F,0x0070,0x0071,0x0072,0x0073,0x0074,0x0075,0x0076,0x0077,0x0078,0x0079,0x007A,0x25B6,
    0x003A,0x00C4,0x00D6,0x00DC,0x00E4,0x00F6,0x00F6,0x2B06,0x2B07,0x2B05,'\0','\0','\0','\0','\n','\0'
};
static const size_t PKSAV_GBA_CHAR_MAP_SIZE =
    sizeof(PKSAV_GBA_CHAR_MAP)/sizeof(PKSAV_GBA_CHAR_MAP[0]);

static enum pksav_error _pksav_gba_import_widetext(
    const uint8_t* p_input_buffer,
    wchar_t* p_output_widetext,
    size_t num_chars
)
{
    assert(p_input_buffer != NULL);
    assert(p_output_widetext != NULL);

    memset(p_output_widetext, 0, sizeof(wchar_t)*num_chars);

    for(size_t char_index = 0; char_index < num_chars; ++char_index)
    {
        if(p_input_buffer[char_index] > PKSAV_GBA_LAST_CHAR)
        {
            break;
        }
        else
        {
            p_output_widetext[char_index] = PKSAV_GBA_CHAR_MAP[p_input_buffer[char_index]];
        }
    }

    return PKSAV_ERROR_NONE;
}

static enum pksav_error _pksav_gba_export_widetext(
    const wchar_t* p_input_widetext,
    uint8_t* p_output_buffer,
    size_t num_chars
)
{
    assert(p_input_widetext != NULL);
    assert(p_output_buffer != NULL);

    memset(p_output_buffer, PKSAV_GBA_TERMINATOR, num_chars);

    for(size_t char_index = 0; char_index < num_chars; ++char_index)
    {
        ssize_t map_index = wchar_map_index(
                                PKSAV_GBA_CHAR_MAP,
                                PKSAV_GBA_CHAR_MAP_SIZE,
                                p_input_widetext[char_index]
                            );
        if(map_index != -1)
        {
            p_output_buffer[char_index] = (uint8_t)map_index;
        }
        else
        {
            break;
        }
    }

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_gba_import_text(
    const uint8_t* p_input_buffer,
    char* p_output_text,
    size_t num_chars
)
{
    if(!p_input_buffer || !p_output_text)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    wchar_t* p_widetext = calloc(num_chars, sizeof(wchar_t));
    _pksav_gba_import_widetext(
        p_input_buffer, p_widetext, num_chars
    );

    memset(p_output_text, 0, num_chars);
    pksav_wcstombs(p_output_text, p_widetext, num_chars);
    free(p_widetext);

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_gba_export_text(
    const char* p_input_text,
    uint8_t* p_output_buffer,
    size_t num_chars
)
{
    if(!p_input_text || !p_output_buffer)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    wchar_t* p_widetext = calloc(num_chars, sizeof(wchar_t));
    pksav_mbstowcs(p_widetext, p_input_text, num_chars);

    _pksav_gba_export_widetext(
        p_widetext, p_output_buffer, num_chars
    );

    free(p_widetext);

    return PKSAV_ERROR_NONE;
}
