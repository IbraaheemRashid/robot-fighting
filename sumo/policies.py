"""Scripted sumo fighters. Each maps an observation (see SumoEnv._observe) to (left, right).

These are baselines and sparring partners for the RL agents that come next.
"""

import time

import numpy as np

from .arena import RING_RADIUS

# Observation layout
OPP_X, OPP_Y, OPP_COS, OPP_SIN = 0, 1, 2, 3
CENTER_X, CENTER_Y = 7, 8


def drive_to(target, speed=1.0, gain=1.5):
    """Steer toward a point in the bot's own frame; spins in place if it's behind."""
    angle = np.arctan2(target[1], target[0])
    turn = np.clip(gain * angle, -1.0, 1.0)
    fwd = speed * max(0.0, np.cos(angle))
    return np.clip([fwd - turn, fwd + turn], -1.0, 1.0)


class Policy:
    name = "policy"

    def reset(self):
        pass

    def __call__(self, obs):
        raise NotImplementedError


class EdgeAware(Policy):
    """Retreats toward the centre when near the edge unless the opponent is right in front."""

    def _safe(self, obs):
        opp = obs[[OPP_X, OPP_Y]]
        center = obs[[CENTER_X, CENTER_Y]]
        near_edge = np.linalg.norm(center) / RING_RADIUS > 0.8
        opp_ahead = opp[0] > 0 and abs(np.arctan2(opp[1], opp[0])) < 0.5 and np.linalg.norm(opp) < 0.4
        return None if not near_edge or opp_ahead else drive_to(center, speed=0.7)


class Charger(EdgeAware):
    """Turns to face the opponent and rams it."""

    name = "charger"

    def __call__(self, obs):
        safe = self._safe(obs)
        return safe if safe is not None else drive_to(obs[[OPP_X, OPP_Y]])


class Flanker(EdgeAware):
    """Circles to the opponent's back before committing to a push."""

    name = "flanker"

    def __call__(self, obs):
        safe = self._safe(obs)
        if safe is not None:
            return safe
        opp = obs[[OPP_X, OPP_Y]]
        facing = obs[[OPP_COS, OPP_SIN]]
        behind_it = np.dot(-opp, facing) < 0
        if behind_it or np.linalg.norm(opp) < 0.3:
            return drive_to(opp)
        return drive_to(opp - 0.35 * facing, speed=0.9)


class Wanderer(Policy):
    """Smoothly random wheel commands, a weak opponent for sanity checks."""

    name = "random"

    def __init__(self, seed=None):
        self.rng = np.random.default_rng(seed)
        self.action = np.zeros(2)

    def reset(self):
        self.action = np.zeros(2)

    def __call__(self, obs):
        self.action = np.clip(0.9 * self.action + 0.3 * self.rng.normal(size=2), -1, 1)
        return self.action


class Pilot(Policy):
    """Human control from the viewer. Arrow keys: up/down set throttle, left/right turn briefly, space stops."""

    name = "pilot"
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE = 265, 264, 263, 262, 32
    TURN_HOLD = 0.25  # the viewer only reports key presses, so each turn press lasts this long

    def __init__(self):
        self.throttle = 0.0
        self.turn = 0.0
        self.turn_until = 0.0

    def reset(self):
        self.throttle = self.turn = 0.0

    def on_key(self, key):
        if key == self.KEY_UP:
            self.throttle = min(1.0, self.throttle + 0.5)
        elif key == self.KEY_DOWN:
            self.throttle = max(-1.0, self.throttle - 0.5)
        elif key in (self.KEY_LEFT, self.KEY_RIGHT):
            self.turn = 0.8 if key == self.KEY_LEFT else -0.8
            self.turn_until = time.monotonic() + self.TURN_HOLD
        elif key == self.KEY_SPACE:
            self.throttle = self.turn = 0.0

    def __call__(self, obs):
        turn = self.turn if time.monotonic() < self.turn_until else 0.0
        return np.clip([self.throttle - turn, self.throttle + turn], -1.0, 1.0)


POLICIES = {"charger": Charger, "flanker": Flanker, "random": Wanderer, "pilot": Pilot}
