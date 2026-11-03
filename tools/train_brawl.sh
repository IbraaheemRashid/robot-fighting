#!/bin/bash
# Train brawl and log the dashboard to a file. Extra args are puffer overrides.
#   tools/train_brawl.sh /tmp/run.log --env.bot_policy=1 ...
LOG=$1
shift
export PATH=/usr/local/cuda/bin:$PATH
cd "${PUFFER:-$HOME/PufferLib}"
./puffer train "$@" > "$LOG" 2>&1
echo "EXIT $?"
bash "$(dirname "$0")/summarize_log.sh" "$LOG"
