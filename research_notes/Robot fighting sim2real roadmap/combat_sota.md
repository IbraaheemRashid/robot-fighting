# Learned Combat / Contact-Rich Skills for Humanoid & Legged Robots, and Real-World Robot Fighting Events (state as of Nov 2026)

Legend: [HW] = shown on real hardware, [SIM] = simulation only, [TELEOP] = a human pilots the robot live, [SCRIPTED] = pre-recorded/pre-programmed motion playback.

## Q1. Academic work on RL/learned fighting and competitive physical agents

### Takeaway
Learned competitive physical agents have a long history in simulation (OpenAI sumo, 2017; Won et al. boxing/fencing, 2021). On hardware, DeepMind's OP3 1v1 soccer (2024) is the clearest learned competitive self-play result. The first autonomous humanoid *boxing* with sim-to-real transfer appeared in 2026: RoboStriker on the Unitree G1. The standard pipeline is now a three-stage recipe: (1) imitate human mocap with a motion tracker, (2) compress the skills into a latent space, (3) run self-play over that latent space for tactics.

### Cited Findings
- **OpenAI, "Emergent Complexity via Multi-Agent Competition"** (Bansal, Pachocki, Sidor, Sutskever, Mordatch; arXiv Oct 10 2017, ICLR 2018) [SIM, MuJoCo]. The paper argues that self-play competition produces behavior much more complex than the environment and gives a natural curriculum, because each agent faces an opponent of matching skill. Learned skills included running, blocking, ducking, tackling, fooling opponents, kicking, and defending with arms and legs — [arXiv](https://arxiv.org/abs/1710.03748)
  - Sumo details (secondhand): the goal is to knock the opponent down or push it out of a round arena. Humanoids learned a stable stance and head-butted opponents. A sumo-trained agent stayed balanced under strong "wind" it had never seen in training — [Colyer summary](https://blog.acolyer.org/2018/01/11/emergent-complexity-via-multi-agent-competition/)
- **Won, Gopinath, Hodgins, "Control Strategies for Physically Simulated Characters Performing Two-player Competitive Sports"** (SIGGRAPH / ACM TOG 2021, Meta) [SIM]. Boxing and fencing between full-DoF physically simulated characters. The policy is an encoder-decoder with an autoregressive latent and a mixture-of-experts decoder. Training has two steps: basic skills via imitation first, then bout-level strategy via deep RL. The paper reports tactical and natural-looking behavior and includes ablations — [Meta AI](https://ai.meta.com/research/publications/control-strategies-for-physically-simulated-characters-performing-two-player-competitive-sports/). A 2024 follow-up (Wang, Hodgins, Won) applies strategy and skill learning to simulated table tennis — [arXiv 2407.16210](https://arxiv.org/html/2407.16210v1)
- **DeepMind, "Learning Agile Soccer Skills for a Bipedal Robot with Deep RL"** (Haarnoja et al., Science Robotics, Apr 10 2024; arXiv 2304.13653) [HW]. Robotis OP3 miniature humanoids (20 actuated joints) played 1v1 soccer.
  - Training ran in MuJoCo and transferred zero-shot to the real robots. Transfer relied on high-frequency control, targeted dynamics randomization, and perturbations during training. Basic behavior regularization kept the fragile robots safe.
  - Versus a scripted baseline: walking 181% faster, turning 302% faster, getting up 63% faster, ball kicked 34% faster. The agent learned fall recovery, walking, turning, kicking, and shot-blocking, and moved smoothly between them — [arXiv](https://arxiv.org/abs/2304.13653)
  - Training had two stages: separate skill policies were trained first, then distilled into one agent that improved through self-play against earlier versions of itself. Early 2v2 tests showed division of labor — [project page](https://sites.google.com/view/op3-soccer?pli=1); [DeepMind](https://deepmind.google/research/publications/31284/)
  - Joonho Lee (ETH) on this work: "We've never seen such resilience from humanoids." — [Science News](https://sciencenews.org/article/reinforcement-learn-ai-humanoid-robots)
- **RoboStriker: Hierarchical Decision-Making for Autonomous Humanoid Boxing** (Yin, Cao, Dong, Zeng, Zhang, Zhang, Wang, Pang, Zhou, Zhang; arXiv 2601.22517, Jan 30 2026) [SIM + claimed HW]. Three stages:
  1. A single-agent motion tracker is trained on human boxing mocap.
  2. The skills are distilled into a latent manifold projected onto a unit hypersphere, which restricts exploration to physically plausible motions.
  3. Latent-Space Neural Fictitious Self-Play (LS-NFSP) learns tactics in latent space instead of raw motor space, which the authors say stabilizes multi-agent training.
  The abstract claims sim-to-real transfer — [arXiv](https://arxiv.org/abs/2601.22517). An aggregator lists a later version, "Latent-Space Strategic Games for Autonomous Humanoid Boxing" (arXiv 2608.16195, Aug 2026), on Unitree G1 with real-robot deployment — [roboticscenter.ai](https://www.roboticscenter.ai/research/papers/robostriker-latent-space-strategic-games-for-autonomous-humanoid-boxing-2608) (not verified against arXiv).
- **HoloBoxing** (preprint, ~Oct 2026) [SIM, unverified]. Per a secondary summary, it trains a boxer in about 7 minutes on one GPU, on top of a frozen pretrained whole-body controller. Simulation only, with no real-robot results or baselines in the abstract — [AI Weekly](https://aiweekly.co/alerts/holoboxing-trains-humanoid-boxer-in-7-minutes-on-one-gpu)
- **ASAP** (He et al., CMU + NVIDIA, arXiv 2502.01143, Feb 2025) [HW, G1]. Motion-tracking policies are pre-trained in simulation. A delta-action model is then fitted to real rollouts and used to fine-tune the policy, which closes the sim-to-real dynamics gap. Results include forward leaps over 1 m on the 1.35 m G1, and kicks were one of the two benchmark tasks. **Hardware cost:** motors often overheated, and two robots were damaged during data collection — [arXiv](https://arxiv.org/abs/2502.01143); [The Decoder](https://the-decoder.com/nvidia-researchers-demonstrate-significant-progress-in-robot-training/)
- **BeyondMimic** (arXiv 2508.08241, Aug 2025) [HW, G1]. A motion-tracking framework validated in sim and on a real G1. Clips include sprinting, turning jumps, and crawling (LAFAN1), and guided diffusion composes the skills — [arXiv](https://arxiv.org/pdf/2508.08241)
- **"A Kung Fu Athlete Bot That Can Do It All Day"** (arXiv 2602.13656, Feb 2026) [HW, G1 29-DoF]. Introduces a dataset of highly dynamic, balance-challenging kung fu motions. It extends BeyondMimic with a fall-recovery objective, trains in Isaac Sim 5.0, and evaluates in MuJoCo. The ablation shows fall recovery significantly improves fall handling and balance — [arXiv](https://arxiv.org/pdf/2602.13656)

### Inferences
- The proven stack for a learned fighter is:
  - human mocap, then a whole-body tracking policy (BeyondMimic/ASAP-style, on G1-class hardware);
  - a latent skill space;
  - self-play (NFSP / PFSP) for tactics;
  - a dedicated fall-recovery or get-up objective.
- This mirrors Won et al. (2021) and OP3 soccer (2024), and was transferred to boxing in RoboStriker (2026).
- OP3 soccer is the closest analogue to "small fighting robots". Small, cheap humanoids tolerate falls far better than full-size ones, and the RL policy got up 63% faster than the scripted baseline. That is direct evidence that learned get-up skills transfer to small hardware.
- For a person-controlled Big Hero 6 bot, the realistic architecture is a hybrid: the human picks high-level commands (strike type, direction, guard), and a learned low-level policy handles balance, striking motion, and recovery. RoboStriker's latent action space is exactly the kind of interface a joystick or gesture controller could drive.

### Gaps
- No quantitative RoboStriker hardware results (win rates, fall rates, duration of real bouts) were retrieved. The 2608 version was not verified on arXiv.
- HoloBoxing details are from a secondary summary only.
- Not researched in depth: ASE/AMP/CALM/PHC-specific combat demos (ASE/CALM are known to include sword and shield strike skills in simulation, but this was not re-verified here), and HoST fall recovery (no source found in this search).
- I found no published learned *contact-wrestling* or *sumo* results on real legged robots.

## Q2. Real-world humanoid fighting demos and leagues — control method and what broke

### Takeaway
Every public humanoid fight event retrieved (2025–2026) was **teleoperated** by human pilots, using joystick/remote, VR, or mocap with voice commands. The robots run onboard balance and locomotion controllers, plus motion libraries learned from mocap of human fighters. Falls are frequent and often need human help, and hardware damage is common (e.g., a head knocked off). Commentators call much of it "robot theater".

### Cited Findings
**CMG World Robot Competition – Mecha Fighting Series / Unitree "Iron Fist King: Awakening"** (Hangzhou, ~May 25 2025) [TELEOP, Unitree G1]
- Billed as the first humanoid robot fighting tournament: Unitree G1 robots controlled by people, with four operator teams — [Henan gov](https://english.henan.gov.cn/2025/05-26/3162232.html); [eHangzhou](https://www.ehangzhou.gov.cn/2025-05/26/c_293755.htm)
- Scoring: punch = 1 point, kick = 3; a fall costs 5 points; failing to rise within 8 seconds costs 10 points and ends the round (as summarized from coverage)
- Control accounts differ:
  - Asia Times: trainers used joysticks remotely — [Asia Times](https://asiatimes.com/?p=920009)
  - eWeek: remote devices plus voice commands — [eWeek](https://www.eweek.com/news/unitree-robot-combat-2026-apac-china/)
  - Ziegler newsletter: motion sensors, direct mocap, and voice — [Ziegler](https://ziegler.substack.com/p/ep64-worlds-first-humanoid-robot)
  - Unitree's Wang Qixin said they captured movement data from professional kickboxers and the robots learned those movements in the virtual world (i.e., sim-trained motion library) — summarized across the same coverage
- PopSci [TELEOP]: four G1s (about 4.3 ft) were remotely controlled by engineers, with brief autonomous, pre-programmed actions — [PopSci](https://www.popsci.com/?p=701631)
- **What broke (PopSci):**
  - "Silk Artisan" fell into an accidental split after a kick to the shin.
  - "AI Strategist" tripped over its opponent's head and face-planted, then got up and won.
  - In the final, AI Strategist beat "Energy Guardian" by knockout in all three rounds — [PopSci](https://www.popsci.com/?p=701631)
- Follow-up: Unitree released a CMG 2026 teaser of robots fighting robots and humans. As of the coverage, no date, venue, or rules had been announced — [eWeek](https://www.eweek.com/news/unitree-robot-combat-2026-apac-china/)

**World Humanoid Robot Games** (Beijing, Aug 14–17 2025)
- 280 teams and 500+ humanoids in 26 events, including boxing/kickboxing — [Wikipedia](https://en.wikipedia.org/wiki/World_Humanoid_Robot_Games); [News on Air](https://www.newsonair.gov.in/500-humanoid-robots-compete-at-world-robot-games-in-beijing)
- A Xinhua caption shows a humanoid kickboxing match on Aug 15 2025 — summarized via [CGTN](https://news.cgtn.com/news/2025-08-15/Cutting-edge-tech-on-display-at-2025-World-Humanoid-Robot-Games-1FQNUH9ccUM/index.html)
- Critical coverage: "Box, run, crash" (Guardian headline as cited) — [InsideHook](https://www.insidehook.com/sports/did-robot-olympics-go-world-humanoid-robot-games)

**REK (Robot Entertainment Kombat), San Francisco** [TELEOP via VR]
- Format: VR pilots operate full-contact humanoids — [Deep Tech Week](https://www.deep-tech-week.com/organizations/rek); [eWeek](https://www.eweek.com/news/robot-fight-club-virtual-reality/)
- Founder: Cix Liv. Sources disagree on the founding year (2025 vs 2022) — [PitchBook](https://pitchbook.com/profiles/company/896148-55)
- Feb 2026 event (second REK match in SF; REK1 was at Kezar Pavilion), per [Rest of World](https://restofworld.org/2026/chinese-robot-boxing-unitree-rek/):
  - Robots: modified Unitree G1s (about 80 lb, 4.5 ft)
  - Format: two bouts of five 60-second rounds, a human referee, and tickets at about $60–80
  - Failures: robots "sometimes fell and needed human help to stand back up"
  - Result: a 13-year-old VR pilot won
  - Cost: a basic G1 is upward of $13k; a T800 is more than $40k
  - Ken Goldberg (UC Berkeley) called much of this "robot theater"
- Sept 18 2026: a human-vs-robot cage bout against an EngineAI T800, remotely piloted via VR rather than run by onboard AI — [Interesting Engineering](https://interestingengineering.com/ai-robotics/human-fights-humanoid-robot-in-cage); [ElectronicsForU](https://www.electronicsforu.com/news/humanoid-robot-demonstrates-teleoperated-combat-skills-in-cage)
  - The California State Athletic Commission then issued a cease-and-desist against unlicensed human-vs-robot fights — [Dexerto](https://www.dexerto.com/entertainment/robotics-company-served-cease-and-desist-over-human-vs-robot-fights-3416650/); [LetsDataScience](https://letsdatascience.com/news/california-commission-halts-rek-human-robot-fights-3c76c10d)
- Oct 2026: a planned "humanoid robot weapon fight" with claws and blades — [SF Standard](https://sfstandard.com/2026/10/11/humanoid-robot-weapon-fight-sf/) (outcome not found)
- REK also reports stops in Tokyo and Vancouver, and plans a league of ~200 lb, ~6 ft robots — [Rest of World](https://restofworld.org/2026/chinese-robot-boxing-unitree-rek/)

**EngineAI T800 / URKL ("Ultimate Robot Knock-out Legend")** (Shenzhen Nanshan Cultural & Sports Center, ~July 2026)
- 32 teams (including named teams from Berkeley, Stanford, Tsinghua, Zhejiang, and HKU), all on the standardized T800, each developing its own software, tuning, and protective gear. Prize pool ¥10M (~$1.48M). Freestyle full-contact punches and kicks — [PC Guide](https://www.pcguide.com/pro/news-pro/chinas-engineai-hosts-worlds-first-robot-mma-competition-t800-loses-head-and-keeps-on-fighting/)
- **What broke:** a kick knocked off one robot's head (with its sensors), and the robot kept fighting on its remaining systems — [PC Guide](https://www.pcguide.com/pro/news-pro/chinas-engineai-hosts-worlds-first-robot-mma-competition-t800-loses-head-and-keeps-on-fighting/)
- Control mode at URKL: not stated in the source
- T800 specs: about 1.73 m and 75 kg, 360° LiDAR, stereo vision, joints up to 450 Nm, priced from about $40k — [Interesting Engineering](https://interestingengineering.com/ai-robotics/china-t800-combat-humanoid-robot); [Mike Kalil](https://mikekalil.com/blog/engineai-t800-fights-globally/)
- Aug 2026: about ten T800s fought in Riyadh at "CyberHero" (Hero eSports league) — [Mike Kalil](https://mikekalil.com/blog/engineai-t800-fights-globally/)
- A Russian-language report notes EngineAI was earlier suspected of CGI-altered videos — [Habr](https://habr.com/ru/news/974966/)

**Other leagues:** Ultimate Fighting Bots (UFB) bills itself as the first robot combat league with human pilots — [Rest of World](https://restofworld.org/2026/chinese-robot-boxing-unitree-rek/)

### Inferences
- The commercial "bot fights" model already exists (REK, URKL, CyberHero, UFB, CMG), and it is person-controlled, which matches the Big Hero 6 concept. Its weak points:
  - cost ($13k–$40k+ per robot)
  - falls that need human help
  - damage
  - regulation once humans enter the ring
- Live VR/mocap teleop of full-body motion is the dominant control mode. A key unanswered design question is how much the onboard controller filters or stabilizes the pilot's motion. Coverage suggests a balance controller plus a mocap-trained move library.

### Gaps
- No bout-level results or detailed rules were found for WHRG 2025 combat events.
- How URKL robots were controlled (autonomous or teleop) is unknown.
- The outcome of REK's Oct 2026 weapon fight was not found.
- The CMG 2026 date and rules were unannounced as of the coverage.
- Not researched: Unitree H1 kung fu, Booster T1 fights, and the specific damage rates and repair costs per event.

## Q3. Traditional combat robotics and ROBO-ONE humanoid fighting

### Takeaway
ROBO-ONE (Japan, since Feb 2002) is the long-running precedent for small, RC-servo bipedal humanoids fighting under human remote control. It was knock-down or ring-out based, with weight classes from 2007 and an autonomous division from 2017. Wheeled combat robotics (SPARC rules) offers mature small classes (150 g, 1 lb, 3 lb) that are cheap and safe for hobby fights.

### Cited Findings
**ROBO-ONE** [TELEOP (RC); autonomous division separate]
- First contest Feb 2 2002 in Japan. A win requires felling the opponent or knocking it out of the ring; one source says at least 3 times — [Wikipedia](https://en.wikipedia.org/wiki/Robo_One)
- Pre-screening: walk 10 steps in 10 s and rise from both supine and prone positions. A 2-min demonstration is judged by a panel. Matches are loosely K-1 style. Over the years, rules tightened foot/sole size relative to robot size, which limits wide stable feet — [Wikipedia](https://en.wikipedia.org/wiki/Robo_One)
- Hardware: RC servos, sheet aluminum (also steel, plastic, or wood) frames, balance sensors, and a small onboard microcontroller. Common kits: Kondo KHR-1/2/3HV, Robotis Bioloid, Hitec Robonova-1, Kyosho Manoi — [Wikipedia](https://en.wikipedia.org/wiki/Robo_One)
- Weight classes introduced at ROBO-ONE 11 (Mar 2007) — [Wikipedia](https://en.wikipedia.org/wiki/Robo_One)
  - A Japanese source describes the planned split as <3 kg, 3–5 kg, and >5 kg — [Weblio](https://www.weblio.jp/content/robo1)
  - The earlier rule was under 3 kg and bipedal — [Vice](https://motherboard.vice.com/read/watch-the-championship-round-of-japanese-hobby-robot-wrestling)
- An autonomous biped fighting event ("ROBO-ONE auto") was added Feb 25 2017 — [Wikipedia](https://en.wikipedia.org/wiki/Robo_One); [Interesting Engineering](https://interestingengineering.com/innovation/robo-one-battles-fully-autonomous-robots-fighting)
- Official rules are published in Japanese about 2 months before each event (robo-one.com) — [Wikipedia](https://en.wikipedia.org/wiki/Robo_One)
- The 36th ROBO-ONE was held Feb 9 2020 at Miraikan, Tokyo — [Wikipedia](https://en.wikipedia.org/wiki/Robo_One)

**Wheeled combat robotics (SPARC / BattleBots lineage)**
- SPARC classes: Fairyweight 150 g (0.33 lb), Antweight 1 lb / 454 g, Beetleweight 3 lb / 1.36 kg (1.5 kg in the UK) — [SPARC Tournament Procedures](https://www.sparc.tools/SPARC_Tournament_Procedures_v1.1.pdf); [OSU](https://osurobotics.club/teams/combat/sparc); [Make](https://makezine.com/article/technology/robotics/know-your-combat-robots-a-field-guide-to-competition-weight-classes-and-weapons/)
- A "plastic antweight" variant requires all-plastic structure except electronics, motors, and fasteners — [OSU](https://clubs.oregonstate.edu/combat-robotics/sparc)
- Events can override SPARC rules if they publish the changes in advance — [SPARC](https://www.sparc.tools/SPARC_Tournament_Procedures_v1.1.pdf)
- RoboGames publishes unified sumo rules — [RoboGames](https://robogames.net/rules/all-sumo.php)

### Inferences
- ROBO-ONE's rules (the get-up requirement, a ring-out/knock-down win condition, servo-based kits of roughly 1–3 kg) are a ready-made template for a small, person-controlled humanoid fight format. Its 20+ years of hobby history show small servo humanoids can survive repeated falls.
- For a small-scale project, a beetleweight-style mass budget (around 1.5–3 kg) with a ROBO-ONE-style knock-down rule is a plausible starting point, and it is far cheaper and safer than G1/T800-class robots.

### Gaps
- Current ROBO-ONE weight-class limits and rulebook (2025–2026) were not retrieved; robo-one.com is in Japanese.
- BattleBots rules (250 lb heavyweight) and Robot Wars rules were not fetched.
- I found no data on ROBO-ONE servo failure rates.

## Q4. Key gaps and open problems

### Takeaway
The recurring failure modes in both research and events are:
- falls, and getting up quickly (rules give about 8 s);
- tracking error and sim-to-real dynamics mismatch for explosive strikes;
- actuator overheating and structural damage;
- lack of autonomy or opponent modeling in live events, which are nearly all teleop.

### Cited Findings
- **Falls:** at CMG 2025 a fall cost 5 points and not rising within 8 s ended the round (coverage summary). Robots did split-falls and face-plants — [PopSci](https://www.popsci.com/?p=701631). REK G1s needed human help to stand — [Rest of World](https://restofworld.org/2026/chinese-robot-boxing-unitree-rek/). Research answer: learned get-up (OP3, 63% faster than scripted) — [arXiv](https://arxiv.org/abs/2304.13653); fall-recovery objective for dynamic kung fu — [arXiv 2602.13656](https://arxiv.org/pdf/2602.13656)
- **Hardware damage/heat:** in ASAP, motors overheated and two G1s were damaged during data collection — [The Decoder](https://the-decoder.com/nvidia-researchers-demonstrate-significant-progress-in-robot-training/). At URKL a T800's head was kicked off — [PC Guide](https://www.pcguide.com/pro/news-pro/chinas-engineai-hosts-worlds-first-robot-mma-competition-t800-loses-head-and-keeps-on-fighting/). DeepMind needed behavior regularization to keep fragile OP3s safe — [arXiv](https://arxiv.org/abs/2304.13653)
- **Sim-to-real for agile skills:** this needs dynamics randomization and perturbations (OP3) or learned delta-action residuals from real data (ASAP) — [arXiv OP3](https://arxiv.org/abs/2304.13653); [arXiv ASAP](https://arxiv.org/abs/2502.01143)
- **Multi-agent training stability:** RoboStriker found raw motor-space self-play unstable and moved it to a latent space — [arXiv](https://arxiv.org/abs/2601.22517)
- **Autonomy/authenticity:** Goldberg says "Beware of what you see in the videos, it isn't quite real" — [Rest of World](https://restofworld.org/2026/chinese-robot-boxing-unitree-rek/). Reports on the T800 cage fight say the footage does not establish the force involved or whether the bout was unscripted — [ElectronicsForU](https://www.electronicsforu.com/news/humanoid-robot-demonstrates-teleoperated-combat-skills-in-cage)
- **Regulation:** California's athletic commission ordered REK to stop unlicensed human-vs-robot fights (Sept 2026) — [LetsDataScience](https://letsdatascience.com/news/california-commission-halts-rek-human-robot-fights-3c76c10d)

### Inferences
- Robot-vs-robot (not human-vs-robot) small-scale fights avoid both the regulatory problem and most of the injury risk.
- Balance under impact remains largely unsolved in published work: the tracking papers measure falls in free motion, not under opponent strikes. Self-play with contact (RoboStriker) and perturbation training (OP3) are the nearest existing answers.
- Opponent modeling on hardware is essentially undemonstrated beyond OP3 soccer self-play and RoboStriker's claimed transfer.

### Gaps
- No quantified data on impact forces, hits-to-failure, or repair cost per bout at any event.
- No published hardware study of humanoid balance recovery under deliberate opponent strikes was found.
- No information on latency or what assistance layers the REK/CMG teleop pipelines use.
