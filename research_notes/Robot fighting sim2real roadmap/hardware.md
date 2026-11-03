# Hardware Platforms for Small Fighting Robots with Sim2Real Learned Control

Research date: 2026-11-02. All prices are USD unless noted. Most figures come from retailer listings or secondary press, and many conflict across regions because of shipping, tariffs and currency conversion. Confirm any price before buying.

## 1. Small/cheap humanoids and servo bots: prices and specs (late 2026)

### Takeaway
The budget humanoid market changed sharply in 2025-2026. Chinese mass-market humanoids now cost about $1.4k (Noetix Bumi), $4-7k (Unitree R1, Booster K1) and $13.5k+ (Unitree G1 base). Open-source research humanoids (ToddlerBot under $6k, Berkeley Humanoid Lite under $5k) are the strongest fit for sim2real because they ship with sim models and training code. Legacy servo humanoids (ROBOTIS OP3 at about $13k, Kondo KHR-3HV at about €2k) cost more for what they can do.

### Cited Findings
**Open-source research humanoids**
- ToddlerBot (Stanford, Shi, Wang, Song, C. Karen Liu; CoRL 2025, arXiv 2502.00893). Under $6,000 total, with about 90% of that going to motors and the computer. 0.56 m tall, 3.4 kg, 30 active DoF (7 per arm, 6 per leg, 2 neck, 2 waist). Fully 3D-printed plus off-the-shelf parts; code is MIT-licensed. — [arXiv v4](https://arxiv.org/html/2502.00893v4); [NSF PAR](https://par.nsf.gov//servlets/purl/10633391); [Gitee mirror](https://gitee.com/iggi128/toddlerbot)
- ToddlerBot actuators are all Dynamixel: XC330-T288 (1.0 Nm stall), XC430-T240BB (1.9 Nm), XM430-W210 (3.0 Nm, used at knee and ankle pitch), 2XL430-W250 (1.5 Nm) and 2XC430-W250 (1.8 Nm). Compute is a Jetson Orin NX 16GB; the RL policy runs on the CPU at 50 Hz. — [arXiv v4](https://arxiv.org/html/2502.00893v4)
- ToddlerBot sim: MuJoCo/MJX plus Brax, PPO for 3×10^8 steps with 1,024 environments. Motor system ID (damping, armature, friction loss, 9-parameter torque-limit model) gives 1.3° average tracking error. Policies transferred to a second robot without re-running sysID. Sim-to-real walking tracking error was 0.082 m in sim vs 0.133 ± 0.018 m in real. — [arXiv v4](https://arxiv.org/html/2502.00893v4)
- A 2026 high-school RL curriculum built on ToddlerBot notes that parts cost under $6k, but fabrication, repairs, compute and mentoring add to the real delivery cost. — [arXiv 2609.25674](https://arxiv.org/pdf/2609.25674)
- Berkeley Humanoid Lite (UC Berkeley, arXiv 2504.17249). Complete hardware cost under about $5,000. Its 22 actuators each combine a BLDC motor, a 3D-printed cycloidal gearbox and a magnetic encoder; the gearboxes print on a 200×200×200 mm FDM printer. Hardware, embedded code and training/deployment frameworks are all open (lite.berkeley-humanoid.org). It is a reference design, not a product for sale. — [humanoid.guide](https://humanoid.guide/berkeley-humanoid-lite-uses-diy-actuators-to-cut-costs/); [R&D World](https://www.rdworldonline.com/?p=78112); [Berkeley Engineering](https://engineering.berkeley.edu/news/2025/11/building-bots-on-a-budget/); [arXiv](https://arxiv.org/pdf/2504.17249); [Open Source For You](https://www.opensourceforu.com/2026/08/berkeley-humanoid-lite-cuts-robot-costs-with-diy-actuators/)
- Hugging Face/Pollen HopeJR: expected price of about $3,000, depending on tariffs, announced May 2025. Reachy Mini: from just under $300. Reachy Mini is a desktop robot (head and antennas), so it is not useful for fighting. About 1,100-1,700 units sold. — [Notebookcheck](https://www.notebookcheck.net/Affordable-robotics-Hugging-Face-introduces-3-000-humanoid-and-300-desktop-robot.1029422.0.html); [eWeek](https://www.eweek.com/news/hugging-face-robots-reachy-mini-hopejr/); [Mike Kalil](https://mikekalil.com/blog/hugging-face-reachy-mini/)
- K-Scale Labs (K-Bot, Z-Bot, Zeroth) has shut down. aiwiki lists K-Bot as discontinued after the company closed in November 2025; per The Information, the company failed to raise $10-15M and planned to refund pre-orders. Earlier quoted prices were K-Bot Founder's Edition $8,999, second batch $10,999 (one report said $8,000), and Z-Bot $999. The open-source repos may survive, but there is no vendor. — [aiwiki](https://aiwiki.ai/wiki/k_scale_k_bot); [cnyes](https://news.cnyes.com/news/id/6238938); [Automate.org](https://www.automate.org/robotics/industry-insights/this-open-source-legged-humanoid-robot-is-available-to-order-at-8-000/anc); [Automate.org Z-Bot](https://www.automate.org/robotics/industry-insights/k-scale-is-building-a-sub-1-000-humanoid-research-robot/anc); [Humanoid Reality Index](https://theresarobotforthat.com/humanoid-reality-index/kscale-kbot/)

**Commercial Chinese low-cost humanoids**
- Noetix Bumi: launch price 9,998 CNY, about $1,370-1,400, marketed as "cheaper than an iPhone". International purchase goes through a "contact consultant" process rather than a posted retail price. — [TechNode](https://technode.com/2025/10/27/after-new-funding-noetix-robotics-explains-how-it-built-a-humanoid-robot-cheaper-than-an-iphone); [Interesting Engineering](https://interestingengineering.com/innovation/bumi-worlds-cheapest-humanoid-robot); [Mike Kalil](https://mikekalil.com/blog/booster-noetix-cheap-humanoid-robots/)
- Booster K1 (kid-size): sources disagree. Starting prices of $4,999 and $5,999 are both reported, plus a 29,900 CNY (about $4,200) limited-time price in China. Booster reported more than 1,000 K1 orders and a price in the 600k JPY range, alongside a new funding round of more than ¥2B. — [Mike Kalil](https://mikekalil.com/blog/booster-noetix-cheap-humanoid-robots/); [36Kr Japan](https://36kr.jp/447223/)
- Booster T1: one EU retailer lists variants at €51,590 and €63,690. — [OpenELAB](https://openelab.io/collections/brands-booster)
- Unitree R1: announced July 2025, with R1 Air at $4,900 and standard R1 at $5,900. About 25 kg; can cartwheel and kick. International sales opened on AliExpress around April 2026 from 29,900 CNY (about $4,370), and one report gives about $6,800 delivered to the US. In June 2026 the China price was cut from 39,900 to 29,900 CNY. — [VnExpress](https://e.vnexpress.net/news/tech/enterprises/unitree-starts-selling-cheapest-humanoid-robot-from-6-800-5063068.html); [VnExpress launch](https://e.vnexpress.net/news/tech/tech-news/unitree-to-launch-cheapest-humanoid-robot-globally-next-week-starting-at-over-4-000-5060948.html); [Automate.org](https://www.automate.org/robotics/industry-insights/unitrees-55-pound-humanoid-costs-6-000-can-cartwheel/anc); [ehangzhou.gov.cn](https://www.ehangzhou.gov.cn/2026-06/26/c_298239.htm); [Engadget](https://www.engadget.com/ai/this-humanoid-robot-can-do-cartwheels-handstands-and-roundhouse-kicks-at-less-than-6000-184500276)
- Unitree G1: base model $13,500 on Unitree's global store as of Aug 2026, backordered, plus $300-1,200 shipping and duties. EDU versions are quote-only. One dealer guide lists Edu Plus at $53,900, Edu Pro at $45,900-56,900 and an "Edu Combat (Boxing)" build at $63,900; the same guide gives the base as $21,600, which conflicts with $13,500. — [theresarobotforthat](https://theresarobotforthat.com/blog/unitree-g1-price/); [globaldronehq](https://globaldronehq.com/blogs/guides/unitree-robot-price-guide-2026); [globaldronehq G1 combat](https://globaldronehq.com/products/unitree-g1-edu-combat-boxing-humanoid-robot)
- EngineAI PM01 is 138 cm tall. No price found. — [Wikipedia](https://en.wikipedia.org/wiki/Engine_AI)

**Legacy servo humanoids**
- ROBOTIS OP3: about $13,000, or €12,985 including VAT at Funduinoshop. An older Robotlab listing shows $11,000. — [thinkrobotics](https://thinkrobotics.com/ar/blogs/product-reviews-buying-guides/humanoid-robot-price-guide-what-different-models-cost-in-2025); [Funduinoshop](https://funduinoshop.com/en/robotics/robotis/robotik/robotis-op3); [Robotlab](https://robotlab.com/store/darwin-op3-robot); [ROBOTIS e-manual](https://emanual.robotis.com/docs/en/platform/op3/introduction/)
- Kondo KHR-3HV Ver.3 standard set: about €2,050 new on eBay.de. The 2009 price was $1,300, so the old figure is not current. This is the classic ROBO-ONE class kit. — [eBay.de](https://www.ebay.de/itm/225517152664); [TechCrunch 2009](https://techcrunch.com/?p=190005)
- Hiwonder AiNex: ₹99,700 on sale (list ₹120,000) at ThinkRobotics India. No USD price found. — [thinkrobotics](https://thinkrobotics.com/collections/hiwonder-ros)

### Inferences
- For a sparring project, the realistic tiers look like this:
  - **About $100-500:** servo kits and combat-bot kits.
  - **About $1.4k:** Bumi, but closed and with an unclear SDK.
  - **About $3-6k:** ToddlerBot, Berkeley Humanoid Lite, HopeJR, R1 or K1.
  - **About $13.5-20k:** G1 base.
  - **About $45-75k:** G1 EDU, needed for low-level SDK access to train your own policies.
  - **About $50-65k:** Booster T1.
- ToddlerBot and Berkeley Humanoid Lite cost less to repair because parts are printed. The ToddlerBot team has already done the motor sysID needed for zero-shot transfer.
- Before buying, check SDK and low-level joint access on the R1, K1 and Bumi. Cheap consumer SKUs may not expose torque/PD control. The G1 base SKU reportedly lacks SDK/API access, as the mydealz listing title "kein SDK/API" ("no SDK/API") suggests: [mydealz](https://www.mydealz.de/deals/unitree-g1-humanoider-roboter-130cm-23-freiheitsgrade-ki-antrieb-3d-lidar-tiefenkamera-2h-laufzeit-kein-sdkapi-2735169).

### Gaps
- No current prices found for ROBOTIS Bioloid/Mini, UBTECH Alpha, Poppy, InMoov, Hiwonder TonyPi (USD), EngineAI PM01 or Booster T1 in the US.
- No confirmed HopeJR shipping status or price as of late 2026.
- Specs of R1, K1 and Bumi (DoF, joint torques, SDK level) were not fetched.
- I did not verify whether the Chinese-vendor robots face export restrictions. US tariffs are mentioned as affecting HopeJR and Jetson pricing.

## 2. Actuators: hobby servos vs quasi-direct-drive (QDD) BLDC

### Takeaway
QDD BLDC actuators (low gear ratio of about 6-10:1, current-based torque control) are backdrivable. That makes them absorb impacts better and makes their dynamics easier to model in sim than high-ratio hobby servos. They now cost about $125-200 per joint for small units (Damiao, RobStride, Steadywin). Smart serial servos (Feetech STS3215 at about $25, Dynamixel) are cheap and easy, but they rely on position control with gear backlash and friction, which then needs careful system ID.

### Cited Findings
- Wensing et al., "Proprioceptive Actuator Design in the MIT Cheetah: Impact Mitigation and High-Bandwidth Physical Interaction" (IEEE T-RO 33(3), 2017). The paper introduces the Impact Mitigation Factor (IMF) to quantify backdrivability at impact. The Cheetah leg's IMF is comparable to quadrupeds that use series springs, and it achieved peak forces over 450 N with contact times down to 85 ms. — [MIT DSpace](https://dspace.mit.edu/handle/1721.1/119863); [MIT actuator PDF](https://fab.cba.mit.edu/classes/865.18/motion/papers/mit-cheetah-actuator.pdf)
- MIT Biomimetics says high backdrivability protects the system against damage from environmental impacts, especially unexpected ones. — [MIT Biomimetics](https://biomimetics.mit.edu/research/1a9ba04b-d200-4743-b8fc-bf231f3231f0)
- RobStride 02: 6 Nm rated and 17 Nm peak, 7.75:1 ratio, dual 14-bit encoders, 48V, 405 g, FOC drive. Price from $189 (rcdrone), $195 backordered (roboticscenter.ai), A$328 (Pakronics). An IP67 variant is also sold. — [rcdrone](https://rcdrone.top/hi/products/robstride-02-qdd-17n-m-integrated-actuator-module-7-75-1-ratio-dual-14-bit-magnetic-encoders-48v-405g-foc-drive-for-robotics); [roboticscenter.ai](https://www.roboticscenter.ai/es/store/product/robstride-02); [OpenELAB IP67](https://openelab.io/products/robstride02-ip67-robot-joint-actuator); [Pakronics](https://www.pakronics.com.au/products/robstride-02-qdd-17n-m-integrated-actuator-module-dual-encoders-ss100064875)
- Damiao DM-J4310-2EC: 24V, 3 Nm nominal, 7 Nm peak, 120 RPM, from $124.99. The DM-J4310P-2EC variant is 3.5 Nm nominal and 12.5 Nm peak, 10:1 ratio, with crossed-roller bearings, at about $155 or €166-177. — [rcdrone](https://rcdrone.top/collections/robotics); [OpenELAB catalog](https://openelab.io/collections/motor-guide-catalog?page=4)
- Steadywin GIM4305-10 (used in Stanford Pupper v3): 4005 BLDC with 10:1 planetary gearbox, about 3.5 Nm peak and 1.0 Nm continuous. A Pupper v3 core-components bundle with these motors costs $1,320. — [Pupper v3 tech specs](https://pupper-v3-documentation.readthedocs.io/en/latest/_sources/learn_more/tech_specs.rst.txt); [AIFITLAB](https://aifitlab.com/products/pupper-v3-stanford-open-source-robotics-dog)
- Feetech STS3215 (the servo used in LeRobot SO-100 arms):
  - **7.4V version:** 16.5 kg·cm stall at 6V and 19.5 kg·cm at 7.4V (about 1.9 Nm), with 5 kg·cm nominal.
  - **12V version:** 30 kg·cm peak and 10 kg·cm rated.
  - **Feedback:** 12-bit magnetic encoder, reporting load, position, speed, voltage and current.
  - **Price:** about $25-26, or A$30.50-41.95.

  — [Core Electronics](https://core-electronics.com.au/feetech-sts3215-smart-servo.html); [RobotShop](https://robotshop.com/products/feetech-12v-30kgcm-magnetic-encoding-servo-sts3215)
- Berkeley Humanoid Lite builds its own BLDC actuators with 3D-printed cycloidal gearboxes. This is a middle path between servos and machined QDD that keeps the cost under $5k for 22 joints. — [humanoid.guide](https://humanoid.guide/berkeley-humanoid-lite-uses-diy-actuators-to-cut-costs/)
- ToddlerBot shows Dynamixel servos can do zero-shot sim2real if each motor model is system-identified (damping, armature, friction loss, torque-limit curve). Same-model motors behaved nearly identically. — [arXiv v4](https://arxiv.org/html/2502.00893v4)

### Inferences
- Per-joint cost: about $25 (STS3215), about $125-200 (Damiao or RobStride QDD), and an unconfirmed $50-300 for Dynamixel X-series depending on model. A 20-DoF QDD humanoid needs about $2.5-4k in actuators alone, which matches the Berkeley Humanoid Lite and ToddlerBot totals.
- For fighting, backdrivability matters twice. It mitigates impact damage, because gear teeth in high-ratio servos strip under shock loads. It also makes torque-controlled RL policies, which simulators model directly, transfer more cleanly.
- RobStride and Damiao units are Chinese-sourced and often backordered, so plan lead times.

### Gaps
- No verified current prices for Dynamixel X/XM/XH series, CubeMars AK-series, Moteus or ODrive.
- No measured backlash figures for the STS3215, and no head-to-head data on servo vs QDD shock tolerance.

## 3. Non-humanoid options (wheeled/tracked combat bots, quadrupeds, wheel-legged)

### Takeaway
For learned sim2real fighting, low-cost quadrupeds are the most mature choice. The Unitree Go2 (from $1,600 list) and Stanford Pupper v3 (about $2k DIY with QDD BLDC actuators) have a large body of RL work. Insect-class combat kits ($280-450) are the cheapest and toughest, but they are teleoperated by default and rarely have sim models.

### Cited Findings
- Unitree Go2 Air: $1,600 list from Unitree, excluding tax and freight. Walmart listed it at $2,590, and some guides say about $2,800 or $3,790, so sources conflict. — [robozaps](https://blog.robozaps.com/b/robot-dog-cost); [Walmart](https://www.walmart.com/c/kp/ground-robots); [globaldronehq](https://globaldronehq.com/blogs/guides/unitree-robot-price-guide-2026)
- Stanford Pupper v3 costs about $2,000 to build yourself and uses Steadywin GIM4305 BLDC actuators. The original Pupper used 12 hobby servos (JX CLS6336HV) and cost $600-1,000; the original paper says under $2,000. — [Pupper v3 docs](https://pupper-v3-documentation.readthedocs.io/en/latest/index.html); [Stanford Pupper arXiv](https://arxiv.org/abs/2110.00736v2); [Stanford Student Robotics](https://stanfordstudentrobotics.org/pupper)
- Mini Pupper 2 is a $649 programmable kit. Petoi Bittle prices range from $250 to $380 in guides, with $289 and $319 also quoted. — [roboticscenter.ai guide](https://www.roboticscenter.ai/guides/best-quadruped-robots-under-5000); [robozaps](https://blog.robozaps.com/b/robot-dog-cost); [digitaltoday](https://www.digitaltoday.co.kr/cn/view/95829/robot-dog-market-splits-by-use-in-2026-from-319-to-100000-dollars)
- Combat kits:
  - **FingerTech Viper (antweight, 1 lb):** $277.95 with radio and LiPo, restocking.
  - **Just Cuz Robotics SSP 2.0 (beetleweight, 3 lb):** $450, with brushless drive options.
  - **Absolute Chaos Robotics:** sells Jolt (3 lb), Shock (1 lb) and Spark (150 g) kits; prices not found.

  — [RobotShop](https://www.robotshop.com/products/fingertech-viper-combat-robot-kit-w-radio-lipoly); [reach.dog](https://m.reach.dog/shop/just-cuz-robotics/products/ssp-robot-kit); [NHRL wiki](https://wiki.nhrl.io/wiki/index.php/Absolute_Chaos_Robotics)
- NHRL runs three insect classes plus 30 lb. The 3 lb beetleweight is described as balancing accessibility and technical complexity. — [DAZN](https://news-frontend.news.indazn.com/en-US/news/sport-entertainment/what-is-nhrl-competitive-robot-fighting/54c3n1rw6bsn1aikttbvgcf2n)
- Unitree already runs humanoid fight events. "Unitree Iron Fist King: Awakening" was a tournament of four 4.3 ft G1s; coverage notes the G1 recovers quickly after being knocked down. G1s have also kickboxed in other events. — [PopSci](https://www.popsci.com/technology/humanoid-robots-boxing-match/); [SCMP](https://www.scmp.com/tech/tech-trends/article/3311831/ready-set-fight-unitrees-humanoid-robots-test-skills-unique-kickboxing-competition); [Interesting Engineering](https://interestingengineering.com/innovation/unitree-plans-robot-boxing-match)

### Inferences
- Wheeled antweight or beetleweight bots are the easiest sim2real starting point. They have low-dimensional dynamics, are simple to model in MuJoCo/Isaac with a hand-built URDF, and are nearly indestructible. They make sense as a stage-1 learned-control testbed (pushing/sumo) before moving to legged platforms.
- Pupper v3 is the cheapest QDD legged option with a sim2real RL pipeline; the Go2 is the cheapest commercial one.

### Gaps
- No data found on wheel-legged robots (e.g. Unitree B2W, Direct Drive Tech Diablo) or Mini Pi prices.
- Did not verify which RL pipelines ship with Pupper v3 or whether the Go2 Air allows low-level control (it is commonly reported that only EDU allows it).

## 4. Onboard compute and sensing; arena state estimation

### Takeaway
A Jetson Orin Nano Super ($249 at launch, possibly higher now) or an Orin NX (used on ToddlerBot) is the standard onboard brain. For state estimation in an arena, overhead-camera fiducials (AprilTag/ArUco) are cheap but less accurate than commercial mocap, especially at oblique angles and high speeds.

### Cited Findings
- Jetson Orin Nano Super Developer Kit: $249 at its Dec 2024 launch. SparkFun lists $249 but out of stock, with a warning that prices may change due to component costs and tariffs. JetsonHacks (Aug 2026) says it now lists at $399. — [SparkFun](https://www.sparkfun.com:443/products/22098); [JetsonHacks Aug 2026](https://jetsonhacks.com/2026/08/26/jetsonhacks-august-2026-newsletter/)
- ToddlerBot runs its RL policy at 50 Hz on the CPU of a Jetson Orin NX 16GB. — [arXiv v4](https://arxiv.org/html/2502.00893v4)
- Fiducial marker pose estimation has lower accuracy than commercial mocap. AprilTag accuracy is best when the camera faces the tag center. A low-budget ArUco system topped out at 2.2 m/s marker speed for 50 mm markers and 1.4 m/s for 30 mm markers. — [arXiv 2001.08622 / review](https://arxiv.org/pdf/2001.08622); [Technical Transactions](https://doaj.org/article/8dc7d8742a9247f19a6a9a7d3bc1475f); [PMC](https://pmc.ncbi.nlm.nih.gov/articles/PMC6960891)
- BlobBoards (a 2026 preprint) reports 3.6-5.0 mm median translation error against mocap and an 80% detection rate, vs 74% for AprilTag and 58% for ArUco. These numbers come from a single preprint. — [arXiv 2608.28830](https://arxiv.org/pdf/2608.28830)
- The original AprilTag paper is Olson 2011. — [UMich](https://april.eecs.umich.edu/pdfs/olson2011a.pdf)

### Inferences
- Fast punches and falls will blur and occlude tags. An arena likely needs several high-FPS overhead or angled cameras with large tags on the head and torso, fused with an onboard IMU.

### Gaps
- No current prices for OptiTrack (Flex 3 / Prime) or Vicon, Raspberry Pi 5, or IMUs (e.g. BNO085). No arena-specific accuracy study.

## 5. Durability, armor, repair, and URDF/MJCF availability

### Takeaway
Hard data on fall damage is scarce. ToddlerBot is the only platform with published numbers: it survived up to 7 falls before a part broke, and repair took about 35 minutes. Sim models are well covered for Unitree (G1, Go2) and ROBOTIS OP3 in MuJoCo Menagerie, and open-source robots ship their own MJCF/URDF.

### Cited Findings
- ToddlerBot survived up to 7 falls before breaking. A repair took about 21 min of 3D printing plus 14 min of assembly and zero-point calibration. Its longest walking streak without a fall was 19 min, and battery life is about 2 h. — [arXiv v4](https://arxiv.org/html/2502.00893v4)
- Unitree warranty and repair in the US is reportedly handled by Toborlife in Mountain View, CA (authorized repair center, per a reseller page; unverified). — [globaldronehq](https://globaldronehq.com/products/unitree-g1-edu-combat-boxing-humanoid-robot)
- The MuJoCo Menagerie/model gallery includes Unitree G1 (humanoids), Unitree Go2 (quadrupeds) and ROBOTIS OP3. No Booster T1 entry was found. Unitree also publishes open-source sim resources (unitree_mujoco, unitree_rl_gym). — [MuJoCo docs models](https://mujoco.readthedocs.io/en/3.8.0/models.html); [Unitree open source](https://www.unitree.com/mobile/opensource); [unitree_mujoco mirror](https://gitee.com/aipf/unitree_mujoco)
- ToddlerBot ships MuJoCo models with transmissions modeled by equality constraints and tendons. Berkeley Humanoid Lite ships its training and deployment frameworks. — [arXiv v4](https://arxiv.org/html/2502.00893v4); [Berkeley Engineering](https://engineering.berkeley.edu/news/2025/11/building-bots-on-a-budget/)

### Inferences
- In falls and impacts, the likely failure points are 3D-printed brackets and links (cheap to reprint), servo gear trains in high-ratio servos, and cable connectors. QDD actuators should survive shocks better (per the IMF argument), but each one costs more to replace.
- For a Big Hero 6-style project, a soft armor shell (foam or TPU) would raise impact duration and lower peak forces. That has to be reflected in the sim model's mass, inertia and contact parameters.

### Gaps
- No published fall or damage statistics for G1, R1, K1 or Bumi, and no replacement-part price lists from Unitree or Booster.
- No armor or shell products found for small humanoids.
- Did not verify official URDF/MJCF for R1, K1, T1, Bumi, PM01, HopeJR or Pupper v3, although several vendors publish URDFs on GitHub.
