#!/bin/bash
# Link the brawl env from this repo into a PufferLib 5.0 checkout (default ~/PufferLib).
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
PUFFER="${1:-$HOME/PufferLib}"
ln -sfn "$HERE/brawl" "$PUFFER/ocean/brawl"
ln -sf "$HERE/brawl/brawl.ini" "$PUFFER/config/brawl.ini"
echo "Linked brawl into $PUFFER"
