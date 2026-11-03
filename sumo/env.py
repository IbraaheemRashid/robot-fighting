"""Two-player robot-sumo environment on MuJoCo.

Each bot sends (left, right) wheel commands in [-1, 1] and gets an 11-dim
observation expressed in its own frame, so the same policy can play either side.
A bout ends when a bot leaves the ring, stays flipped, or time runs out.
"""

from dataclasses import dataclass, field

import mujoco
import numpy as np

from .arena import BOT_NAMES, MAX_WHEEL_SPEED, RING_RADIUS, RING_TOP, WHEEL_RADIUS, build_arena_xml

OBS_DIM = 11
CONTROL_DT = 0.02  # policies act at 50 Hz
FLIP_TIMEOUT = 1.0  # seconds upside down before it counts as a loss


@dataclass
class BoutInfo:
    t: float = 0.0
    done: bool = False
    winner: str | None = None  # "red", "blue" or None for a draw
    reason: str = ""
    flipped_time: dict = field(default_factory=lambda: {n: 0.0 for n in BOT_NAMES})


class SumoEnv:
    def __init__(self, time_limit: float = 20.0, seed: int | None = None):
        self.model = mujoco.MjModel.from_xml_string(build_arena_xml())
        self.data = mujoco.MjData(self.model)
        self.time_limit = time_limit
        self.n_substeps = round(CONTROL_DT / self.model.opt.timestep)
        self.rng = np.random.default_rng(seed)
        self._body = {n: self.model.body(n).id for n in BOT_NAMES}
        self._qpos = {n: self.model.joint(f"{n}_root").qposadr[0] for n in BOT_NAMES}
        self._qvel = {n: self.model.joint(f"{n}_root").dofadr[0] for n in BOT_NAMES}
        self._act = {n: [self.model.actuator(f"{n}_{s}").id for s in ("l", "r")] for n in BOT_NAMES}
        self.info = BoutInfo()

    def reset(self):
        mujoco.mj_resetData(self.model, self.data)
        for name, side in zip(BOT_NAMES, (-1.0, 1.0)):
            x = side * 0.5 + self.rng.uniform(-0.1, 0.1)
            y = self.rng.uniform(-0.15, 0.15)
            # Face roughly toward the centre, with some jitter.
            yaw = (0.0 if side < 0 else np.pi) + self.rng.uniform(-0.4, 0.4)
            adr = self._qpos[name]
            self.data.qpos[adr:adr + 3] = (x, y, RING_TOP + WHEEL_RADIUS + 0.002)
            self.data.qpos[adr + 3:adr + 7] = (np.cos(yaw / 2), 0, 0, np.sin(yaw / 2))
        mujoco.mj_forward(self.model, self.data)
        self.info = BoutInfo()
        return self.observe()

    def step(self, action_red, action_blue):
        """Advance one control step. Returns (obs, rewards, info); obs/rewards keyed by bot."""
        for name, action in zip(BOT_NAMES, (action_red, action_blue)):
            cmd = np.clip(np.asarray(action, dtype=float), -1.0, 1.0) * MAX_WHEEL_SPEED
            self.data.ctrl[self._act[name]] = cmd
        mujoco.mj_step(self.model, self.data, nstep=self.n_substeps)
        self.info.t += CONTROL_DT
        self._judge()
        rewards = {n: 0.0 for n in BOT_NAMES}
        if self.info.done and self.info.winner:
            loser = next(n for n in BOT_NAMES if n != self.info.winner)
            rewards[self.info.winner], rewards[loser] = 1.0, -1.0
        return self.observe(), rewards, self.info

    def _judge(self):
        out = [n for n in BOT_NAMES if self._is_out(n)]
        for n in BOT_NAMES:
            up_z = self.data.xmat[self._body[n]][8]
            self.info.flipped_time[n] = self.info.flipped_time[n] + CONTROL_DT if up_z < 0 else 0.0
        flipped = [n for n in BOT_NAMES if self.info.flipped_time[n] >= FLIP_TIMEOUT]
        losers = set(out) | set(flipped)
        if len(losers) == 1:
            loser = losers.pop()
            self._finish(next(n for n in BOT_NAMES if n != loser), "ring out" if loser in out else "flipped")
        elif len(losers) == 2:
            self._finish(None, "double out")
        elif self.info.t >= self.time_limit:
            self._finish(None, "time")

    def _finish(self, winner, reason):
        self.info.done, self.info.winner, self.info.reason = True, winner, reason

    def _is_out(self, name):
        x, y, z = self.data.xpos[self._body[name]]
        return z < RING_TOP or np.hypot(x, y) > RING_RADIUS + 0.05

    def _pose(self, name):
        b = self._body[name]
        rot = self.data.xmat[b].reshape(3, 3)
        return self.data.xpos[b][:2].copy(), np.arctan2(rot[1, 0], rot[0, 0]), rot

    def observe(self):
        return {n: self._observe(n, next(o for o in BOT_NAMES if o != n)) for n in BOT_NAMES}

    def _observe(self, me, opp):
        pos, yaw, rot = self._pose(me)
        opp_pos, opp_yaw, _ = self._pose(opp)
        c, s = np.cos(yaw), np.sin(yaw)
        to_local = np.array([[c, s], [-s, c]])  # world -> my frame (yaw only)
        rel_opp = to_local @ (opp_pos - pos)
        to_center = to_local @ (-pos)
        dv = self._qvel[me]
        vel_local = (rot.T @ self.data.qvel[dv:dv + 3])[:2]
        yaw_rate = self.data.qvel[dv + 5]  # free-joint angular velocity is in the local frame
        rel_heading = opp_yaw - yaw
        return np.array([
            *rel_opp,
            np.cos(rel_heading), np.sin(rel_heading),
            *vel_local,
            yaw_rate,
            *to_center,
            np.hypot(*opp_pos) / RING_RADIUS,
            rot[2, 2],
        ])
