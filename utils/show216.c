/*
 * Copyright (C) 2021, 2024 taylor.fish <contact@taylor.fish>
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

#include "../ansicode.h"
#include "../ansidb/ansidb.h"
#include <stdbool.h>

#ifndef SHOW216_SHOW_REST
#define SHOW216_SHOW_REST false
#endif

#ifndef SHOW216_C1
#define SHOW216_C1 r
#endif

#ifndef SHOW216_C2
#define SHOW216_C2 g
#endif

#ifndef SHOW216_C3
#define SHOW216_C3 b
#endif

void print_cube(FILE *file, int c3) {
    for (int c2 = 0; c2 < 6; c2++) {
        for (int c1 = 0; c1 < 6; c1++) {
            int SHOW216_C1 = c1;
            int SHOW216_C2 = c2;
            int SHOW216_C3 = c3;
            int color = 16 + (36 * r) + (6 * g) + b;
            XYZColor xyz = rgb_to_xyz(srgb_to_rgb(ansi_to_srgb(color)));
            ansi_256_bg(file, color);
            ansi_256_fg(file, (xyz.y <= 0.5) ? 231 : 16);
            fprintf(file, "   %3d   ", color);
        }
        ansi_reset(file);
        fprintf(file, "\n");
    }
}

void show_color(FILE *file, int color) {
    ansi_256_bg(file, color);
    fprintf(file, "         ");
    ansi_reset(file);
    fprintf(file, " %3d\n", color);
}

void show_rest(FILE *file) {
    for (int i = 0; i < 16; i++) {
        show_color(file, i);
    }

    for (int i = 216 + 16; i < 256; i++) {
        show_color(file, i);
    }
}

int main(void) {
    for (int c3 = 0; c3 < 6; c3++) {
        if (c3 > 0) {
            printf("\n");
        }
        print_cube(stdout, c3);
    }

    #if SHOW216_SHOW_REST
        printf("\n");
        show_rest(stdout);
    #endif
    return 0;
}
