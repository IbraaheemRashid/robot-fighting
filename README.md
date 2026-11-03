# Robot Sumo Sim

A MuJoCo arena for robot fighting, starting with 3 lb-class wedge sumo bots. Step one toward
training fighters with self-play RL and moving them onto real robots.

## Setup (Windows)

```
py -3.11 -m venv .venv
.venv\Scripts\python -m pip install -r requirements.txt
```

## Run

```
.venv\Scripts\python run_match.py --red charger --blue flanker --bouts 50
.venv\Scripts\python run_match.py --red charger --blue flanker --view
.venv\Scripts\python run_match.py --red pilot --blue charger --view
```

As `pilot`, drive red with the arrow keys in the viewer: up/down step the throttle, left/right
turn, space stops.

## How it works

- `sumo/arena.py` builds the MJCF: a 1 m-radius raised ring and two differential-drive bots
  (~1.4 kg, torque-capped velocity-servo wheels, front wedge, frictionless casters).
- `sumo/env.py` is the two-player environment. Each bot sends `(left, right)` wheel commands in
  `[-1, 1]` at 50 Hz and gets an 11-dim observation in its own frame, so one policy can play
  either colour. Rewards are +1/-1 on a win/loss.
- A bout ends on a ring out, a bot staying flipped for 1 s, or the time limit (a draw).
- `sumo/policies.py` has scripted baselines: `charger`, `flanker`, `random`, plus `pilot` for
  human control.

## Baselines (seed 0)

| Matchup | Result |
|---|---|
| charger vs random (30 bouts) | 30-0 |
| charger vs flanker (50 bouts, each side) | charger wins ~71% |

About 4,600 control steps/s (46k physics steps/s) on one CPU core.

## Notes

- Wheels collide as spheres. MuJoCo's cylinder-on-cylinder contact with the ring launched bots
  into the air, so the visible cylinders are visual only.

## Next

1. Gymnasium wrapper and PPO self-play (month 2).
2. GPU-parallel version on MuJoCo Warp / mjlab, with a steps-per-second benchmark.
3. Small humanoid fighters.
