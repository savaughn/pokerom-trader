/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN1_TEXT_H
#define PKSAV_GEN1_TEXT_H

#include <pksav/config.h>
#include <pksav/error.h>

#include <stdint.h>
#include <stdlib.h>

#define PKSAV_GEN1_TEXT_TERMINATOR (0x50)

#ifdef __cplusplus
extern "C" {
#endif

//! Convert a string from Generation I format to a UTF-8 C string
/*!
 * In Generation I games, strings are stored with a proprietary character
 * map.
 *
 * Both buffers passed into the function must be at least the length passed
 * in the num_chars parameter, or it will result in undefined behavior.
 *
 * \param input_buffer Generation I string
 * \param output_text output buffer in which to place converted text
 * \param num_chars the number of characters to convert
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_ERROR_NULL_POINTER if input_buffer or output_text is NULL
 */
PKSAV_API enum pksav_error pksav_gen1_import_text(
    const uint8_t* p_input_buffer,
    char* p_output_text,
    size_t num_chars
);

//! Convert a UTF-8 C string to Generation I format
/*!
 * In Generation I games, strings are stored with a proprietary character
 * map.
 *
 * Both buffers passed into the function must be at least the length passed
 * in the num_chars parameter, or it will result in undefined behavior.
 *
 * \param input_text C string to convert
 * \param output_buffer output buffer in which to place Generation I string
 * \param num_chars the number of characters to convert
 * \returns PKSAV_ERROR_NONE upon success
 * \returns PKSAV_ERROR_NULL_POINTER if input_text or output_buffer is NULL
 */
PKSAV_API enum pksav_error pksav_gen1_export_text(
    const char* p_input_text,
    uint8_t* p_output_buffer,
    size_t num_chars
);

#ifdef __cplusplus
}
#endif

#endif /* PKSAV_GEN1_TEXT_H */
