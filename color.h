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
} SRGBColor;

typedef struct {
    double rl;
    double gl;
    double bl;
} RGBColor;

typedef struct {
    double x;
    double y;
    double z;
} XYZColor;

LChColor lab_to_lch(LabColor lab);
LabColor lch_to_lab(LChColor lch);
SRGBColor srgb_from_ints(int r, int g, int b);
void srgb_to_ints(SRGBColor srgb, int *r, int *g, int *b);
RGBColor srgb_to_rgb(SRGBColor srgb);
SRGBColor rgb_to_srgb(RGBColor rgb);
XYZColor rgb_to_xyz(RGBColor rgb);
RGBColor xyz_to_rgb(XYZColor xyz);
LabColor xyz_to_lab(XYZColor xyz);
XYZColor lab_to_xyz(LabColor lab);
LabColor rgb_to_lab(RGBColor rgb);
RGBColor lab_to_rgb(LabColor lab);
LabColor srgb_to_lab(SRGBColor srgb);
SRGBColor lab_to_srgb(LabColor lab);
double ciede2000(LabColor lab1, LabColor lab2);

#endif
