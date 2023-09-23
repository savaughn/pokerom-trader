/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "util/fs.h"
#include "test-utils.h"

#include <pksav/config.h>

#include <stdio.h>
#include <string.h>

#ifdef PKSAV_PLATFORM_WIN32
#    include <windows.h>
#else
#    include <unistd.h>
#endif

int randomize_buffer(
    uint8_t* buffer,
    size_t buffer_len
)
{
    if(!buffer || (buffer_len == 0))
    {
        return -1;
    }

    // Do this in as few runs as possible.
    if((buffer_len % 4) == 0)
    {
        int* int_buffer = (int*)buffer;
        for(size_t int_buffer_index = 0;
            int_buffer_index < (buffer_len / 4);
            ++int_buffer_index)
        {
            int_buffer[int_buffer_index] = rand();
        }
    }
    else if((buffer_len % 2) == 0)
    {
        uint16_t* uint16_buffer = (uint16_t*)buffer;
        for(size_t uint16_buffer_index = 0;
            uint16_buffer_index < (buffer_len / 2);
            ++uint16_buffer_index)
        {
            uint16_buffer[uint16_buffer_index] = (uint16_t)(rand() % 0xFFFF);
        }
    }
    else
    {
        for(size_t buffer_index = 0; buffer_index < buffer_len; ++buffer_index)
        {
            buffer[buffer_index] = (uint8_t)(rand() % 0xFF);
        }
    }

    return 0;
}

int do_files_differ(
    const char* filename1,
    const char* filename2,
    bool* result_out
)
{
    if(!filename1 || !filename2 || !result_out)
    {
        return -1;
    }

    size_t file1size = 0;
    size_t file2size = 0;

    if(pksav_fs_filesize(filename1, &file1size) || pksav_fs_filesize(filename2, &file2size))
    {
        return -1;
    }

    if(file1size == file2size)
    {
        uint8_t* buffer1 = NULL;
        uint8_t* buffer2 = NULL;

        if(pksav_fs_read_file_to_buffer(filename1, &buffer1, &file1size))
        {
            free(buffer1);
            free(buffer2);
            return -1;
        }
        if(pksav_fs_read_file_to_buffer(filename2, &buffer2, &file2size))
        {
            free(buffer1);
            free(buffer2);
            return -1;
        }

        *result_out = (memcmp(buffer1, buffer2, file1size) != 0);
        free(buffer1);
        free(buffer2);
        return 0;
    }
    else
    {
        *result_out = true;
        return 0;
    }

    // Should not reach here
    return -1;
}

int get_pid()
{
#ifdef PKSAV_PLATFORM_WIN32
    return (int)GetCurrentProcessId();
#else
    return (int)getpid();
#endif
}

// Direct port of boost::archive::tmpdir()
const char* get_tmp_dir()
{
    const char* ret = NULL;

    ret = getenv("TMP");
    if(ret) {
        return ret;
    }
    ret = getenv("TMPDIR");
    if(ret) {
        return ret;
    }
    ret = getenv("TEMP");
    if(ret) {
        return ret;
    }

    // Somehow, none of these exist, so just guess.
    ret = "/tmp";

    return ret;
}

int delete_file(
    const char* filepath
)
{
    if(!filepath)
    {
        return -1;
    }

#ifdef PKSAV_PLATFORM_WIN32
    return (DeleteFile(filepath) != 0) ? 0 : 1;
#else
    return remove(filepath);
#endif
}
