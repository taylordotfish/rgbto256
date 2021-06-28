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

#include "../ansicode.h"
#include <stdbool.h>

#define SHOW_REST false

void print_cube(FILE *file, int g) {
    for (int r = 0; r < 6; r++) {
        for (int b = 0; b < 6; b++) {
            int color = 16 + (36 * r) + (6 * g) + b;
            ansi_256_bg(file, color);
            ansi_256_fg(file, (g < 3) ? 231 : 16);
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
    for (int g = 0; g < 6; g++) {
        if (g > 0) {
            printf("\n");
        }
        print_cube(stdout, g);
    }

    #if SHOW_REST
        printf("\n");
        show_rest(stdout);
    #endif
    return 0;
}
