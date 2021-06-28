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

#include "../ansidb/ansidb.h"
#include "../ecode.h"
#include "utils.h"
#include <stdio.h>

static ExitCode usage(void) {
    fprintf(stderr, "Usage: 256torgb-text <ansi-code>\n");
    return exitcode_exit(1);
}

ExitCode parse_args(int argc, char **argv, int *ansi_code) {
    if (argc != 2) {
        return usage();
    }

    long ansi_code_long = 0;
    if (!try_strtol(argv[1], 10, &ansi_code_long)) {
        return usage();
    }
    if (ansi_code_long < ANSI_MIN || ansi_code_long >= ANSI_MAX) {
        return usage();
    }

    *ansi_code = ansi_code_long;
    return exitcode_noexit;
}

void print_ansi_to_rgb(int ansi_code) {
    RGBColor rgb = ansi_to_rgb(ansi_code);
    int r = 0, g = 0, b = 0;
    rgb_to_ints(rgb, &r, &g, &b);
    printf("%d %d %d\n", r, g, b);
}

int main(int argc, char **argv) {
    int ansi_code = 0;
    maybe_exit(parse_args(argc, argv, &ansi_code));
    print_ansi_to_rgb(ansi_code);
    return 0;
}
