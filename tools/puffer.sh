#!/bin/bash
# Run a command inside the PufferLib checkout with CUDA on PATH.
#   tools/puffer.sh ./build.sh brawl
#   tools/puffer.sh ./puffer train --base.env_name=brawl
export PATH=/usr/local/cuda/bin:$PATH
cd "${PUFFER:-$HOME/PufferLib}"
"$@"
