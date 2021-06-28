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

#ifndef ECODE_H
#define ECODE_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int code;
    bool exit;
} ExitCode;

static const ExitCode exitcode_noexit = { .code = 0, .exit = false };

static inline ExitCode exitcode_exit(int code) {
    return (ExitCode){ .code = code, .exit = true };
}

static inline void maybe_exit(ExitCode ecode) {
    if (ecode.exit) {
        exit(ecode.code);
    }
}

#endif
