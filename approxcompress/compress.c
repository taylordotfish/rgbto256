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

#include "../ecode.h"
#include <stdio.h>
#include <stdbool.h>

#define DB_SIZE (256 * 256 * 256)

typedef enum {
    LOAD_OK = 0,
    LOAD_FILE_ERR = 1,
    LOAD_ALLOC_FAILED = 2,
    LOAD_BAD_DB = 3,
} LoadStatus;

LoadStatus load_db(const char *path, char **data) {
    *data = NULL;
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        return LOAD_FILE_ERR;
    }

    char *buf = malloc(DB_SIZE);
    if (buf == NULL) {
        fclose(file);
        return LOAD_ALLOC_FAILED;
    }

    size_t fread_status = fread(buf, 1, DB_SIZE, file);
    fclose(file);
    if (fread_status == DB_SIZE) {
        *data = buf;
        return LOAD_OK;
    }

    free(buf);
    return LOAD_BAD_DB;
}

ExitCode load_db_or_ecode(const char *path, char **data) {
    switch (load_db(path, data)) {
        case LOAD_OK:
            return exitcode_noexit;

        case LOAD_FILE_ERR:
            fprintf(stderr, "Could not open database.\n");
            break;

        case LOAD_ALLOC_FAILED:
            fprintf(stderr, "Memory allocation failed.\n");
            break;

        case LOAD_BAD_DB:
            fprintf(stderr, "Database contains invalid data.\n");
            break;

        default:
            fprintf(stderr, "Could not load database (unknown error).\n");
            break;
    }
    return exitcode_exit(1);
}

void write_size(FILE *file, size_t size) {
    if (size < 255) {
        fputc(size, file);
        return;
    }

    fputc(255, file);  // Signals 3-byte size
    // Write a 24-bit (3-byte) integer.
    for (int i = 0; i < 3; i++) {
        fputc((size & 0xff0000UL) >> 16, file);
        size <<= 8;
    }
}

void compress_db(FILE *file, char *data) {
    char prev = data[0];
    size_t chunk_size = 1;
    for (size_t i = 1; i < DB_SIZE; i++) {
        if (data[i] == prev) {
            chunk_size++;
            continue;
        }
        write_size(file, chunk_size);
        fputc(prev, file);
        prev = data[i];
        chunk_size = 1;
    }
    write_size(file, chunk_size);
    fputc(prev, file);
}

bool save_compressed(const char *compressed_db_path, char *uncompressed_data) {
    FILE *file = fopen(compressed_db_path, "wb");
    if (file == NULL) {
        return false;
    }
    compress_db(file, uncompressed_data);
    fclose(file);
    return true;
}

ExitCode save_compressed_or_ecode(
    const char *compressed_db_path,
    char *uncompressed_data
) {
    if (save_compressed(compressed_db_path, uncompressed_data)) {
        return exitcode_noexit;
    }
    fprintf(stderr, "Could not open compressed DB for writing.\n");
    return exitcode_exit(1);
}

ExitCode usage(void) {
    fprintf(stderr, "Usage: compress-approxdb <db-in-path> <db-out-path>\n");
    return exitcode_exit(1);
}

ExitCode parse_args(
    int argc,
    char **argv,
    const char **in_path,
    const char **out_path
) {
    if (argc != 3) {
        return usage();
    }
    *in_path = argv[1];
    *out_path = argv[2];
    return exitcode_noexit;
}

int main(int argc, char **argv) {
    const char *in_path, *out_path;
    maybe_exit(parse_args(argc, argv, &in_path, &out_path));

    char *db_data = NULL;
    maybe_exit(load_db_or_ecode(in_path, &db_data));

    int status = save_compressed_or_ecode(out_path, db_data).code;
    free(db_data);
    return status;
}
