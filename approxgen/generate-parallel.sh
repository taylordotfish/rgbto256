#!/bin/bash
cd "$(dirname "$0")"
mkdir -p approxdb/
for i in $(seq 0 15); do

    tmux new -d -s db$i "\
./generate-approxdb approxdb/db$i \
$((256 * 256 * 16 * i)) \
$((256 * 256 * 16 * (i + 1))) \
"

done
