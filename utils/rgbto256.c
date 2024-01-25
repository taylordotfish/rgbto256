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
#include "../ansicode.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static const char * const start_pattern[] = {
    "\x1b", "[", "34", "8", ";",
};

static const size_t start_pattern_len = (
    sizeof(start_pattern) / sizeof(*start_pattern)
);

static size_t fgetn(char *buf, size_t size, FILE *file) {
    for (size_t i = 0; i < size; i++) {
        int chr = fgetc(file);
        if (chr == EOF) {
            buf[i] = '\0';
            return i;
        }
        buf[i] = chr;
    }
    return size;
}

static void fputn(const char *buf, size_t size, FILE *file) {
    for (size_t i = 0; i < size; i++) {
        fputc(buf[i], file);
    }
}

bool try_start_pattern(FILE *in, FILE *out) {
    for (size_t i = 0; i < start_pattern_len; i++) {
        int chr = fgetc(in);
        if (chr == EOF) {
            return false;
        }

        fputc(chr, out);
        if (strchr(start_pattern[i], chr) == NULL) {
            return false;
        }
    }
    return true;
}

bool try_match(FILE *in, FILE *out) {
    if (!try_start_pattern(in, out)) {
        return false;
    }

    char buf[2];
    size_t len = fgetn(buf, sizeof(buf), in);
    if (buf[0] == '2' && buf[1] == ';') {
        fputs("5;", out);
        return true;
    }
    fputn(buf, len, out);
    return false;
}

bool read_until_match(FILE *in, FILE *out) {
    do {
        if (feof(in)) {
            return false;
        }
    } while (!try_match(in, out));
    return true;
}

void handle_match(FILE *in, FILE *out) {
    int r, g, b;
    if (scanf("%d;%d;%d", &r, &g, &b) != 3) {
        return;
    }

    fgetc(in);  // 'm'
    int closest_ansi = approxdb_closest_to_srgb(r, g, b);
    if (closest_ansi >= 0) {
        fprintf(out, "%dm", closest_ansi);
    }
}

void process_files(FILE *in, FILE *out) {
    while (read_until_match(in, out)) {
        handle_match(in, out);
    }
}

int main(void) {
    maybe_exit(approxdb_init_or_ecode());
    process_files(stdin, stdout);
    approxdb_free();
    return 0;
}
