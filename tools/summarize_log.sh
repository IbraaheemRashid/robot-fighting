#!/bin/bash
# Condense a PufferLib dashboard log into one line per sampled frame.
#   tools/summarize_log.sh /tmp/brawl_train.log [every_nth_frame]
LOG=$1
EVERY=${2:-25}
awk -v every="$EVERY" '
    /Steps/ {steps=$3}
    /entropy/ {ent=$(NF-1)}
    /^│ perf/ {perf=$3}
    /episode_return/ {ret=$3; len=$5}
    /damage_dealt/ {kos=$3; dmg=$5;
        n++; if (n % every == 0) printf "steps %-7s perf %-6s return %-8s len %-8s kos %-6s dmg %-7s entropy %s\n", steps, perf, ret, len, kos, dmg, ent}
' "$LOG"
grep -a "bot_eval\|Error\|error" "$LOG" | tail -8
