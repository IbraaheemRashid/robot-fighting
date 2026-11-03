# Robot Fighting

Training fighting robots in simulation, with the long-term goal of moving them onto real
hardware (sim2real). Think small piloted robots fighting in a ring: the pilot picks the moves,
and learned controllers handle the fighting skills.

The project has two simulators that do different jobs:

| | **Brawl** (PufferLib) | **Robot Sumo** (MuJoCo) |
|---|---|---|
| Purpose | Strategy lab: learn to fight with self-play | Physics lab: realistic contact, path to real robots |
| World | 2D side-view platform fighter | 3D ring with 3 lb-class wedge bots |
| Speed | ~3M training steps/s on one GPU | ~4.6k control steps/s on one CPU core |
| Status | Self-play fighter trained | Arena, scripted fighters, human pilot |

## Brawl: self-play fighters in PufferLib

A 1v1 robot platform fighter written in C as a [PufferLib 5.0](https://github.com/PufferAI/PufferLib)
environment. Knock the other bot past the blast zones; damage builds up and makes knockback
grow. Four moves form a rock-paper-scissors triangle: attacks (jab, heavy) beat grabs, grabs beat
blocks, blocks beat attacks.

**Result:** after 500M steps of self-play (about 3 minutes on an RTX 5070 Ti), a fighter that never
trained against the scripted bots beats them:

| Opponent | Win rate |
|---|---|
| idle | 100% |
| rush | 100% |
| smart | 93% |

Details, setup and controls are in [puffer/README.md](puffer/README.md).

## Robot Sumo: MuJoCo arena

A raised 1 m ring and two differential-drive wedge bots (~1.4 kg, torque-capped wheels). Lose by
leaving the ring or staying flipped for a second. Each bot sees the fight from its own point of
view, so one policy can play either side.

```
py -3.11 -m venv .venv
.venv\Scripts\python -m pip install -r requirements.txt
.venv\Scripts\python run_match.py --red charger --blue flanker --bouts 50
.venv\Scripts\python run_match.py --red charger --blue flanker --view
.venv\Scripts\python run_match.py --red pilot --blue charger --view
```

As `pilot`, drive red with the arrow keys in the viewer (up/down throttle, left/right turn, space
stops). Scripted baselines: `charger` beats `random` 30-0 and beats `flanker` about 70% of the
time.

## Lessons so far

- **Mirror actions, not just observations.** In Brawl, the second seat sees a mirrored world.
  Until its left/right controls were mirrored too, one shared policy couldn't play both seats
  and self-play stayed completely random.
- **Check learning against a fixed opponent first.** Training against a scripted bot showed the
  learning setup worked, which narrowed the self-play failure down to the env.
- **Contact geometry matters.** In MuJoCo, cylinder wheels on a cylinder ring produced unstable
  contacts that launched bots into the air. Sphere colliders fixed it (0 glitches in 29k steps).
- **Beating a scripted bot 100% is a warning.** A policy trained directly against the smart bot
  wins every game within 60M steps, so that bot has an exploit.

## Research

[reports/Robot fighting sim2real roadmap.md](reports/Robot%20fighting%20sim2real%20roadmap.md)
covers the state of the art (human-piloted humanoid fight leagues, RL boxing research), simulators
and training methods, hardware from $300 combat kits to $100k humanoids, control interfaces, and
safety. The notes behind it are in [research_notes/](research_notes/).

## Layout

```
sumo/            MuJoCo arena, env and scripted fighters
run_match.py     sumo tournaments and viewer
puffer/brawl/    Brawl env (brawl.h), config and C test harness
puffer/models/   trained Brawl fighter
tools/           WSL CUDA setup and PufferLib training scripts
reports/         research report
```

## Next

1. Self-play RL for the sumo bots, then a GPU-parallel version (MuJoCo Warp / mjlab).
2. Stronger Brawl bots and longer runs, then a PufferLib contribution.
3. A small humanoid fighter in MuJoCo, with domain randomization for sim2real.
