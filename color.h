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

#ifndef COLOR_H
#define COLOR_H

typedef struct {
    double L;
    double a;
    double b;
} LabColor;

typedef struct {
    double L;
    double C;
    double h;
} LChColor;

typedef struct {
    double r;
    double g;
    double b;
} RGBColor;

typedef struct {
    double rl;
    double gl;
    double bl;
} SRGBColor;

typedef struct {
    double x;
    double y;
    double z;
} XYZColor;

LChColor lab_to_lch(LabColor lab);
LabColor lch_to_lab(LChColor lch);
RGBColor rgb_from_ints(int r, int g, int b);
void rgb_to_ints(RGBColor rgb, int *r, int *g, int *b);
SRGBColor rgb_to_srgb(RGBColor rgb);
RGBColor srgb_to_rgb(SRGBColor srgb);
XYZColor srgb_to_xyz(SRGBColor srgb);
SRGBColor xyz_to_srgb(XYZColor xyz);
LabColor xyz_to_lab(XYZColor xyz);
XYZColor lab_to_xyz(LabColor lab);
LabColor srgb_to_lab(SRGBColor srgb);
SRGBColor lab_to_srgb(LabColor lab);
LabColor rgb_to_srgb_lab(RGBColor rgb);
RGBColor lab_to_srgb_rgb(LabColor lab);
double ciede2000(LabColor lab1, LabColor lab2);

#endif
