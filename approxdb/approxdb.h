/*
 * Copyright (C) 2021 taylor.fish <contact@taylor.fish>
 *
 * This file is part of rgbto256.
 *
 * rgbto256 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rgbto256 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with rgbto256. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef APPROXDB__APPROXDB_H
#define APPROXDB__APPROXDB_H

#include "../ecode.h"
#include <stdlib.h>
#include <stdint.h>

extern unsigned char *approxdb_main_db;

typedef enum {
    APD_INIT_OK = 0,
    APD_INIT_ALREADY = 1,
    APD_INIT_ALLOC_FAIL = 2,
} ApproxDBInitStatus;

ApproxDBInitStatus approxdb_init(void);
void approxdb_free(void);
ExitCode approxdb_init_or_ecode(void);

static inline int approxdb_closest_to_srgb(int r, int g, int b) {
    if (r < 0 || g < 0 || b < 0) {
        return -1;
    }
    if (r > 255 || g > 255 || b > 255) {
        return -1;
    }
    size_t index = (r << 16) | (g << 8) | b;
    return approxdb_main_db[index];
}

#endif
