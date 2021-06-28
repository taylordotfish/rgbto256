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

#include "approxdb.h"
#include "string.h"
#include <stdio.h>

#define UINT64_MASK 0xFFFFFFFFFFFFFFFF
#define DB_SIZE (256 * 256 * 256)

extern const unsigned char approxdb_compressed[];
extern const uint_least64_t approxdb_compressed_size;
unsigned char *approxdb_main_db = NULL;

static inline size_t get_approxdb_size(void) {
    return approxdb_compressed_size & UINT64_MASK;
}

static size_t read_size(size_t *i) {
    size_t size = approxdb_compressed[(*i)++];
    if (size < 255) {
        return size;
    }

    size = 0;
    for (int j = 0; j < 3; j++) {
        size <<= 8;
        size += approxdb_compressed[(*i)++];
    }
    return size;
}

static void fill_db(unsigned char *db) {
    size_t compressed_size = get_approxdb_size();
    size_t cmp_i = 0;
    size_t db_i = 0;

    while (cmp_i < compressed_size) {
        size_t size = read_size(&cmp_i);
        unsigned char val = approxdb_compressed[cmp_i++];
        memset(db + db_i, val, size);
        db_i += size;
    }
}

ApproxDBInitStatus approxdb_init(void) {
    if (approxdb_main_db != NULL) {
        return APD_INIT_ALREADY;
    }

    unsigned char *db = malloc(DB_SIZE);
    if (db == NULL) {
        return APD_INIT_ALLOC_FAIL;
    }

    fill_db(db);
    approxdb_main_db = db;
    return APD_INIT_OK;
}

void approxdb_free(void) {
    free(approxdb_main_db);
    approxdb_main_db = NULL;
}

ExitCode approxdb_init_or_ecode(void) {
    switch (approxdb_init()) {
        case APD_INIT_OK:
        case APD_INIT_ALREADY:
            return exitcode_noexit;

        case APD_INIT_ALLOC_FAIL:
            fprintf(stderr, "Memory allocation failed.\n");
            break;

        default:
            fprintf(
                stderr, "Could not initialize approx DB (unknown error).\n"
            );
            break;
    }
    return exitcode_exit(1);
}

