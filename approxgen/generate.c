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

#include "ansiapprox.h"
#include "../ansicode.h"
#include "../ecode.h"
#include "../utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

ExitCode generate_db(const char *path, long start, long end) {
    FILE *db = fopen(path, "wb");
    if (db == NULL) {
        fprintf(stderr, "Could not open DB for writing.\n");
        return exitcode_exit(1);
    }

    for (long i = start; i < end;) {
        int r = (i >> 16) & 255;
        int g = (i >> 8) & 255;
        int b = i & 255;
        fputc(closest_ansi_to_srgb(srgb_from_ints(r, g, b)), db);

        i++;
        if (i % 64 == 0 || i == end) {
            printf(
                "\r%8ld/%ld (%.4f%%)", i, end,
                (100.0 * i) / end
            );
            fflush(stdout);
        }
    }

    printf("\n");
    fclose(db);
    return exitcode_noexit;
}

static ExitCode usage(void) {
    fprintf(stderr, "Usage: generate-approxdb <db-path> [<start> <end>]\n");
    return exitcode_exit(1);
}

ExitCode parse_args(
    int argc,
    char **argv,
    const char **path,
    long *start,
    long *end
) {
    if (!(argc == 2 || argc == 4)) {
        return usage();
    }
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        return usage();
    }

    *path = argv[1];
    if (argc < 4) {
        *start = 0;
        *end = 256 * 256 * 256;
        return exitcode_noexit;
    }

    bool success = (
        try_strtol(argv[2], 10, start) &&
        try_strtol(argv[3], 10, end)
    );
    if (!success) {
        return usage();
    }
    return exitcode_noexit;
}

int main(int argc, char **argv) {
    const char *path = NULL;
    long start = 0;
    long end = 0;
    maybe_exit(parse_args(argc, argv, &path, &start, &end));
    return generate_db(path, start, end).code;
}
