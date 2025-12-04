/*
 * various simple utilities for libavformat
 * Copyright (c) 2000, 2001, 2002 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "libavutil/time_internal.h"
#include "avformat.h"
#include "internal.h"
#include "libavutil/avstring.h"

#define ISLEAP(y) (((y) % 4 == 0) && (((y) % 100) != 0 || ((y) % 400) == 0))
#define LEAPS_COUNT(y) ((y)/4 - (y)/100 + (y)/400)

/* This is our own gmtime_r. It differs from its POSIX counterpart in a
   couple of places, though. */
struct tm *ff_brktimegm(time_t secs, struct tm *tm)
{
    tm = gmtime_r(&secs, tm);

    tm->tm_year += 1900; /* unlike gmtime_r we store complete year here */
    tm->tm_mon  += 1;    /* unlike gmtime_r tm_mon is from 1 to 12 */

    return tm;
}

int ff_mkdir_p(const char *path)
{
    int ret = 0;
    char *temp = av_strdup(path);
    char *pos = temp;
    char tmp_ch = '\0';

    if (!path || !temp) {
        return -1;
    }

    // Skip root indicator
    if (!av_strncasecmp(temp, "/", 1) || !av_strncasecmp(temp, "\\", 1)) {
        pos++;
    // skip current dir leading indicator
    } else if (!av_strncasecmp(temp, "./", 2) || !av_strncasecmp(temp, ".\\", 2)) {
        pos += 2;
    }

    for ( ; *pos != '\0'; ++pos) {
        if (*pos == '/' || *pos == '\\') {
            tmp_ch = *pos;
            // intermediate
            *pos = '\0';
            ret = mkdir(temp, 0755);
            *pos = tmp_ch;
        }
    }

    if ((*(pos - 1) != '/') && (*(pos - 1) != '\\')) {
        ret = mkdir(temp, 0755);
    }

    av_free(temp);
    return ret;
}