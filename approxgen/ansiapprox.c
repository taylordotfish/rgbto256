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
#include <stdlib.h>

int closest_ansi_to_lab(LabColor lab) {
    int min_ansi = 0;
    double min_diff = 0;
    for (size_t i = 0; i < ANSI_MAP_LEN; i++) {
        LabColor ansi_lab = ansi_to_lab(i + ANSI_MAP_OFFSET);
        double diff = ciede2000(lab, ansi_lab);
        if (i == 0 || diff < min_diff) {
            min_ansi = i + ANSI_MAP_OFFSET;
            min_diff = diff;
        }
    }
    return min_ansi;
}

int closest_ansi_to_srgb(SRGBColor srgb) {
    LabColor lab = srgb_to_lab(srgb);
    return closest_ansi_to_lab(lab);
}
