#!/bin/bash
# Build and run the brawl bot-vs-bot test against a PufferLib checkout.
#   ./test.sh            # headless stats
#   ./test.sh 2 1        # watch bot 2 (smart) vs bot 1 (rush)
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
PUFFER="${PUFFER:-$HOME/PufferLib}"
RAYLIB="$PUFFER/raylib-5.5_linux_amd64"
mkdir -p "$HOME/.cache/brawl"
OUT="$HOME/.cache/brawl/test_brawl"
${CC:-gcc} -O2 -Wall -Wno-unused-function -Wno-unused-variable -std=gnu11 \
    -I"$HERE/brawl" -I"$PUFFER/src" -I"$PUFFER/vendor" -I"$RAYLIB/include" \
    "$HERE/brawl/test_brawl.c" -o "$OUT" \
    "$RAYLIB/lib/libraylib.a" -lGL -lm -lpthread -ldl
"$OUT" "$@"
