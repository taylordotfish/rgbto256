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

#include "../approxdb/approxdb.h"
#include "../ecode.h"
#include "utils.h"
#include <stdio.h>

static ExitCode usage(void) {
    fprintf(stderr, "Usage: rgbto256-text <r> <g> <b>\n");
    return exitcode_exit(1);
}

ExitCode parse_args(int argc, char **argv, int *r, int *g, int *b) {
    if (argc != 4) {
        return usage();
    }

    long rl = 0, gl = 0, bl = 0;
    bool success = (
        try_strtol(argv[1], 10, &rl) &&
        try_strtol(argv[2], 10, &gl) &&
        try_strtol(argv[3], 10, &bl)
    );
    if (!success) {
        return usage();
    }

    *r = rl;
    *g = gl;
    *b = bl;
    return exitcode_noexit;
}

void print_rgb_to_ansi(int r, int g, int b) {
    int ansi_code = approxdb_closest_to_rgb(r, g, b);
    printf("%d\n", ansi_code);
}

int main(int argc, char **argv) {
    int r = 0, g = 0, b = 0;
    maybe_exit(parse_args(argc, argv, &r, &g, &b));
    maybe_exit(approxdb_init_or_ecode());
    print_rgb_to_ansi(r, g, b);
    approxdb_free();
    return 0;
}
