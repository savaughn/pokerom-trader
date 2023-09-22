/*!
 * @file    pksav/math/endian.h
 * @ingroup PKSav
 * @brief   Endianness (byte-ordering) conversions.
 *
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_MATH_ENDIAN_H
#define PKSAV_MATH_ENDIAN_H

#include <pksav/config.h>

#include <stdint.h>

#ifndef __DOXYGEN__
#if defined(PKSAV_PLATFORM_WIN32) || defined(PKSAV_PLATFORM_MINGW)
#    include <stdlib.h>
#    define PKSAV_BYTESWAP16(num) _byteswap_ushort(num)
#    define PKSAV_BYTESWAP32(num) _byteswap_ulong(num)
#elif defined(PKSAV_PLATFORM_LINUX)
#    include <byteswap.h>
#    define PKSAV_BYTESWAP16(num) bswap_16(num)
#    define PKSAV_BYTESWAP32(num) bswap_32(num)
#elif defined(PKSAV_PLATFORM_MACOS)
#    include <libkern/OSByteOrder.h>
#    define PKSAV_BYTESWAP16(num) OSSwapInt16(num)
#    define PKSAV_BYTESWAP32(num) OSSwapInt32(num)
#else
#    define PKSAV_BYTESWAP16(num) (((num & 0xFF) << 8) | ((num & 0xFF00) >> 8))
#    define PKSAV_BYTESWAP32(num) ((PKSAV_BYTESWAP16((uint16_t)(num & 0xFFFF)) << 16) | \
                                   (PKSAV_BYTESWAP16((uint16_t)((num & 0xFFFF0000) >> 16))))
#endif
#endif /* __DOXYGEN__ */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Swaps the endianness (byte ordering) of a 16-bit number.
 *
 * When possible, this function uses an optimized version of this
 * operation provided by the operating system.
 */
static PKSAV_INLINE uint16_t pksav_byteswap16(uint16_t num) {
    return PKSAV_BYTESWAP16(num);
}

/*!
 * @brief Swaps the endianness of a given 16-bit number to/from big-endian.
 *
 * On big-endian platforms, this simply returns the given number.
 */
static PKSAV_INLINE uint16_t pksav_bigendian16(uint16_t num) {
#if PKSAV_LITTLE_ENDIAN
    return pksav_byteswap16(num);
#else
    return num;
#endif
}

/*!
 * @brief Swaps the endianness of a given 16-bit number to/from little-endian.
 *
 * On little-endian platforms, this simply returns the given number.
 */
static PKSAV_INLINE uint16_t pksav_littleendian16(uint16_t num) {
#if PKSAV_LITTLE_ENDIAN
    return num;
#else
    return pksav_byteswap16(num);
#endif
}

/*!
 * @brief Swaps the endianness (byte ordering) of a 32-bit number.
 *
 * When possible, this function uses an optimized version of this
 * operation provided by the operating system.
 */
static PKSAV_INLINE uint32_t pksav_byteswap32(uint32_t num) {
    return PKSAV_BYTESWAP32(num);
}

/*!
 * @brief Swaps the endianness of a given 32-bit number to/from big-endian.
 *
 * On big-endian platforms, this simply returns the given number.
 */
static PKSAV_INLINE uint32_t pksav_bigendian32(uint32_t num) {
#if PKSAV_LITTLE_ENDIAN
    return pksav_byteswap32(num);
#else
    return num;
#endif
}

/*!
 * @brief Swaps the endianness of a given 32-bit number to/from little-endian.
 *
 * On little-endian platforms, this simply returns the given number.
 */
static PKSAV_INLINE uint32_t pksav_littleendian32(uint32_t num) {
#if PKSAV_LITTLE_ENDIAN
    return num;
#else
    return pksav_byteswap32(num);
#endif
}

#ifdef __cplusplus
}
#endif

#endif
