"""Run sumo bouts between two fighters.

  python run_match.py --red charger --blue flanker --bouts 50   # headless tournament
  python run_match.py --red pilot --blue charger --view         # watch (or drive red with arrow keys)
"""

import argparse
import time
from collections import Counter

from sumo import POLICIES, SumoEnv
from sumo.env import CONTROL_DT


def run_headless(env, red, blue, bouts):
    results, reasons = Counter(), Counter()
    steps, start = 0, time.perf_counter()
    for _ in range(bouts):
        obs = env.reset()
        red.reset(), blue.reset()
        info = env.info
        while not info.done:
            obs, _, info = env.step(red(obs["red"]), blue(obs["blue"]))
            steps += 1
        results[info.winner or "draw"] += 1
        reasons[info.reason] += 1
    elapsed = time.perf_counter() - start
    print(f"{red.name} (red) vs {blue.name} (blue), {bouts} bouts")
    print(f"  red wins {results['red']}, blue wins {results['blue']}, draws {results['draw']}")
    print(f"  endings: {dict(reasons)}")
    print(f"  {steps / elapsed:,.0f} control steps/s ({steps * env.n_substeps / elapsed:,.0f} physics steps/s)")


def run_viewer(env, red, blue):
    import mujoco.viewer

    pilots = [p for p in (red, blue) if hasattr(p, "on_key")]

    def on_key(key):
        for p in pilots:
            p.on_key(key)

    score = Counter()
    with mujoco.viewer.launch_passive(env.model, env.data, key_callback=on_key) as viewer:
        viewer.cam.distance, viewer.cam.elevation = 3.2, -55
        while viewer.is_running():
            obs = env.reset()
            red.reset(), blue.reset()
            info = env.info
            while viewer.is_running() and not info.done:
                tick = time.perf_counter()
                obs, _, info = env.step(red(obs["red"]), blue(obs["blue"]))
                viewer.sync()
                time.sleep(max(0.0, CONTROL_DT - (time.perf_counter() - tick)))
            if info.done:
                score[info.winner or "draw"] += 1
                print(f"{info.winner or 'draw'} ({info.reason}, {info.t:.1f}s)  "
                      f"score red {score['red']} - blue {score['blue']} - draws {score['draw']}")
                time.sleep(1.0)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--red", choices=POLICIES, default="charger")
    parser.add_argument("--blue", choices=POLICIES, default="flanker")
    parser.add_argument("--bouts", type=int, default=20)
    parser.add_argument("--time-limit", type=float, default=20.0)
    parser.add_argument("--seed", type=int, default=0)
    parser.add_argument("--view", action="store_true", help="open the MuJoCo viewer")
    args = parser.parse_args()

    if "pilot" in (args.red, args.blue) and not args.view:
        parser.error("the pilot policy needs --view")
    env = SumoEnv(time_limit=args.time_limit, seed=args.seed)
    red, blue = POLICIES[args.red](), POLICIES[args.blue]()
    if args.view:
        run_viewer(env, red, blue)
    else:
        run_headless(env, red, blue, args.bouts)


if __name__ == "__main__":
    main()
