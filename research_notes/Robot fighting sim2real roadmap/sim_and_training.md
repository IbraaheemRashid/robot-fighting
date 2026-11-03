# Simulators and Training Pipelines for Sim2Real Humanoid/Legged Fighting Robots

(Research date: 2026-11-02. About 15 web calls were made. Search-tool summaries were cross-checked against primary pages where those were fetched: the mjlab GitHub page, the Isaac Lab installation docs, the RoboStriker arXiv HTML, the FastSAC arXiv abstract, the MuJoCo Playground site and the ASAP arXiv abstract. Anything else comes from search snippets and is marked as such.)

## Simulators: capabilities, hardware, Windows support, licensing, robot models

### Takeaway
In late 2026 the field is converging on GPU MuJoCo (MuJoCo Warp / MJX, used by mjlab and MuJoCo Playground) and NVIDIA's Isaac Lab 3.0. Isaac Lab 3.0 now uses Newton as its default backend, Newton runs on Warp, and it exposes a MuJoCo-Warp solver preset. MuJoCo Warp officially supports native Windows. Isaac Lab 3.0 lists Windows 11, but the vendor's own recommendation is still Ubuntu. For a Windows 11 hobbyist, the practical path is native Windows for MuJoCo Warp, or WSL2/Linux dual-boot (or a cloud Linux GPU) for Isaac Lab, mjlab or JAX/MJX.

### Cited Findings
**Isaac Lab / Isaac Sim / Newton**
- The Isaac Lab installation docs (main branch) cover Isaac Lab 3.0.0, with Isaac Sim 6.2.0 as an *optional* simulator. — [Isaac Lab install docs](https://isaac-sim.github.io/IsaacLab/main/source/isaaclab/installation.html)
- "Isaac Lab installs Newton by default", and Newton uses NVIDIA Warp 1.17 for simulation. There is a preset `physics=newton_mjwarp` (the MuJoCo-Warp solver inside Newton). PhysX remains available via the `isaacsim` extra (`physics=isaacsim_physx`) or the `ovphysx` extra. — [Isaac Lab install docs](https://isaac-sim.github.io/IsaacLab/main/source/isaaclab/installation.html)
- Supported operating systems: Ubuntu 22.04+ x86_64, Linux aarch64 (including DGX Spark) and Windows 11 x86_64. On Windows you must enable `LongPathsEnabled` in the registry before cloning. Python 3.12 is required. Minimum drivers are 595.58.03 (Linux) and 595.97 (Windows). The default CUDA 13 stack needs Turing or newer (compute capability 7.5+). — [Isaac Lab install docs](https://isaac-sim.github.io/IsaacLab/main/source/isaaclab/installation.html)
- Headless Newton does not need RTX hardware. If you use Isaac Sim, plan for at least 32 GB RAM, at least 16 GB VRAM and an RTX GPU. Isaac Sim requires accepting the Omniverse EULA, and its first launch can take more than 10 minutes. A "kit-less" Docker image (`nvcr.io/nvidia/isaac-lab:3.0.0-kitless`) ships Newton, OVPhysX and four RL libraries with no Isaac Sim EULA. — [Isaac Lab install docs](https://isaac-sim.github.io/IsaacLab/main/source/isaaclab/installation.html)
- The Isaac Lab 3.0 beta-era docs warned that Windows support "is still being completed" and recommended Ubuntu 24.04 for reliability. One 2026 summary calls the Newton integration "experimental" within the 3.0 beta. — [Isaac Lab 3.0.0-beta2 install](https://isaac-sim.github.io/IsaacLab/v3.0.0-beta2/source/setup/installation/index.html); [NVIDIA perspectives page](https://perspectives.nvidia.com/isaac-lab/which-simulation-frameworks-integrate-the-newton-physics-engin/) (snippet only)
- Isaac Sim 6.0 reached general availability, and NVIDIA forum threads report Newton teething issues (e.g. Newton working on CPU but not GPU, and crashes in 6.0 dev2). — [Isaac Sim 6.0 GA forum](https://forums.developer.nvidia.com/t/isaac-sim-6-0-general-availability/372621); [Newton issue thread](https://forums.developer.nvidia.com/t/issue-with-newton-physics-implementation/380046) (snippets)

**MuJoCo Warp / MJX / MuJoCo Playground**
- MuJoCo Warp (MJWarp) is "currently supported on Windows or Linux on x86-64". It needs native Python on Windows (not MSYS2 or WSL) and CUDA 12.4 or newer. An NVIDIA GPU is required for fast simulation, and CPU works for debugging. The latest PyPI version seen is about 3.8.x. "MJX-Warp is simply MJX hooked up to MuJoCo Warp." — [mujoco-warp PyPI](https://pypi.org/project/mujoco-warp/); [mujoco_warp GitHub](https://github.com/google-deepmind/mujoco_warp) (search snippet). Note that the mjlab README shows a "MuJoCo Warp 3.16.0" badge, so the version numbering is inconsistent across sources ([mjlab](https://github.com/mujocolab/mjlab)).
- MuJoCo Playground (DeepMind, arXiv 2502.08844, RSS 2025) is built on MJX. It claims policies train "in minutes on a single GPU", and the whole pipeline can run in a Colab notebook. Locomotion that once needed days on multi-host setups now takes minutes to hours on one GPU. It installs with `pip install playground`. — [arXiv 2502.08844](https://www.arxiv.org/pdf/2502.08844); [Playground site](https://playground.mujoco.org/); [Tech report](https://playground.mujoco.org/assets/playground_technical_report.pdf)
- Playground environments include quadruped joystick, handstand and fall recovery, general humanoid locomotion, and Booster T1 locomotion and fall recovery. The paper reports hardware transfer on robots including Berkeley Humanoid and Unitree G1. — [Playground site](https://playground.mujoco.org/); [arXiv 2502.08844](https://www.arxiv.org/pdf/2502.08844)
- Playground has a Beta MuJoCo Warp backend. — [Playground discussion #197](https://github.com/google-deepmind/mujoco_playground/discussions/197)

**Genesis**
- Early independent criticism said Genesis's headline speed benchmark "is not representative" of RL performance, and that contacts in a tutorial manipulation scene glitched (a cube falling out of the gripper). Commenters noted that the speed gains shrink in contact-heavy scenes. — [Stone Tao substack](https://stoneztao.substack.com/p/the-new-hyped-genesis-simulator-is)
- Genesis AI released "Genesis World 1.0" in May 2026 and claims an "unprecedentedly low sim-to-real gap". This is a vendor claim, not independent. — [Dealroom news](https://app.dealroom.co/news/note/genesis-ai-open-sources-world-1-0-simulation-platform-for-robotics)
- ASAP used Genesis as a sim2sim transfer *target*, i.e. as a test simulator. — [ASAP arXiv 2502.01143](https://arxiv.org/abs/2502.01143)
- The GAUGE physics-engine benchmark (Aug 2026) argues that simulator vendors generally leave contact-accuracy validation to users. — [Pebblous report](https://blog.pebblous.ai/report/physics-engine-reality-gap-gauge/en/) (snippet only; no Genesis-specific numbers seen)

**Contact accuracy in general**
- The UPenn DAIR lab (Acosta et al. 2022) studied validating robotics simulators against real-world impacts. — [DAIR bibliography](https://dair.seas.upenn.edu/bibliography/Acosta2022) (title only seen)

**Ready robot models (from repos found)**
- mjlab: Unitree G1 velocity and tracking tasks. — [mjlab](https://github.com/mujocolab/mjlab)
- Playground: Booster T1, Berkeley Humanoid, Unitree G1, plus quadrupeds. — [Playground](https://playground.mujoco.org/)
- unitree_rl_gym: Go2, H1, H1_2 and G1. — [unitree_rl_gym mirror](https://gitee.com/cvsuser/unitree_rl_gym)
- GMR retargeting supports G1, H1, H1-2, Booster T1, Berkeley Humanoid Lite, ToddlerBot, Fourier N1/GR3 and others (see the training section). — [GMR wiki](https://github.com/YanjieZe/GMR/wiki)
- DeepMind's OP3 soccer work was done in MuJoCo with a 20-joint Robotis OP3. — [arXiv 2304.13653](https://arxiv.org/html/2304.13653v2)

### Inferences
- **Easiest native-Windows route:** MuJoCo plus MuJoCo Warp (and anything built on it that does not depend on JAX). JAX GPU wheels have historically been Linux-only, so MJX-JAX and Playground-JAX on Windows most likely need WSL2. This is not verified; see Gaps.
- **Isaac Lab 3.0 on Windows:** officially listed, but reliability is unproven. Ubuntu dual-boot or WSL2 is the lower-risk choice. The kit-less Docker image is a good fit for cloud training.
- **Sim2sim checks:** because Newton exposes an `mjwarp` solver, policies can be cross-checked between PhysX and MuJoCo-Warp inside one framework. This is a cheap robustness check before hardware.
- **PyBullet and Gazebo:** not researched in this pass. Based on general field knowledge they are not competitive for massively parallel RL. Gazebo is more relevant for ROS 2 integration and testing than for training.
- **Brax:** effectively superseded by MJX/Playground as the DeepMind JAX path. Not verified in this pass.

### Gaps
- No source fetched on PyBullet or Gazebo's current state, or on Brax's 2026 status.
- No confirmation of JAX/MJX GPU on native Windows.
- No independent quantitative contact-accuracy benchmark comparing Isaac/Newton, MuJoCo and Genesis for humanoid impacts. The GAUGE report's per-engine numbers were not seen.
- No exact Isaac Lab 3.0 GA release date or Newton 1.0 date was found. The docs show 3.0.0 is out, alongside Isaac Sim 6.2.0.
- VRAM needs per environment count were not found for MuJoCo Warp or mjlab.

## Training approaches and compute (PPO, imitation, retargeting, self-play, distillation, DR, real-to-sim)

### Takeaway
The current recipe for a fighting humanoid is to (1) retarget human mocap (GMR), (2) train a whole-body motion tracker with PPO and heavy domain randomization (BeyondMimic-style tracking in mjlab or Isaac Lab, or KungfuBot/PBHC for agile skills), (3) distill it into a compact latent skill space, and (4) run self-play in that latent space. RoboStriker (Aug 2026) did exactly this for two Unitree G1s boxing, training on a *single RTX 4090*. Locomotion alone can now train in about 15 minutes on one RTX 4090 with FastSAC/FastTD3.

### Cited Findings
**Directly relevant: competitive humanoid boxing**
- **RoboStriker** (arXiv 2608.16195, 17 Aug 2026; Shanghai AI Lab, SJTU and others) frames humanoid boxing as a two-player zero-sum Markov game in the latent space of a pretrained motion decoder. — [arXiv HTML](https://arxiv.org/html/2608.16195)
  - Robot: Unitree G1, 29-DoF. Simulator: Isaac Lab, with 200 Hz physics and a 50 Hz policy.
  - Stage I is a motion tracker trained on about 30 minutes of Xsens boxing mocap (mirrored), retargeted to G1. Stage II is teacher-student distillation into a 32-D latent on a unit hypersphere, with an encoder, decoder and state-conditioned prior plus KL. Stage IIIa is a residual latent policy against a stationary opponent with AMP-style rewards. Stage IIIb is Latent-Space Neural Fictitious Self-Play: PPO best response plus a supervised average policy, a 10^6 reservoir buffer and η=0.1, with historical opponents rather than a formal league.
  - The paper says standard self-play in raw joint space "often collapses" because of humanoid fragility. That is why play happens in latent space.
  - Compute: a single RTX 4090 with 4,096 parallel environments. Total wall-clock time is not reported.
  - Simulation results: 68.5% win rate vs Fictitious SP, 76.2% vs Naive SP and 84.5% vs PPO-only. Landing rate was 0.685, vs 0.142 for raw 29-DoF action-space SP. The 32-D latent beat the 64-D one on landing rate (0.685 vs 0.520), even though 64-D tracked better (35.0 vs 38.5 mm MPKPE).
  - Real deployment: two G1s run ONNX on their onboard CPUs over ROS 2. Pelvis positions come from external mocap at 100 Hz, which is a crutch: no onboard opponent perception. Validation was qualitative only.
  - Domain randomization covered contact materials, joint biases, torso CoM and random pushes. A hit counts above a 10 N contact force. A bout ends when a non-foot body part touches the ground.
  - Code and video are said to be on [sites.google.com/view/robo-striker](https://sites.google.com/view/robo-striker). No license was stated.
- **DeepMind OP3 soccer** (Science Robotics, 10 Apr 2024): teacher skill policies (get-up, scoring) were distilled into one agent, which was then trained with multi-agent self-play in MuJoCo on a 20-joint OP3. Transfer to the real robots was zero-shot, and the authors found the sim2real gap "surprisingly low". — [arXiv 2304.13653](https://arxiv.org/html/2304.13653v2); [project site](https://sites.google.com/view/op3-soccer?pli=1). The compute was not found in the sources retrieved.

**Fast single-GPU training**
- "Learning Sim-to-Real Humanoid Locomotion in 15 Minutes" (Seo, Sferrazza, Chen, Shi, Duan, Abbeel; arXiv 2512.01996, Dec 2025) trains G1 and Booster T1 locomotion in about 15 minutes on a single RTX 4090 using off-policy **FastSAC/FastTD3** with thousands of parallel environments. It uses minimalist rewards and strong domain randomization (dynamics, rough terrain, pushes), and also covers whole-body motion tracking. The code is open source. — [arXiv 2512.01996](https://arxiv.org/abs/2512.01996)
- MuJoCo Playground trains in "minutes on a single GPU" for most tasks. — [arXiv 2502.08844](https://www.arxiv.org/pdf/2502.08844)

**Motion imitation / tracking**
- **BeyondMimic** (Cornell/Berkeley, arXiv 2508.08241, Aug 2025) is an open-source pipeline from raw mocap to hardware. It uses a unified MDP and a single hyperparameter set for motion tracking, plus guided-diffusion distillation for versatile control. A news write-up says the full training pipeline is open source and reproducible. — [arXiv 2508.08241](https://arxiv.org/html/2508.08241v2); [Interesting Engineering](https://interestingengineering.com/innovation/humanoid-robots-perform-cartwheels-sprints). The training time was not found.
- **KungfuBot / PBHC** (arXiv 2506.12851, NeurIPS 2025) is a two-stage framework for highly dynamic skills on a 23-DoF G1, with human video processed by GVHMR. It improves motion feasibility "entirely in simulation", in contrast to ASAP's real-data approach. Code is at TeleHuman/PBHC (about 1.1k stars), and a follow-up, KungfuBot2, adds general motion tracking. — [arXiv 2506.12851](https://arxiv.org/pdf/2506.12851); [TeleHuman GitHub](https://github.com/TeleHuman)
- **mjlab** ships a `Mjlab-Tracking-Flat-Unitree-G1` motion-imitation task, with motion datasets managed through a WandB registry, and a `Mjlab-Velocity-Flat-Unitree-G1` task at 4,096 envs. It supports multi-GPU via `--gpu-ids`. — [mjlab](https://github.com/mujocolab/mjlab)

**Real-to-sim / delta action**
- **ASAP** (arXiv 2502.01143, RSS 2025): pretrain in IsaacGym, deploy on the real G1, collect real data, then train a **delta (residual) action model** that compensates for the dynamics mismatch. That model is plugged into the simulator to fine-tune the policy. It beat SysID, domain randomization and delta-dynamics baselines on tracking error. Transfer was evaluated to IsaacSim, Genesis and the real G1. — [arXiv 2502.01143](https://arxiv.org/abs/2502.01143). The ASAP code release is commonly associated with the HumanoidVerse framework, but the abstract page does not confirm this.

**Retargeting**
- **GMR (General Motion Retargeting)**, by Yanjie Ze et al., is MIT-licensed. Inputs: SMPL-X (AMASS, OMOMO), GVHMR (monocular video), BVH (LAFAN1, Nokov) and OptiTrack FBX. Robots supported (about 17 by Jan 2026): G1 (including Dex31 hands), H1/H1-2, Booster T1, Berkeley Humanoid Lite, ToddlerBot, Fourier N1/GR3, PND Adam Lite, Tienkung, KUAVO, Talos, Galaxea R1 Pro and others. TWIST2 support was added on 2025-12-02. — [GMR wiki](https://github.com/YanjieZe/GMR/wiki); [fork changelog](https://github.com/REKbot/GMR) (robot counts differ across forks)

### Inferences
- **Fighting pipeline for a hobbyist:**
  - Record or obtain boxing/kicking mocap. A cheap route is phone video, then GVHMR, then GMR.
  - Train a tracker in mjlab or Isaac Lab (BeyondMimic-style).
  - Distill it into a latent skill space.
  - Run latent self-play à la RoboStriker.
  - This is now demonstrably feasible on one RTX 4090, though total hours are unreported.
- **Expected training times:** locomotion takes about 15 minutes to an hour on a 4090. Single-motion tracking probably takes hours. Multi-stage latent self-play probably takes a day or more. These are estimates, not sourced.
- **Self-play in latent space:** raw joint-space self-play is reported to collapse, so a hobbyist should not attempt it.
- **RTX 5090:** no benchmarks were found. Blackwell (compute capability 12.0) meets Isaac Lab's CUDA 13 minimum, but check the PyTorch and Warp builds.

### Gaps
- No wall-clock training times were found for BeyondMimic, PBHC, RoboStriker's full pipeline or OP3 soccer.
- Not researched in this pass: DeepMimic, AMP, ASE, PHC, GMT, HumanPlus, OmniH2O and ExBody specifics. These are older or adjacent work. AMP appears inside RoboStriker as a reward.
- No sources were fetched on actuator networks (the ANYmal-style learned actuator model) or on Unitree-specific SysID.
- No RTX 5090 benchmarks were found.

## Sim2real gaps for impacts, falls and contact

### Takeaway
Impacts and falls are hard to simulate for several reasons:
- Whole-body contacts (not just the feet) matter.
- Collision geometry choice has an outsized effect.
- Simulators trade step size and stiffness for speed.
- Vendors rarely publish contact-error envelopes.

Projects mitigate this with simplified or primitive collision geometry, randomized contact materials, push perturbations, smoothness regularization, terminating episodes on non-foot ground contact, and real-data correction (ASAP).

### Cited Findings
- **HoST** (RSS 2025 Best Systems Paper finalist) is a humanoid standing-up framework. Its repo says using full meshes as ankle collision models "will cause huge sim-to-real gap". The fix was to approximate the collision with discrete points, following Unitree's code. — [HoST GitHub](https://github.com/InternRobotics/HoST); [RSS paper](https://roboticsconference.org/2025/program/papers/64/)
- The get-up policy paper (arXiv 2502.12152) argues that getting up needs contacts beyond the feet. Locomotion shortcuts — freezing or decoupling the upper body, coarse upper-body collision and larger sim step sizes — "no longer hold". — [arXiv 2502.12152](https://arxiv.org/pdf/2502.12152)
- A survey says HoST relies on smoothing regularization and implicit action-velocity limits for natural standing on the G1. — [arXiv 2507.00917](https://arxiv.org/pdf/2507.00917)
- FIRM (a fall-safety follow-up) reports that HoST's success drops on hard terrain in simulation (23.2% uneven, 10.2% wave) vs 93.2% and 55.9% for FIRM. FIRM also measures impact via base impulse and acceleration. — [arXiv 2511.07407](https://arxiv.org/pdf/2511.07407) (snippet; these are sim numbers)
- RoboStriker randomizes contact materials, joint biases, torso CoM and pushes, and penalizes undesired contacts and action rate. It starts bouts from a stiff standing controller and sets PD gains from inertia (ω=10, ζ=2). It ends bouts on non-foot ground contact, i.e. falls are avoided rather than simulated well. — [RoboStriker](https://arxiv.org/html/2608.16195)
- The DeepMind OP3 team found the sim2real gap surprisingly low even for agile soccer, falls and get-ups, on a small, low-mass robot. — [arXiv 2304.13653](https://arxiv.org/html/2304.13653v2)
- An R:SS 2020 workshop summary records the view that simulator gaps for contact-rich tasks may be too wide for positive transfer. — [arXiv 2012.03806](https://arxiv.org/pdf/2012.03806)
- ASAP's delta action model learns the residual from real data, as a data-driven fix for dynamics mismatch. — [ASAP](https://arxiv.org/abs/2502.01143)

### Inferences
- **Robot size:** small, light robots (OP3 or ToddlerBot class) have lower impact energies and appear to transfer better. For a hobbyist fighting robot, a small humanoid is lower risk for both hardware damage and sim2real.
- **Validating hits:** strikes involve robot-robot soft contact (gloves or padding), which simulators model poorly. Use primitive capsule/box collision shapes, randomize restitution and friction, and threshold the reward on contact force (like the 10 N rule). Check strikes with a sim2sim test (Newton/PhysX vs MuJoCo-Warp) before hardware.

### Gaps
- No source quantified the sim-vs-real error of strike impact forces between two robots.
- No source was found on simulating padding or compliance for robot-robot strikes.

## Open-source starter frameworks and best beginner entry points

### Takeaway
For a beginner in late 2026:
- **mjlab** is arguably the cleanest entry point: Isaac Lab-style API on MuJoCo Warp, `uv` install, a G1 velocity and tracking demo, a Colab notebook and Apache-2.0.
- **MuJoCo Playground** is the easiest "train in Colab in minutes" option.
- **Isaac Lab 3.0** is the most capable, with the largest ecosystem, but the heaviest.
- **unitree_rl_gym** is the classic train-to-sim2sim-to-real template, but it sits on the legacy, Linux-only Isaac Gym stack.

### Cited Findings
- **mjlab:** combines the Isaac Lab manager-based API with MuJoCo Warp.
  - It is "a lightweight framework for GPU-accelerated robot learning" (arXiv 2601.22074, 2026), Apache-2.0, with some code forked from Isaac Lab under BSD-3.
  - Try it with `uvx --from mjlab demo`. Docker and Colab are available, and the PyPI version is about 1.3.0.
  - It requires an NVIDIA GPU for training. macOS is evaluation-only, and Windows is not mentioned.
  - — [mjlab GitHub](https://github.com/mujocolab/mjlab); [PyPI](https://pypi.org/project/mjlab/)
- **MuJoCo Playground:** `pip install playground`, runs in Colab, and has Booster T1 locomotion and get-up tasks. — [Playground](https://playground.mujoco.org/)
- **unitree_rl_gym:** covers Go2, H1, H1_2 and G1. The pipeline is Isaac Gym training, then MuJoCo sim2sim (`deploy/deploy_mujoco/deploy_mujoco.py g1.yaml`), then real deployment. It uses a Python 3.8 conda environment. — [unitree_rl_gym mirror](https://gitee.com/cvsuser/unitree_rl_gym); [RoboVerse wiki](https://roboverse.wiki/roboverse_learn/reinforcement_learning/unitree_rl)
- **Humanoid-Gym** (RobotEra, arXiv 2404.05695): a zero-shot sim2real humanoid locomotion framework. — [GitHub](https://github.com/roboterax/humanoid-gym)
- **Isaac Lab 3.0:** multiple RL libraries are bundled. RL-Games and Robomimic need a source checkout. — [Isaac Lab install](https://isaac-sim.github.io/IsaacLab/main/source/isaaclab/installation.html)
- **PBHC/KungfuBot** (G1, agile skills) and **HoST** (getting up) are open source. — [TeleHuman](https://github.com/TeleHuman); [HoST](https://github.com/InternRobotics/HoST)
- **FastSAC humanoid** code is linked from younggyo.me/fastsac-humanoid. — [arXiv 2512.01996](https://arxiv.org/abs/2512.01996)

### Inferences
- **Suggested beginner order on Windows 11:**
  1. MuJoCo Playground in Colab (zero setup).
  2. mjlab under WSL2 or Linux on a local GPU: G1 velocity, then G1 tracking with GMR-retargeted clips.
  3. Isaac Lab 3.0 if you need RoboStriker-style reproduction, since RoboStriker used Isaac Lab.
- **unitree_rl_gym and legged_gym:** best treated as reference code and for their sim2sim/deploy scripts. Isaac Gym is deprecated in favor of Isaac Lab (general field knowledge; not re-verified here).
- **rsl_rl:** the PPO library used by legged_gym and Isaac Lab, and likely by mjlab. Not verified here.

### Gaps
- Booster Gym, the ToddlerBot codebase, HumanoidVerse, LocoMuJoCo and ProtoMotions were not researched in this pass; no sources were fetched on their status, Windows support or robots.
- Isaac Gym's Windows status was not confirmed (believed Linux-only).
- mjlab's Windows status is not stated in its README.

## Cloud GPU costs

### Takeaway
As of Aug–Oct 2026, an RTX 4090 rents for about $0.13–$0.89/hr: the Vast.ai marketplace is at the low end, and RunPod Community/Secure Cloud runs about $0.34–$0.89. An H100 costs about $1.49–$2.21/hr on the Vast.ai marketplace and about $2.89–$4.29/hr on RunPod Secure or Lambda. Because RoboStriker-class work ran on one 4090, a 4090 rental is the cost-efficient default.

### Cited Findings
- **RTX 4090 (Sep 2026):** Vast.ai from $0.14/hr, Hostinger $0.38/hr, RunPod Community $0.34/hr and RunPod Secure $0.74/hr. — [Hostinger](https://www.hostinger.com/tutorials/?p=158956)
- **RunPod Secure RTX 4090:** $0.89/hr (30 Oct 2026). Vast.ai prices are host-set and variable. — [Stork](https://www.stork.ai/blog/runpod-alternatives-gpu-cloud-2026)
- **August 2026 figures:** RunPod Secure 4090 at $0.69/hr, Vast.ai floor at $0.13/hr, and Vast.ai H100 SXM typically $1.49–$2.21/hr. — [tech-insider](https://tech-insider.org/runpod-vs-lambda-vs-vast-ai-2026/)
- **H100 on RunPod Secure:** PCIe $2.89/hr and SXM $3.99/hr. Lambda H100 SXM is $3.99–$4.29/hr. — [Stork](https://www.stork.ai/blog/runpod-alternatives-gpu-cloud-2026)
- **Isaac Lab on major clouds:** deployable on AWS, GCP, Azure or Alibaba via Isaac Automator. — [Isaac Lab install](https://isaac-sim.github.io/IsaacLab/main/source/isaaclab/installation.html)

### Inferences
- **Example costs:** a 15-minute locomotion run costs well under $1. A multi-day self-play run on a rented 4090 might cost about $10–$50. This is an estimate from the hourly rates above.
- **Hardware choice:** for small-scale PPO with thousands of environments, a 4090 or 5090 is usually more cost-effective than an H100.

### Gaps
- No RTX 5090 cloud prices were collected.
- These are aggregator snapshots, not live provider pages, and the sources conflict slightly on RunPod's 4090 price ($0.69 vs $0.74 vs $0.89).
