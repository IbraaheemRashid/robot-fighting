# Human Control Interfaces, Shared Autonomy, Game Design and Safety for Piloted Fighting Robots

Scope: how a human could realistically pilot a learned-policy fighting robot (Big Hero 6 style), with latency and bandwidth figures, BCI/EMG realism, game/scoring design, and arena/safety rules. Research date: 2026-11-02. About 17 tool calls; several figures come from single-group preprints and are flagged.

## Teleoperation methods for humanoids (mocap, VR, RGB, gamepad) and latency

### Takeaway
Every real humanoid fight event so far (CMG Hangzhou May 2025, REK San Francisco 2025-2026, the viral Unitree G1 mocap kickboxing clip) uses a human pilot driving a Unitree G1-class robot, with a learned whole-body controller handling balance. Measured end-to-end teleop latency in research systems runs from about 50 ms (ExtremControl, Feb 2026 preprint) to over 700 ms (TWIST). Most VR or RGB systems sit at 180-450 ms, which is too slow to react to strikes. That fits press reports that robots "struggle to dodge blows".

### Cited Findings
**Events and industry practice**
- CMG World Robot Competition, Mecha Fighting Series, Hangzhou, May 2025. Four human-controlled Unitree G1s fought three 2-minute rounds, with winners decided on points. — [Henan gov / Xinhua](https://english.henan.gov.cn/2025/05-26/3162232.html)
- CMG scoring penalties: a team lost 5 points when its robot fell and 10 points when it stayed down for more than 8 seconds. — [Henan gov / Xinhua](https://english.henan.gov.cn/2025/05-26/3162232.html); also reported by [PopSci](https://www.popsci.com/?p=701631)
- Unitree said it trained the fight moves with AI plus motion capture of professional fighters, learned in simulation. It also planned to record data from the remote-controlled bouts for later training. Reports differ on how autonomous the bouts were: the main matches were human-operated, with some semi-autonomous or autonomous use mentioned. — [Interesting Engineering](https://interestingengineering.com/innovation/unitree-plans-robot-boxing-match); [heise](https://heise.de/-10348477); [heise, student test](https://heise.de/-10394610)
- G1 specs: 1.32 m tall, 35 kg. Its main weakness in matches was reacting, meaning dodging and keeping balance after being hit. — [Henan gov](https://english.henan.gov.cn/2025/05-26/3162232.html)
- Viral clip, Christmas 2025 (Bilibili): an engineer in what appears to be an Xsens mocap suit drives a G1, and the robot's mirrored kick hits him in the groin. WinFuture blamed signal delay, while Techeblog said the system behaved as designed and the operator's kick direction caused it. No measured latency is available. Safety lesson: keep pilots outside the robot's reach envelope or behind a barrier. — [Interesting Engineering](https://interestingengineering.com/ai-robotics/humanoid-robot-kicks-teleoperator); [WinFuture](https://winfuture.de/news,155852.html); [Techeblog](https://www.techeblog.com/?p=230559); [Mike Kalil](https://mikekalil.com/blog/unitree-g1-kicks-teleoperator/)
- REK (San Francisco, founded 2022, founder Cix Liv) runs a league of remotely piloted humanoids. Pilots wear VR headsets and "combat controllers" and see a holographic cockpit view. Software handles balance and low-level control. Partner Reflex Arc describes the control as pilot input combined with trained robot motion. — [Runtimewire](https://runtimewire.com/article/human-vs-robot-rek-put-a-human-in-the-ring-with-a-six-foot-humanoid-robot); [ISPR, Sept 2025](https://ispr.info/2025/09/25/futuristic-fight-club-vr-controlled-boxing-humanoid-robots-battle-in-san-francisco/); [eWeek](https://www.eweek.com/de/news/robot-fight-club-virtual-reality/)
- REK details: robots are modified Unitree G1s (a basic G1 is "upward of $13,000"; an EngineAI T800 is ">$40,000"). Tickets cost $60-80. One event ran two bouts of five 60-second rounds with a human referee. A 13-year-old VR pilot won a bout. Robots sometimes fell and needed humans to stand them up. EngineAI launched a separate URKL league with a prize worth about $1.4M. — [Rest of World, Feb 2026](https://restofworld.org/2026/chinese-robot-boxing-unitree-rek/)
- REK timeline: San Francisco fights from 2025, a Kezar Pavilion tournament in Feb 2026, and full-height humanoids (about 200 lb, nearly 6 ft) in Aug 2026. In Sept 2026 REK staged a human-vs-humanoid kickboxing demo against EngineAI T800s. The California State Athletic Commission sent a cease-and-desist, and REK says it will not repeat human-vs-robot bouts. Robot-vs-robot fights remain unaffected. REK also sells a Steam / Meta Quest simulator for training pilots. — [Runtimewire](https://runtimewire.com/article/human-vs-robot-rek-put-a-human-in-the-ring-with-a-six-foot-humanoid-robot); [Kyunghyang](https://www.khan.co.kr/en/article/202610091626007); [Mezha](https://mezha.ua/en/news/california-human-robot-fights-315994/)

**Research teleop systems and latency**
- TWIST (Ze et al., Stanford/SFU, CoRL 2025): OptiTrack mocap at 120 Hz, retargeting at 50 Hz, policy at 50 Hz on an RTX 4090, robot PD at 1000 Hz, on a 29-DoF G1. End-to-end delay is about 0.9 s (about 0.7 s retargeting and goal generation, about 0.2 s policy), roughly measured from video. Motors overheat after 5-10 min of continuous use. No egocentric video feedback is provided, and the paper does not discuss e-stops. — [TWIST arXiv 2505.02833](https://arxiv.org/html/2505.02833v1); [PMLR](https://proceedings.mlr.press/v305/ze25a.html)
- ExtremControl (UMass Amherst, arXiv 2602.11321, Feb 2026) claims about 50 ms end-to-end (about 54 ms with mocap, about 64 ms with VR) on a G1. It skips full-body retargeting and adds velocity feedforward at the low level, which saves about 100 ms. Demos include ping-pong returns, juggling and frisbee catching. — [ExtremControl](https://arxiv.org/html/2602.11321)
- ExtremControl's comparison table, all estimated by optical flow on public videos (not instrumented): CLONE (VR) ~178 ms; OmniH2O (VR) ~185; AMS (mocap) ~201; TWIST2 (VR) ~234; HumanPlus (RGB) ~340; H2O (RGB) ~373; AMO (VR) ~380; SONIC (VR) ~427 (+~100 for RGB); HOMIE (exoskeleton) ~454; TWIST (mocap) >700. — [ExtremControl Table 1](https://arxiv.org/html/2602.11321)
- OmniH2O uses kinematic pose as a universal interface, driven by VR headset, verbal instruction or RGB camera. Its stated limitations are root-odometry dependence and the lack of safety guarantees. — [OmniH2O arXiv 2406.08858](https://arxiv.org/pdf/2406.08858)
- Open-TeleVision (MIT/UCSD) streams head, wrist and hand poses from VR (Vision Pro / Quest) with active stereo vision feedback. Coverage names latency, bandwidth and operator fatigue as open problems. — [arXiv 2407.01512](https://arxiv.org/html/2407.01512v2); [ISPR](https://ispr.info/2024/07/31/open-television-a-new-immersive-vr-based-human-robot-teleoperation-system/)
- TWIST's authors note that RGB pose estimation (HumanPlus) gives poor root position, and VR keypoints (OmniH2O) capture only the upper body. — [TWIST](https://arxiv.org/html/2505.02833v1)

### Inferences
- Human visual reaction time is roughly 150-250 ms (general knowledge, not sourced this session). Adding 200-450 ms of teleop latency makes reactive defence nearly impossible for a pilot. So blocking and dodging should be delegated to the onboard policy (a reflex layer), while the human sets strategy and attack choice.
- Ranking for a small-bot fighting product: mocap or VR full-body mirroring is spectacular but laggy, tiring, and puts the operator at risk if co-located. A gamepad or "combat controller" sending high-level commands (REK-style) is cheaper, lower bandwidth, and pairs naturally with command-conditioned policies.
- Rounds of 60-120 s with breaks match the motor thermal limits observed in TWIST (5-10 min continuous).

### Gaps
- No instrumented (hardware-timestamped) latency for CMG or REK pilot setups was found. Whether CMG pilots used gamepads or mocap suits during matches is not clearly documented in English sources.
- HumanPlus, AMO and TWIST2 primary-paper latency numbers were not checked independently; the figures above come from the ExtremControl table.

## Shared/hybrid autonomy: high-level intent + RL policy executes

### Takeaway
Shared autonomy is the de facto industry design. REK and CMG pilots direct while learned controllers keep the robot balanced. Research gives a clean stack for "intent in, physics-valid motion out": latent skill spaces (ASE, CALM) and the unified masked-goal controller MaskedMimic, which accepts sparse inputs such as joystick root targets, text, or VR head and hand points.

### Cited Findings
- REK: "software handles lower-level functions such as balance while a person directs the machine". Reflex Arc combines pilot input with trained robot motion. — [Runtimewire](https://runtimewire.com/article/human-vs-robot-rek-put-a-human-in-the-ring-with-a-six-foot-humanoid-robot); [ElectronicsForU](https://www.electronicsforu.com/news/humanoid-robot-demonstrates-teleoperated-combat-skills-in-cage)
- ASE (NVIDIA, 2022) combines adversarial imitation with unsupervised RL to learn reusable skill embeddings, which act as an easy-to-control representation for downstream tasks. — [NVIDIA PAR](https://research.nvidia.com/labs/par/tag/imitation-learning/)
- CALM (SIGGRAPH 2023) encodes motions into a latent vector and conditions both the policy and the discriminator on it to avoid mode collapse. Tasks can be directed at the skill level without retraining. — [CALM arXiv 2305.02195](https://arxiv.org/pdf/2305.02195); [SIGGRAPH PDF](https://research.nvidia.com/labs/par/calm/assets/SIGGRAPH2023_CALM.pdf)
- MaskedMimic (NVIDIA, 2024) treats control as motion inpainting from partial goals: joystick-style future root position and heading, text commands, VR head and hand positions, or object and scene targets. One controller covers all of them without task-specific fine-tuning. NVIDIA reports 98.1% success vs PULSE 93.4%, ASE 37.6%, CALM 10.1% (self-reported). — [NVIDIA dev blog](https://developer.nvidia.com/blog/unified-whole-body-control-for-physically-simulated-humanoids/); [AI Matters summary](https://aimatters.co.kr/news-report/ai-report/11464/)
- OmniH2O frames a single kinematic-pose goal interface that teleop (VR/RGB), language, or an autonomous policy can all drive. This is the same goal-space idea, deployed on a real humanoid. — [OmniH2O](https://arxiv.org/pdf/2406.08858)
- SONIC (NVIDIA whole-body controller) is listed with VR and RGB teleop inputs in ExtremControl's comparison. — [ExtremControl](https://arxiv.org/html/2602.11321)

### Inferences
- Recommended fighting stack: (1) a human command layer with discrete moves (jab, cross, hook, kick, block high/low, dodge, step direction) plus a continuous joystick for footwork; (2) a command-conditioned tracking or latent-skill policy (MaskedMimic-style or CALM-style) trained on mocap fight clips; (3) an autonomous reflex layer for balance recovery, fall-safe poses and get-up. This matches what REK and CMG appear to do. It also lets a BCI or EMG input with a few bits per second be viable, because each command selects a whole skill.
- MaskedMimic's sparse-goal design is a good fit for multi-modal input: a VR wrist target for "punch here" plus a joystick root target for movement.

### Gaps
- No public technical description of REK's or Unitree's command-to-skill architecture was found (it is proprietary).
- No found paper applies MaskedMimic or CALM to real-hardware humanoid fighting. Prior work is simulation characters, plus real-world general tracking (OmniH2O, TWIST).

## Brain-computer interfaces / EEG / EMG (the Big Hero 6 headset)

### Takeaway
Consumer or low-cost non-invasive EEG reaches roughly 2-4 discrete commands, about 60-93% accuracy, and around 15-21 bits/min, with decisions taking about 4-5 s each. That is good enough to select a stance or mode, nowhere near fast enough for real-time fighting. The film's "think and the bot moves" headset is hype for non-invasive EEG. Wrist sEMG is far more realistic: Meta's generic sEMG band delivers about 0.9 discrete gestures per second closed-loop with no per-user calibration.

### Cited Findings
- Low-cost (~$3k, 16-channel, open-source) EEG motor-imagery robot teleop: 78% single-day validation accuracy, 75% over 3 days, but only 62% real-world robot command accuracy. — [arXiv 2412.08971](https://arxiv.org/html/2412.08971v1)
- OpenBCI community study with 3 adults driving a mobile robot: 56.4% accuracy for motor imagery vs 72.7% for actual physical movement. — [OpenBCI community](https://openbci.com/community/?p=5803)
- EEG + AR robot grasping (arXiv 2509.20656): motor-imagery group mean 93.1% accuracy, 8.3% false activations, 14.8 bits/min, mean 4.69 ± 1.20 s per command. The best AR-feedback condition reached 21.3 bits/min with 97.2% grasp success (a slow, discrete task). — [arXiv 2509.20656](https://arxiv.org/pdf/2509.20656)
- Consumer-EEG virtual keyboard: 1.55-1.8 words per minute. — [arXiv 2409.00035](https://arxiv.org/pdf/2409.00035)
- Meta / CTRL-labs sEMG wristband (Nature 645, 2025): generic decoders trained on thousands of people reach >90% offline accuracy on held-out users. Closed-loop medians are 0.66 target acquisitions/s, 0.88 discrete gestures/s, and 20.9 wpm handwriting. About 20 min of personal data improves handwriting by about 16%. More than 100 h of sEMG from 300 people has been released. — [Nature paper PDF](https://repository.cshl.edu/id/eprint/42049/1/10.1038.s41586-025-09255-w.pdf); [The Register](https://www.theregister.com/2025/07/23/meta_wristband_ui_control/); [Scimex](https://www.scimex.org/newsfeed/could-you-swap-your-mouse-and-keyboard-for-a-smart-bracelet)
- Meta demoed the neural wrist band at NeurIPS 2025. — [NeurIPS demo](https://neurips.cc/virtual/2025/130020)

### Inferences
- ITR of about 15-20 bits/min is roughly 0.3 bits/s, or one 4-way choice every 6-7 s. A fight needs a decision every 0.3-1 s. EEG could work as a novelty "special move / mode switch" trigger, or for slow focus/attention metrics (e.g. a "power meter"). EMG (forearm gestures mapped to punch types) is the realistic "neural" interface for the Big Hero 6 feel.
- Background knowledge not verified this session: Muse headsets have 4 dry EEG channels, OpenBCI Cyton is 8 channels at 250 Hz, and Emotiv EPOC is 14 channels. Movement and jaw-clench artifacts badly contaminate EEG, and a pilot who is moving or excited makes this worse. Confirm these before publication.
- Invasive BCIs (Neuralink, BrainGate) exceed non-invasive rates but are clinical-only and irrelevant for a hobby or entertainment product.

### Gaps
- No found demo of EEG controlling a fighting robot or a balance-critical humanoid in real time.
- No found latency figures for consumer EMG armbands (e.g. Myo successors) in robot teleop. Meta's closed-loop numbers are for UI tasks.

## Game-like environments, RL competitions and scoring systems

### Takeaway
Existing fighting-RL benchmarks are mostly 2D video games (FightLadder, ICML 2024). Physical robot-fight scoring precedents fall into two families: knockdown/count systems (ROBO-ONE, CMG fall penalties) and judged damage/aggression systems (NHRL's Attack 5 + Condition 6). Hit detection with sensors is not standardised in any event found.

### Cited Findings
- FightLadder (Princeton, ICML 2024) is a competitive MARL benchmark over 5 two-player fighting games. Their general agent beats 12 built-in characters, but non-exploitable two-player agents are hard to train without human demonstrations. Code and videos are public. — [arXiv 2406.02081](https://export.arxiv.org/abs/2406.02081); [PMLR](https://proceedings.mlr.press/v235/li24q.html); [project site](https://sites.google.com/view/fightladder/home)
- ROBO-ONE (Japan, humanoid combat since 2002): a robot loses after 3 downs in a round or by failing to rise within a 10-count. Only intentional attacks count as downs; self-falls ("slips") do not. Rules are published in Japanese about 2 months before each event and change over time. A ROBO-ONE auto division (autonomous) required robots to recognise robots, humans and bottles. — [Wikipedia](https://Www.Wikipedia.org/wiki/Robo_One); [NBC 2004](https://www.nbcnews.com/id/wbna5685333); [ITmedia 2004](https://www.itmedia.co.jp/news/article/0402/13/1040213068/2); [MONOist 2019 auto](https://monoist.itmedia.co.jp/mn/articles/1911/21/news012.html)
- RoboGames humanoid wrestling awards 2 points to the opponent per knockdown, and a robot must walk 2 steps after a down before attacking. — [RoboGames](https://robogames.net/rules/r1-wrestling.php)
- CMG mecha fighting: points-based over three 2-min rounds, -5 for a fall, -10 for being down more than 8 s. — [Henan gov](https://english.henan.gov.cn/2025/05-26/3162232.html)
- NHRL 2026: 3-min fights; KO via a 10-s referee count on an immobile robot; tap-out counts as KO; pins limited to 10 s. Judges score Attack (5) + Condition (6), with condition levels for Drive (8/6/4/2/0), Weapon (8/6/4/2/0) and Rest of bot (4/3/2/1/0), and a 2-of-3 judges majority. A 20-s "prove-it" period follows each fight. — [NHRL Rules 2026](https://wiki.nhrl.io/wiki/index.php/NHRL_Rules)

### Inferences
- For a learned-policy fighting game, a hybrid score works well: (a) sensor-verified hits (IMU impulse thresholds or force-sensitive/piezo pads on head and torso targets, with debouncing; analogous to fencing or taekwondo electronic scoring, which was not researched here), (b) knockdowns per ROBO-ONE rules (intentional strike only, with 10-count), and (c) a fall penalty à la CMG. Sim environments should implement the same scoring function as reward so sim-trained policies optimise the real rules.
- The FightLadder finding (exploitable agents without human data) argues for seeding self-play with human pilot data. Unitree's plan to record piloted bouts for training is the same idea.

### Gaps
- No MuJoCo or Isaac humanoid boxing environment, RoboSumo results, or NeurIPS robot-fighting competition surfaced in searches. These need a separate search (e.g. OpenAI "RoboSumo" 2017, the Isaac Gym/ASE sword-and-shield "strike" task).
- No current official ROBO-ONE rulebook (Japanese) was retrieved. No event found uses electronic hit sensors for scoring.

## Safety: arenas, energy limits, kill switches, batteries, policy safeguards, regulation

### Takeaway
Combat-robotics practice (NHRL, SPARC) gives directly reusable rules: one-hand master power cutoff, signal-loss failsafe, attended balance-charging of LiPos, a fire-safe cart, and multi-layer polycarbonate walls scaled by weight class and tip speed. Humanoid-specific additions are torque and thermal limits, fall detection, and keeping pilots outside strike range. Human-vs-robot bouts have drawn regulatory action in California.

### Cited Findings
- NHRL 2026 master cutoff: a switch or removable link reachable without disassembly, actuated with one hand, deactivating within 15 s. 12 and 30 lb bots need a dedicated strain-relieved link; 3 lb bots may use the battery connector. — [NHRL Rules](https://wiki.nhrl.io/wiki/index.php/NHRL_Rules)
- NHRL failsafe: drive must stop within 30 s of signal loss or transmitter off, weapons within 60 s, flame within 30 s. Each bot or segment needs a dedicated receiver, and autonomous bots must keep radio enable/disable. — [NHRL Rules](https://wiki.nhrl.io/wiki/index.php/NHRL_Rules)
- NHRL batteries: maximum 75 V nominal without approval. Lithium packs must be balance-charged, and charging must always be attended (unattended LiPo earns a demerit; 3 demerits mean DQ). Robots must fit a fire-safety cart of about 23x23x44 in. — [NHRL Rules](https://wiki.nhrl.io/wiki/index.php/NHRL_Rules)
- NHRL weapons: tip speed above 300 mph needs design approval. Weapon locks must limit rotation to 180° (KE) or 15° (non-KE). Weapons and drive may not be used with the cage open. — [NHRL Rules](https://wiki.nhrl.io/wiki/index.php/NHRL_Rules)
- NHRL weight classes are 3, 12 and 30 lb. Walker bonus: +3, +12 and +30 lb (that is, a walker gets double weight, with pre-approval). Non-traditional locomotion bonus: +1.5, +6 and +15 lb. Older rules (2023/2024) used different 3 lb maxima. — [NHRL Rules 2026](https://wiki.nhrl.io/wiki/index.php/NHRL_Rules); [NHRL 2024](https://wiki.nhrl.io/wiki/index.php/NHRL_Rules_-_2024)
- SPARC arena guidance: polycarbonate is the wall material of choice, and two thinner layers are preferred over one thick sheet. v1.0 lists about 1/4" equivalent for insect classes and 1/2" for up to 30 lb. v1.1 lists 1.5" equivalent as common for up to 220 lb (or 1" with tip-speed or weapon limits). A 1/16" + 1/4" roof is no longer adequate for modern 30 lb bots. — [SPARC v1.0](https://sparc.tools/SPARC_Arena_Construction_Best_Practices_v1.0.pdf); [SPARC v1.1](https://sparc.tools/SPARC_Arena_Construction_Best_Practices_v1.1.pdf)
- Some events lower tip speed further, e.g. a BYU beetleweight 250 mph limit. — [BYU rules](https://combatrobotics.byu.edu/rules)
- TWIST observed motor overheating after 5-10 min, and its authors did not describe e-stops. OmniH2O lists the lack of safety guarantees as a limitation. — [TWIST](https://arxiv.org/html/2505.02833v1); [OmniH2O](https://arxiv.org/pdf/2406.08858)
- Operator injury risk: a mocap-mirrored G1 kick struck its own operator (Dec 2025). — [Interesting Engineering](https://interestingengineering.com/ai-robotics/humanoid-robot-kicks-teleoperator)
- Regulation: the California State Athletic Commission issued a cease-and-desist over REK's Sept 2026 human-vs-robot bout. Robot-vs-robot bouts are not affected. — [Kyunghyang](https://www.khan.co.kr/en/article/202610091626007); [Runtimewire](https://runtimewire.com/article/human-vs-robot-rek-put-a-human-in-the-ring-with-a-six-foot-humanoid-robot)

### Inferences
- Policy-level safeguards for a small fighting humanoid, my own recommendations drawn from the above: per-joint torque and velocity clamps below hardware maxima; a punch-energy budget (cap end-effector speed near the opponent); thermal derating and round time limits; IMU fall detection that triggers a compliant fall pose and then the get-up skill; an action filter that rejects commands outside the trained distribution; and a hardware e-stop independent of the policy computer (a radio relay cutting motor power), mirroring NHRL's dedicated-receiver rule.
- Pilots should stand outside the arena behind polycarbonate. A walker-only, no-spinner ruleset lets a small-humanoid league use insect-class arena walls (about 1/4" to 1/2" polycarbonate), since fist strikes carry far less energy than spinners.
- For insurance, events usually require waivers and venue liability cover. I found no specific insurance figures.

### Gaps
- SPARC's current robot construction spec (beyond arena best practices) and insurance requirements were not retrieved. Specific insurance terms for NHRL or REK were not found.
- No public standard was found for humanoid fighting robots: punch energy limits, protective padding requirements, or approval processes.
