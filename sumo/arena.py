"""MJCF for a robot-sumo arena: a raised circular ring and two wedge bots.

Scale is roughly a 3 lb (beetleweight) robot: ~1.4 kg, 20 x 16 cm footprint.
Each bot is differential drive (two powered wheels + two frictionless casters)
with a sloped wedge on the front for getting under the opponent.
"""

RING_RADIUS = 1.0  # metres
RING_TOP = 0.1  # height of the ring surface above the floor
WHEEL_RADIUS = 0.035
MAX_WHEEL_SPEED = 30.0  # rad/s, ~1 m/s top speed
BOT_NAMES = ("red", "blue")
BOT_COLORS = {"red": "0.85 0.2 0.2 1", "blue": "0.2 0.35 0.9 1"}


def _bot_xml(name: str, color: str) -> str:
    # Body origin sits at wheel-axle height, so a resting bot has z = RING_TOP + WHEEL_RADIUS.
    # Wheels collide as spheres (hidden in group 3): MuJoCo's cylinder-cylinder contact against
    # the ring was unstable and launched bots into the air. The cylinders are visual only.
    return f"""
    <body name="{name}" pos="0 0 {RING_TOP + WHEEL_RADIUS + 0.01}">
      <freejoint name="{name}_root"/>
      <geom name="{name}_chassis" type="box" size="0.1 0.08 0.03" pos="0 0 0.005"
            mass="1.2" rgba="{color}"/>
      <geom name="{name}_wedge" type="box" size="0.004 0.08 0.035" pos="0.11 0 -0.005"
            euler="0 -40 0" mass="0.1" rgba="0.75 0.75 0.75 1" friction="0.3"/>
      <geom name="{name}_caster_f" type="sphere" size="0.012" pos="0.08 0 -0.023"
            mass="0.01" condim="1" priority="1" rgba="0.3 0.3 0.3 1"/>
      <geom name="{name}_caster_b" type="sphere" size="0.012" pos="-0.08 0 -0.023"
            mass="0.01" condim="1" priority="1" rgba="0.3 0.3 0.3 1"/>
      <body name="{name}_wheel_l" pos="0 0.095 0">
        <joint name="{name}_wheel_l" type="hinge" axis="0 1 0" damping="0.001"/>
        <geom name="{name}_wheel_l" type="sphere" size="{WHEEL_RADIUS}" mass="0.05" friction="1.2"
              group="3" rgba="0.1 0.1 0.1 1"/>
        <geom type="cylinder" size="{WHEEL_RADIUS} 0.01" euler="90 0 0" mass="0"
              contype="0" conaffinity="0" rgba="0.1 0.1 0.1 1"/>
      </body>
      <body name="{name}_wheel_r" pos="0 -0.095 0">
        <joint name="{name}_wheel_r" type="hinge" axis="0 1 0" damping="0.001"/>
        <geom name="{name}_wheel_r" type="sphere" size="{WHEEL_RADIUS}" mass="0.05" friction="1.2"
              group="3" rgba="0.1 0.1 0.1 1"/>
        <geom type="cylinder" size="{WHEEL_RADIUS} 0.01" euler="90 0 0" mass="0"
              contype="0" conaffinity="0" rgba="0.1 0.1 0.1 1"/>
      </body>
    </body>"""


def _actuators_xml(name: str) -> str:
    # Velocity servos with a torque cap, like a hobby brushless drive.
    return "".join(
        f"""
    <velocity name="{name}_{side}" joint="{name}_wheel_{side}" kv="0.1"
              ctrlrange="-{MAX_WHEEL_SPEED} {MAX_WHEEL_SPEED}" forcerange="-0.6 0.6"/>"""
        for side in ("l", "r")
    )


def build_arena_xml() -> str:
    bots = "".join(_bot_xml(n, BOT_COLORS[n]) for n in BOT_NAMES)
    actuators = "".join(_actuators_xml(n) for n in BOT_NAMES)
    half = RING_TOP / 2
    return f"""
<mujoco model="sumo_arena">
  <option timestep="0.002" integrator="implicitfast"/>
  <visual>
    <global offwidth="1280" offheight="720"/>
  </visual>
  <asset>
    <texture name="grid" type="2d" builtin="checker" rgb1="0.25 0.27 0.3" rgb2="0.2 0.22 0.25"
             width="512" height="512"/>
    <material name="grid" texture="grid" texrepeat="8 8"/>
  </asset>
  <worldbody>
    <light pos="0 0 4" dir="0 0 -1" diffuse="0.9 0.9 0.9"/>
    <geom name="floor" type="plane" size="4 4 0.1" material="grid"/>
    <geom name="ring" type="cylinder" size="{RING_RADIUS} {half}" pos="0 0 {half}"
          friction="0.9" rgba="0.95 0.95 0.95 1"/>
    <!-- Black visual disk on top leaves a white border line at the edge. -->
    <geom name="ring_inner" type="cylinder" size="{RING_RADIUS - 0.03} 0.0005"
          pos="0 0 {RING_TOP + 0.0004}" contype="0" conaffinity="0" rgba="0.08 0.08 0.08 1"/>
    {bots}
  </worldbody>
  <actuator>{actuators}
  </actuator>
</mujoco>
"""
