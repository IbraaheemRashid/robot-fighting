# Brawl (PufferLib env)

A side-view 1v1 robot platform fighter written as a PufferLib 5.0 Ocean env, for training
fighters with self-play. Knock the other bot past the blast zones; damage builds up and makes
knockback grow.

- **Moves:** jab (fast, weak), heavy (slow, strong launch), block, grab. Attacks beat grabs,
  grabs beat blocks, blocks beat attacks. Blocking drains a guard meter; empty guard means a
  stun.
- **Actions:** `{move: none/left/right, jump: no/yes, move: none/jab/heavy/block/grab}`.
- **Observations:** 39 floats, ego-centric and mirrored so both seats see the same game.
- **Rewards:** +-1 per KO, plus `damage_reward` x damage dealt/taken.
- **Scripted bots** (`bot_policy`, weakest first): `0` idle, `1` rush, `2` smart. Used for
  training against a fixed opponent and as the eval ladder.

## Files

- `brawl/brawl.h` is the whole env, following PufferLib's env API and style guide.
- `brawl/brawl.ini` is the training config (self-play against a pool of past checkpoints).
- `brawl/test_brawl.c` runs the bots against each other: win rates, KOs, obs range and speed.
- `link_into_pufferlib.sh` symlinks the env into a PufferLib checkout (default `~/PufferLib`).
- `test.sh` builds and runs the bot test in WSL (`./test.sh 2 1` to watch smart vs rush).

## Train (WSL2 Ubuntu, NVIDIA GPU)

One-time setup: `tools/setup_wsl_cuda.sh`, then clone PufferLib 5.0 into `~/PufferLib` and run
`link_into_pufferlib.sh`. From Windows, `tools/puffer.sh` runs a command in the checkout with
CUDA on the PATH, and `tools/train_brawl.sh LOG [overrides]` trains and summarizes the log.

```
cd ~/PufferLib
./build.sh brawl            # trainer -> ./puffer
./puffer train              # selfplay, ~3M steps/s on an RTX 5070 Ti
./build.sh brawl --cpu      # viewer -> ./brawl
./brawl latest              # watch the newest checkpoint; hold Left Shift to pilot red
./brawl /mnt/c/Users/icybo/FIGHTING/puffer/models/brawl_selfplay_500M.bin
```

Viewer controls while holding Left Shift: A/D move, W jump, J jab, K heavy, L block, I grab.

## Results

500M steps of selfplay (~3 min on an RTX 5070 Ti), never trained against the bots:

| Opponent | Win rate |
|---|---|
| idle | 100% |
| rush | 100% |
| smart | 93% |

Saved as `models/brawl_selfplay_500M.bin`. Trained directly against the smart bot instead, a
policy wins 100% within 60M steps, a sign the scripted bot is exploitable.

## Lessons

- Actions must be mirrored for seat 1 exactly like observations. Before that fix one shared
  policy saw "toward the opponent" flip between seats and selfplay stayed uniform-random.
- `damage_reward` 0.003 was too sparse to learn from; 0.01-0.02 works.
- `./puffer train` ignores `load_model_path` (only eval/match load weights), so curricula
  that warm-start selfplay from a bot-trained policy need a trainer change.

## Bot ladder (300 games each, seat 0 vs seat 1)

| | vs idle | vs rush | vs smart |
|---|---|---|---|
| idle | draw | 0% | 0% |
| rush | 100% | ~50% | 8% |
| smart | 100% | 88% | ~50% |

About 11M env steps/s on one CPU core.
