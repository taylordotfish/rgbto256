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

#define _XOPEN_SOURCE 500

#include "color.h"
#include <stdio.h>
#include <math.h>

static const double srgb_to_xyz_matrix[] = {
    0.4124, 0.3576, 0.1805,
    0.2126, 0.7152, 0.0722,
    0.0193, 0.1192, 0.9505,
};

static const double xyz_to_srgb_matrix[] = {
    3.2406255, -1.537208, -0.4986286,
    -0.9689307, 1.8757561, 0.0415175,
    0.0557101, -0.2040211, 1.0569959,
};

#define LAB_XYZ_SIGMA (6.0 / 29.0)
#define LAB_XYZ_SIGMA_SQUARED (LAB_XYZ_SIGMA * LAB_XYZ_SIGMA)
#define LAB_XYZ_SIGMA_CUBED (LAB_XYZ_SIGMA_SQUARED * LAB_XYZ_SIGMA)
#define XYZ_D65_XN 0.95047
#define XYZ_D65_YN 1.0
#define XYZ_D65_ZN 1.08883
#define VAL_25_POW_7 6103515625

static inline double to_deg(double rad) {
    return rad * (180.0 / M_PI);
}

static inline double to_rad(double deg) {
    return deg * (M_PI / 180.0);
}

static inline double atan2_or_zero(double y, double x) {
    return (x == 0 && y == 0) ? 0 : atan2(y, x);
}

static inline double square(double value) {
    return value * value;
}

static void matrix_vector_multiply(
        const double *matrix, const double *vector, size_t n_rows,
        size_t n_cols, double *result) {
    for (size_t row = 0; row < n_rows; row++) {
        double sum = 0;
        for (size_t col = 0; col < n_cols; col++) {
            sum += matrix[row * n_cols + col] * vector[col];
        }
        result[row] = sum;
    }
}

LChColor lab_to_lch(LabColor lab) {
    return (LChColor){
        .L = lab.L,
        .C = hypot(lab.a, lab.b),
        .h = to_deg(atan2_or_zero(lab.b, lab.a)),
    };
}

LabColor lch_to_lab(LChColor lch) {
    double h_rad = to_rad(lch.h);
    return (LabColor){
        .L = lch.L,
        .a = lch.C * cos(h_rad),
        .b = lch.C * sin(h_rad),
    };
}

RGBColor rgb_from_ints(int r, int g, int b) {
    return (RGBColor){
        .r = r / 255.0,
        .g = g / 255.0,
        .b = b / 255.0,
    };
}

void rgb_to_ints(RGBColor rgb, int *r, int *g, int *b) {
    *r = round(rgb.r * 255.0);
    *g = round(rgb.g * 255.0);
    *b = round(rgb.b * 255.0);
}

static inline double srgb_transfer(double value) {
    if (value <= 0.0031308) {
        return 12.92 * value;
    }
    return 1.055 * pow(value, 1 / 2.4) - 0.055;
}

static inline double srgb_inverse(double value) {
    if (value <= 0.04045) {
        return value / 12.92;
    }
    return pow((value + 0.055) / 1.055, 2.4);
}

SRGBColor rgb_to_srgb(RGBColor rgb) {
    return (SRGBColor){
        .rl = srgb_inverse(rgb.r),
        .gl = srgb_inverse(rgb.g),
        .bl = srgb_inverse(rgb.b),
    };
}

RGBColor srgb_to_rgb(SRGBColor srgb) {
    return (RGBColor){
        .r = srgb_transfer(srgb.rl),
        .g = srgb_transfer(srgb.gl),
        .b = srgb_transfer(srgb.bl),
    };
}

XYZColor srgb_to_xyz(SRGBColor srgb) {
    double xyz_vals[3];
    double srgb_vals[] = { srgb.rl, srgb.gl, srgb.bl };
    matrix_vector_multiply(
        srgb_to_xyz_matrix, srgb_vals, 3, 3, xyz_vals
    );
    return (XYZColor){
        .x = xyz_vals[0],
        .y = xyz_vals[1],
        .z = xyz_vals[2],
    };
}

SRGBColor xyz_to_srgb(XYZColor xyz) {
    double srgb_vals[3];
    double xyz_vals[] = { xyz.x, xyz.y, xyz.z };
    matrix_vector_multiply(
        xyz_to_srgb_matrix, xyz_vals, 3, 3, srgb_vals
    );
    return (SRGBColor){
        .rl = srgb_vals[0],
        .gl = srgb_vals[1],
        .bl = srgb_vals[2],
    };
}

static double lab_xyz_f(double value) {
    if (value > LAB_XYZ_SIGMA_CUBED) {
        return cbrt(value);
    }
    return value / (3 * LAB_XYZ_SIGMA_SQUARED) + (4.0 / 29.0);
}

static double lab_xyz_f_inv(double value) {
    if (value > LAB_XYZ_SIGMA) {
        return value * value * value;
    }
    return 3 * LAB_XYZ_SIGMA_SQUARED * (value - (4.0 / 29.0));
}

LabColor xyz_to_lab(XYZColor xyz) {
    double f_y_yn = lab_xyz_f(xyz.y / XYZ_D65_YN);
    return (LabColor){
        .L = 116 * f_y_yn - 16,
        .a = 500 * (lab_xyz_f(xyz.x / XYZ_D65_XN) - f_y_yn),
        .b = 200 * (f_y_yn - lab_xyz_f(xyz.z / XYZ_D65_ZN)),
    };
}

XYZColor lab_to_xyz(LabColor lab) {
    double f_y_yn = (lab.L + 16) / 116.0;
    return (XYZColor){
        .x = XYZ_D65_XN * lab_xyz_f_inv(f_y_yn + lab.a / 500.0),
        .y = XYZ_D65_YN * lab_xyz_f_inv(f_y_yn),
        .z = XYZ_D65_ZN * lab_xyz_f_inv(f_y_yn - lab.b / 200.0),
    };
}

LabColor srgb_to_lab(SRGBColor srgb) {
    return xyz_to_lab(srgb_to_xyz(srgb));
}

SRGBColor lab_to_srgb(LabColor lab) {
    return xyz_to_srgb(lab_to_xyz(lab));
}

LabColor rgb_to_srgb_lab(RGBColor rgb) {
    return srgb_to_lab(rgb_to_srgb(rgb));
}

RGBColor lab_to_srgb_rgb(LabColor lab) {
    return srgb_to_rgb(lab_to_srgb(lab));
}

static double ciede2000_a_prime(double a, double Cavg_pow7) {
    double C7 = Cavg_pow7;
    return a + (a / 2.0) * (1 - sqrt(C7 / (C7 + VAL_25_POW_7)));
}

static double ciede2000_h_prime(double b, double a_p) {
    double h_p = to_deg(atan2_or_zero(b, a_p));
    if (h_p < 0) {
        h_p += 360;
    }
    return h_p;
}

static double ciede2000_dh_prime(double h1_p, double h2_p) {
    if (fabs(h1_p - h2_p) <= 180) {
        return h2_p - h1_p;
    }
    if (h2_p <= h1_p) {
        return h2_p - h1_p + 360;
    }
    return h2_p - h1_p - 360;
}

static double ciede2000_Havg_prime(double h1_p, double h2_p) {
    if (fabs(h1_p - h2_p) <= 180) {
        return (h1_p + h2_p) / 2.0;
    }
    if (h1_p + h2_p < 360) {
        return (h1_p + h2_p + 360) / 2.0;
    }
    return (h1_p + h2_p - 360) / 2.0;
}

double ciede2000(LabColor lab1, LabColor lab2) {
    LChColor lch1 = lab_to_lch(lab1);
    LChColor lch2 = lab_to_lch(lab2);

    double dL_p = lab2.L - lab1.L;
    double Lavg = (lab1.L + lab2.L) / 2.0;
    double Cavg = (lch1.C + lch2.C) / 2.0;
    double Cavg_pow7 = pow(Cavg, 7);
    double a1_p = ciede2000_a_prime(lab1.a, Cavg_pow7);
    double a2_p = ciede2000_a_prime(lab2.a, Cavg_pow7);
    double C1_p = hypot(a1_p, lab1.b);
    double C2_p = hypot(a2_p, lab2.b);
    double Cavg_p = (C1_p + C2_p) / 2.0;
    double dC_p = C2_p - C1_p;
    double h1_p = ciede2000_h_prime(lab1.b, a1_p);
    double h2_p = ciede2000_h_prime(lab2.b, a2_p);
    double dh_p = ciede2000_dh_prime(h1_p, h2_p);
    double dH_p = 2 * sqrt(C1_p * C2_p) * sin(to_rad(dh_p / 2.0));
    double Havg_p = ciede2000_Havg_prime(h1_p, h2_p);
    double T = (
        1 -
        0.17 * cos(to_rad(Havg_p - 30)) +
        0.24 * cos(to_rad(2 * Havg_p)) +
        0.32 * cos(to_rad(3 * Havg_p + 6)) -
        0.20 * cos(to_rad(4 * Havg_p - 63))
    );

    double Lavg_m50 = Lavg - 50;
    double Lavg_m50_sq = square(Lavg_m50);
    double SL = 1 + (0.015 * Lavg_m50_sq) / sqrt(20 + Lavg_m50_sq);
    double SC = 1 + 0.045 * Cavg_p;
    double SH = 1 + 0.015 * Cavg_p * T;

    double Cavg_p_pow7 = pow(Cavg, 7);
    double Havg_p_m275_d25 = (Havg_p - 275) / 25;
    double Havg_p_m275_d25_sq = square(Havg_p_m275_d25);
    double RT = (
        -2 * sqrt(Cavg_p_pow7 / (Cavg_p_pow7 + VAL_25_POW_7)) *
        sin(to_rad(60 * exp(-Havg_p_m275_d25_sq)))
    );

    double dL_p_div_SL = dL_p / SL;
    double dC_p_div_SC = dC_p / SC;
    double dH_p_div_SH = dH_p / SH;
    return sqrt(
        square(dL_p_div_SL) +
        square(dC_p_div_SC) +
        square(dH_p_div_SH) +
        RT * dC_p_div_SC * dH_p_div_SH
    );
}
