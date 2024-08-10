# Copyright (C) 2021 taylor.fish <contact@taylor.fish>
#
# This file is part of rgbto256.
#
# rgbto256 is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# rgbto256 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with rgbto256. If not, see <https://www.gnu.org/licenses/>.

BUILD_DIR = build
BUILD_SUBDIRS = ansidb approxgen approxcompress approxdb utils

CC = gcc
CFLAGS += -Wall -Wextra -pedantic -std=c99 -O3 -MMD
LDFLAGS =
LDLIBS =

APRGEN_OBJECTS = color.o ansidb/ansidb.o approxgen/ansiapprox.o \
                 approxgen/generate.o utils/utils.o
APRGEN_BINARY = approxgen/generate-approxdb

DBGEN_OBJECTS = ansidb/generate.o color.o
DBGEN_BINARY = ansidb/generate-ansidb

APRCMP_OBJECTS = approxcompress/compress.o
APRCMP_BINARY = approxcompress/compress-approxdb

RTO2_OBJECTS = approxdb/approxdb.o approxdb/compressed.o ansicode.o \
               utils/rgbto256.o
RTO2_BINARY = utils/rgbto256

SHOW216_OBJECTS = utils/show216.o ansicode.o ansidb/ansidb.o color.o
SHOW216_BINARY = utils/show216

2TORT_OBJECTS = utils/256torgb-text.o ansidb/ansidb.o color.o utils/utils.o
2TORT_BINARY = utils/256torgb-text

RTO2T_OBJECTS = approxdb/approxdb.o approxdb/compressed.o utils/utils.o \
                utils/rgbto256-text.o
RTO2T_BINARY = utils/rgbto256-text

BUILD_SUBDIRS := $(addprefix $(BUILD_DIR)/,$(BUILD_SUBDIRS))
APRGEN_OBJECTS := $(addprefix $(BUILD_DIR)/,$(APRGEN_OBJECTS))
DBGEN_OBJECTS := $(addprefix $(BUILD_DIR)/,$(DBGEN_OBJECTS))
APRCMP_OBJECTS := $(addprefix $(BUILD_DIR)/,$(APRCMP_OBJECTS))
RTO2_OBJECTS := $(addprefix $(BUILD_DIR)/,$(RTO2_OBJECTS))
SHOW216_OBJECTS := $(addprefix $(BUILD_DIR)/,$(SHOW216_OBJECTS))
2TORT_OBJECTS := $(addprefix $(BUILD_DIR)/,$(2TORT_OBJECTS))
RTO2T_OBJECTS := $(addprefix $(BUILD_DIR)/,$(RTO2T_OBJECTS))

LM_BINARIES = $(APRGEN_BINARY) $(DBGEN_BINARY) $(2TORT_BINARY) \
              $(SHOW216_BINARY)
$(LM_BINARIES): LDLIBS += -lm

ALL_OBJECTS = $(APRGEN_OBJECTS) $(DBGEN_OBJECTS) $(APRCMP_OBJECTS) \
              $(RTO2_OBJECTS) $(SHOW216_OBJECTS) $(2TORT_OBJECTS) \
              $(RTO2T_OBJECTS)
ALL_BINARIES = $(APRGEN_BINARY) $(DBGEN_BINARY) $(APRCMP_BINARY) \
               $(RTO2_BINARY) $(SHOW216_BINARY) $(2TORT_BINARY) \
               $(RTO2T_BINARY)

.PHONY: all
all: $(ALL_BINARIES)

$(APRGEN_BINARY): $(APRGEN_OBJECTS)
$(DBGEN_BINARY): $(DBGEN_OBJECTS)
$(APRCMP_BINARY): $(APRCMP_OBJECTS)
$(RTO2_BINARY): $(RTO2_OBJECTS)
$(SHOW216_BINARY): $(SHOW216_OBJECTS)
$(2TORT_BINARY): $(2TORT_OBJECTS)
$(RTO2T_BINARY): $(RTO2T_OBJECTS)

$(BUILD_DIR)/approxdb/compressed.o: approxdb/compressed-db.bin

$(ALL_BINARIES):
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

-include $(ALL_OBJECTS:.o=.d)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.s
	$(CC) -c -o $@ $<

$(ALL_OBJECTS): | $(BUILD_DIR) $(BUILD_SUBDIRS)

$(BUILD_SUBDIRS): | $(BUILD_DIR)

$(BUILD_SUBDIRS) $(BUILD_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -f $(ALL_OBJECTS) $(ALL_OBJECTS:.o=.d) $(ALL_BINARIES)
	[ -d build ] && find $(BUILD_DIR) -type d -empty -delete || true

%: %.o
