// Brawl: a side-view 1v1 robot platform fighter. Knock the other bot past the
// blast zones. Damage builds up and makes knockback grow, so the longer you
// survive the further you fly. Attacks beat grabs (jab is faster), grabs beat
// blocks, blocks beat attacks. Two learning slots for selfplay, or one slot vs
// a scripted bot.
#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>
typedef float obs_t;
#define PUF_HAS_BOT_POLICY
#include "pufferenv.h"

// Actions: move {none, left, right}, jump {no, yes}, move {none, jab, heavy, block, grab}
#define ACT_SIZES {3, 2, 5}
#define NUM_ATNS 3
#define FIGHTER_OBS 18
#define OBS_SIZE (2*FIGHTER_OBS + 3)

typedef Env Brawl;

// Scripted opponents, weakest first: [env] bot_policy ids and [selfplay] eval_bots rungs
enum { BOT_IDLE = 0, BOT_RUSH = 1, BOT_SMART = 2 };
enum { MOVE_NONE = 0, MOVE_JAB = 1, MOVE_HEAVY = 2, MOVE_BLOCK = 3, MOVE_GRAB = 4 };

#define TIMESTEP (1.0f/30.0f)
#define GRAVITY -38.0f
#define FALL_MAX -18.0f
#define WALK_SPEED 6.5f
#define AIR_ACCEL 30.0f
#define AIR_SPEED 5.5f
#define JUMP_SPEED 13.0f
#define JUMP_COOLDOWN 8
#define MAX_JUMPS 3 // one ground jump, two air jumps for recovery
#define BODY_W 0.9f
#define BODY_H 1.6f
#define STAGE_HALF 7.0f
#define STAGE_THICK 2.0f
#define BLAST_X 15.0f
#define BLAST_TOP 14.0f
#define BLAST_BOTTOM -8.0f
#define GUARD_MAX 100.0f
#define GUARD_BREAK_STUN 50
#define RESPAWN_INVULN 45
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 640
#define PX_PER_M 36.0f

const Color PUFF_RED = {187, 0, 0, 255};
const Color PUFF_CYAN = {0, 187, 187, 255};
const Color PUFF_WHITE = {241, 241, 241, 255};
const Color PUFF_BACKGROUND = {6, 24, 24, 255};
const Color STAGE_COLOR = {60, 70, 80, 255};
const Color HIT_COLOR = {255, 210, 90, 160};

typedef struct {
    int startup, active, recovery;
    float damage, base_kb, kb_growth, angle, reach;
} Move;

// Frame data at 30 ticks/s, indexed by MOVE_*. angle in degrees above
// horizontal. Positional, since the trainer compiles this as C++.
const Move MOVES[5] = {
    {0},
    {.startup = 2, .active = 2, .recovery = 6, .damage = 3,
        .base_kb = 3.0f, .kb_growth = 0.06f, .angle = 20, .reach = 1.0f},
    {.startup = 7, .active = 3, .recovery = 14, .damage = 12,
        .base_kb = 7.0f, .kb_growth = 0.16f, .angle = 40, .reach = 1.4f},
    {0},
    {.startup = 3, .active = 2, .recovery = 16, .damage = 7,
        .base_kb = 8.0f, .kb_growth = 0.10f, .angle = 30, .reach = 0.8f},
};

typedef struct {
    float x, y, vx, vy;
    float facing;      // -1 or 1
    float damage;      // percent, raises knockback taken
    float guard;
    int stocks, jumps, jump_cd, grounded;
    int move, move_timer, hit_landed, blocking;
    int hitstun, invuln;
    float* observations;
    Color color;
} Fighter;

struct Log {
    // Win credit for policy 0 in [0, 1]; 0.5 is a draw
    float perf;
    float score;
    float episode_return;
    float episode_length;
    float kos;
    float damage_dealt;
    float policy_0_score;
    float policy_1_score;
    float draw_rate;
    float n;
};

struct Env {
    Log log;
    Agent agents[2];
    Fighter fighters[2];
    float episode_return[2];
    float episode_damage[2];
    float episode_kos[2];
    float bot_observations[2][OBS_SIZE];
    float bot_actions[2][NUM_ATNS];
    int num_agents;
    int num_bots;
    int bot_policy;
    int max_ticks;
    int stocks;
    float damage_reward;
    int tick;
    int tag;
    int boundary_reached;
    unsigned int rng;
};

static inline void puf_set_bot_policy(Env* env, int bot_policy) {
    env->bot_policy = bot_policy;
}

void puf_init(Env* env, Dict* kwargs) {
    env->num_agents = dict_get(kwargs, "num_agents");
    env->num_bots = dict_get(kwargs, "num_bots");
    env->bot_policy = dict_get(kwargs, "bot_policy");
    env->max_ticks = dict_get(kwargs, "max_ticks");
    env->stocks = dict_get(kwargs, "stocks");
    env->damage_reward = dict_get(kwargs, "damage_reward");
    assert(env->num_agents + env->num_bots == 2
        && "brawl is 1v1: env.num_agents + env.num_bots must be 2");
    assert(env->bot_policy >= BOT_IDLE && env->bot_policy <= BOT_SMART);
    for (int i = 0; i < env->num_agents; i++) {
        env->agents[i].policy = i;
        env->agents[i].action_mask = NULL;
    }
}

void puf_log(Log* log, Dict* out) {
    dict_set(out, "perf", log->perf);
    dict_set(out, "score", log->score);
    dict_set(out, "episode_return", log->episode_return);
    dict_set(out, "episode_length", log->episode_length);
    dict_set(out, "kos", log->kos);
    dict_set(out, "damage_dealt", log->damage_dealt);
    dict_set(out, "policy_0_score", log->policy_0_score);
    dict_set(out, "policy_1_score", log->policy_1_score);
    dict_set(out, "draw_rate", log->draw_rate);
    dict_set(out, "n", log->n);
}

float randf(Brawl* env) {
    return (float)rand_r(&env->rng) / (float)RAND_MAX;
}

int move_active(Fighter* f) {
    Move m = MOVES[f->move];
    return f->move_timer >= m.startup && f->move_timer < m.startup + m.active;
}

// Ego-centric and mirrored by side, so both seats see the same game.
void fighter_obs(Fighter* f, float side, obs_t* obs) {
    obs[0] = f->x*side / STAGE_HALF;
    obs[1] = f->y / 5.0f;
    obs[2] = f->vx*side / 10.0f;
    obs[3] = f->vy / 10.0f;
    obs[4] = f->facing*side;
    obs[5] = f->damage / 100.0f;
    obs[6] = f->stocks / 3.0f;
    obs[7] = f->jumps / (float)MAX_JUMPS;
    obs[8] = f->grounded;
    obs[9] = f->guard / GUARD_MAX;
    obs[10] = f->hitstun / 30.0f;
    obs[11] = f->invuln > 0;
    obs[12] = f->jump_cd == 0;
    for (int m = MOVE_JAB; m <= MOVE_GRAB; m++) {
        obs[12 + m] = (f->move == m) || (m == MOVE_BLOCK && f->blocking);
    }
    Move m = MOVES[f->move];
    int total = m.startup + m.active + m.recovery;
    obs[17] = total > 0 ? f->move_timer / (float)total : 0.0f;
}

void compute_observations(Brawl* env) {
    for (int i = 0; i < 2; i++) {
        Fighter* me = &env->fighters[i];
        Fighter* opp = &env->fighters[1 - i];
        float side = i == 0 ? 1.0f : -1.0f;
        obs_t* obs = me->observations;
        fighter_obs(me, side, obs);
        fighter_obs(opp, side, obs + FIGHTER_OBS);
        obs[2*FIGHTER_OBS] = (opp->x - me->x)*side / STAGE_HALF;
        obs[2*FIGHTER_OBS + 1] = (opp->y - me->y) / 5.0f;
        obs[2*FIGHTER_OBS + 2] = 1.0f - env->tick / (float)env->max_ticks;
    }
}

void spawn(Brawl* env, Fighter* f, float x) {
    f->x = x;
    f->y = 0;
    f->vx = f->vy = 0;
    f->facing = x < 0 ? 1.0f : -1.0f;
    f->damage = 0;
    f->guard = GUARD_MAX;
    f->jumps = MAX_JUMPS;
    f->jump_cd = 0;
    f->grounded = 1;
    f->move = MOVE_NONE;
    f->move_timer = f->hit_landed = f->blocking = 0;
    f->hitstun = 0;
}

void puf_reset(Brawl* env) {
    env->tick = 0;
    for (int i = 0; i < 2; i++) {
        Fighter* f = &env->fighters[i];
        float side = i == 0 ? -1.0f : 1.0f;
        spawn(env, f, side*(3.0f + randf(env)));
        f->stocks = env->stocks;
        f->invuln = 0;
        f->color = i == 0 ? PUFF_RED : PUFF_CYAN;
        f->observations = (i < env->num_agents)
            ? env->agents[i].observations : env->bot_observations[i];
        env->episode_return[i] = 0;
        env->episode_damage[i] = 0;
        env->episode_kos[i] = 0;
    }
    compute_observations(env);
}

void add_reward(Brawl* env, int slot, float reward) {
    if (slot >= env->num_agents) {
        return;
    }
    env->agents[slot].rewards[0] += reward;
    env->episode_return[slot] += reward;
}

// outcome: 1 slot 0 won, -1 slot 0 lost, 0 draw
void end_episode(Brawl* env, int outcome) {
    float s0_score = (outcome > 0) ? 1.0f : (outcome < 0) ? 0.0f : 0.5f;
    env->log.policy_0_score += s0_score*env->num_agents;
    env->log.policy_1_score += (1.0f - s0_score)*env->num_agents;
    if (outcome == 0) {
        env->log.draw_rate += env->num_agents;
    }
    if (env->tag > 0) {
        env->boundary_reached = 1;
    }
    env->log.perf += s0_score*env->num_agents;
    env->log.score += s0_score*env->num_agents;
    for (int i = 0; i < env->num_agents; i++) {
        env->log.episode_return += env->episode_return[i];
        env->log.episode_length += env->tick;
        env->log.kos += env->episode_kos[i];
        env->log.damage_dealt += env->episode_damage[i];
        env->log.n += 1.0f;
        env->agents[i].terminals[0] = 1.0f;
    }
    puf_reset(env);
}

void air_drift(Fighter* f, int dir) {
    float target = dir == 1 ? -1.0f : dir == 2 ? 1.0f : 0.0f;
    f->vx += target*AIR_ACCEL*TIMESTEP;
    f->vx = fmaxf(fminf(f->vx, AIR_SPEED), -AIR_SPEED);
}

// Actions are ego-centric like observations: seat 1 sees the world mirrored,
// so its left/right swap back to world directions here.
int world_dir(int dir, int seat) {
    if (seat == 0 || dir == 0) {
        return dir;
    }
    return 3 - dir;
}

void read_inputs(Fighter* f, Fighter* opp, float* actions, int seat) {
    int dir = world_dir((int)actions[0], seat);
    int jump = (int)actions[1];
    int act = (int)actions[2];
    if (f->hitstun > 0) {
        f->hitstun--;
        f->blocking = 0;
        return;
    }
    if (f->move != MOVE_NONE) {
        Move m = MOVES[f->move];
        f->move_timer++;
        if (f->move_timer >= m.startup + m.active + m.recovery) {
            f->move = MOVE_NONE;
        }
        if (f->grounded) {
            f->vx *= 0.6f;
        } else {
            air_drift(f, dir);
        }
        return;
    }
    if (f->grounded && opp->x != f->x) {
        f->facing = opp->x > f->x ? 1.0f : -1.0f;
    }
    f->blocking = act == MOVE_BLOCK && f->grounded && f->guard > 0;
    if (f->blocking) {
        f->vx = 0;
        return;
    }
    if (act == MOVE_JAB || act == MOVE_HEAVY || act == MOVE_GRAB) {
        f->move = act;
        f->move_timer = 0;
        f->hit_landed = 0;
        if (f->grounded) {
            f->vx = 0;
        }
        return;
    }
    if (f->grounded) {
        f->vx = (dir == 1 ? -1.0f : dir == 2 ? 1.0f : 0.0f)*WALK_SPEED;
    } else {
        air_drift(f, dir);
    }
    if (jump && f->jumps > 0 && f->jump_cd == 0) {
        f->vy = f->jumps == MAX_JUMPS ? JUMP_SPEED : 0.9f*JUMP_SPEED;
        f->jumps--;
        f->jump_cd = JUMP_COOLDOWN;
        f->grounded = 0;
    }
}

void integrate(Fighter* f) {
    float prev_y = f->y;
    if (f->jump_cd > 0) {
        f->jump_cd--;
    }
    if (f->invuln > 0) {
        f->invuln--;
    }
    if (!f->blocking && f->guard < GUARD_MAX) {
        f->guard = fminf(f->guard + 0.6f, GUARD_MAX);
    }
    if (!f->grounded) {
        f->vy = fmaxf(f->vy + GRAVITY*TIMESTEP, FALL_MAX);
        if (f->hitstun > 0) {
            f->vx *= 0.97f;
        }
    } else if (f->hitstun > 0) {
        f->vx *= 0.8f;
    }
    f->x += f->vx*TIMESTEP;
    f->y += f->vy*TIMESTEP;
    float edge = STAGE_HALF + BODY_W/2;
    if (prev_y >= 0 && f->y < 0 && fabsf(f->x) <= edge) {
        f->y = 0;
        f->vy = 0;
    } else if (f->y < 0 && f->y > -STAGE_THICK && fabsf(f->x) < edge) {
        f->x = f->x < 0 ? -edge : edge;
        f->vx = 0;
    }
    f->grounded = f->y == 0 && fabsf(f->x) <= edge && f->vy <= 0;
    if (f->grounded) {
        f->jumps = MAX_JUMPS;
    }
}

// Returns damage dealt by attacker this tick (0 if no clean hit)
float try_hit(Fighter* atk, Fighter* def) {
    if (atk->move == MOVE_NONE || atk->move == MOVE_BLOCK || atk->hit_landed
            || !move_active(atk) || def->invuln > 0) {
        return 0;
    }
    Move m = MOVES[atk->move];
    float front = atk->x + atk->facing*BODY_W/2;
    float reach_x = front + atk->facing*m.reach;
    float lo = fminf(front, reach_x), hi = fmaxf(front, reach_x);
    if (def->x + BODY_W/2 < lo || def->x - BODY_W/2 > hi) {
        return 0;
    }
    if (def->y > atk->y + BODY_H || def->y + BODY_H < atk->y + 0.2f) {
        return 0;
    }
    atk->hit_landed = 1;
    if (def->blocking && atk->move != MOVE_GRAB) {
        def->guard -= m.damage*5.0f;
        def->vx = atk->facing*2.0f;
        if (def->guard <= 0) {
            def->guard = 0;
            def->blocking = 0;
            def->hitstun = GUARD_BREAK_STUN;
        }
        return 0;
    }
    def->damage += m.damage;
    float kb = m.base_kb + m.kb_growth*def->damage;
    float rad = m.angle*PI/180.0f;
    def->vx = atk->facing*kb*cosf(rad);
    def->vy = kb*sinf(rad);
    def->grounded = 0;
    def->move = MOVE_NONE;
    def->blocking = 0;
    def->hitstun = (int)(kb*0.8f);
    return m.damage;
}

int out_of_bounds(Fighter* f) {
    return f->x < -BLAST_X || f->x > BLAST_X
        || f->y < BLAST_BOTTOM || f->y > BLAST_TOP;
}

// Scripted bots read world state directly and pick world directions
void bot_decide(Brawl* env, int idx) {
    Fighter* me = &env->fighters[idx];
    Fighter* opp = &env->fighters[1 - idx];
    float* act = env->bot_actions[idx];
    float dx = opp->x - me->x;
    float adx = fabsf(dx);
    act[0] = 0;
    act[1] = 0;
    act[2] = MOVE_NONE;
    // Everyone recovers: drift toward the stage (out from under it first) and
    // save jumps until falling low
    if (fabsf(me->x) > STAGE_HALF || me->y < 0) {
        int toward_centre = fabsf(me->x) > STAGE_HALF;
        act[0] = (me->x > 0) == toward_centre ? 1 : 2;
        act[1] = me->vy < 0 && me->y < 1.0f;
        return;
    }
    // No air attacks near the edge, just drift back in
    if (!me->grounded && fabsf(me->x) > STAGE_HALF - 2.0f) {
        act[0] = me->x > 0 ? 1 : 2;
        return;
    }
    if (env->bot_policy == BOT_IDLE) {
        return;
    }
    act[0] = dx < 0 ? 1 : 2;
    if (env->bot_policy == BOT_RUSH) {
        if (adx < 1.6f && fabsf(opp->y - me->y) < 1.0f) {
            act[2] = MOVE_JAB;
        }
        return;
    }
    // BOT_SMART: spacing plus a noisy read of the jab > grab > block > heavy triangle
    int opp_attacking = opp->move == MOVE_JAB || opp->move == MOVE_HEAVY;
    if (opp_attacking && adx < 2.2f && randf(env) < 0.6f) {
        act[2] = MOVE_BLOCK;
    } else if (opp->blocking && adx < 1.4f) {
        act[2] = MOVE_GRAB;
    } else if (adx < 1.2f) {
        act[2] = randf(env) < 0.7f ? MOVE_JAB : MOVE_GRAB;
    } else if (adx < 2.0f && fabsf(opp->y - me->y) < 1.0f) {
        act[2] = MOVE_HEAVY;
    }
    // Hold the ledge rather than chase an opponent off stage
    if (fabsf(opp->x) > STAGE_HALF && fabsf(me->x) > STAGE_HALF - 1.5f) {
        act[0] = 0;
    }
    if (me->grounded && opp->y > me->y + 1.5f && randf(env) < 0.1f) {
        act[1] = 1;
    }
}

// Bot actions go through the same ego-centric frame as a learned policy's
void puf_bot(Brawl* env, int idx) {
    bot_decide(env, idx);
    env->bot_actions[idx][0] = world_dir((int)env->bot_actions[idx][0], idx);
}

void puf_step(Brawl* env) {
    env->tick++;
    for (int i = 0; i < env->num_agents; i++) {
        env->agents[i].rewards[0] = 0;
        env->agents[i].terminals[0] = 0;
    }
    for (int i = env->num_agents; i < 2; i++) {
        puf_bot(env, i);
    }
    for (int i = 0; i < 2; i++) {
        float* actions = (i < env->num_agents)
            ? env->agents[i].actions : env->bot_actions[i];
        read_inputs(&env->fighters[i], &env->fighters[1 - i], actions, i);
    }
    for (int i = 0; i < 2; i++) {
        integrate(&env->fighters[i]);
    }
    // Resolve both hits before applying either, so simultaneous hits trade
    Fighter snapshot[2] = {env->fighters[0], env->fighters[1]};
    for (int i = 0; i < 2; i++) {
        Fighter atk = snapshot[i];
        float dealt = try_hit(&atk, &env->fighters[1 - i]);
        env->fighters[i].hit_landed = atk.hit_landed;
        if (dealt > 0) {
            env->episode_damage[i] += dealt;
            add_reward(env, i, env->damage_reward*dealt);
            add_reward(env, 1 - i, -env->damage_reward*dealt);
        }
    }
    // Bodies push apart instead of overlapping
    Fighter* a = &env->fighters[0];
    Fighter* b = &env->fighters[1];
    float dx = b->x - a->x;
    if (fabsf(dx) < BODY_W && fabsf(b->y - a->y) < BODY_H) {
        float push = 0.5f*(BODY_W - fabsf(dx))*(dx >= 0 ? 1.0f : -1.0f);
        a->x -= push;
        b->x += push;
    }
    for (int i = 0; i < 2; i++) {
        Fighter* f = &env->fighters[i];
        if (!out_of_bounds(f)) {
            continue;
        }
        f->stocks--;
        env->episode_kos[1 - i] += 1;
        add_reward(env, i, -1.0f);
        add_reward(env, 1 - i, 1.0f);
        if (f->stocks > 0) {
            spawn(env, f, 0.0f);
            f->y = 3.0f;
            f->grounded = 0;
            f->invuln = RESPAWN_INVULN;
        }
    }
    Fighter* f0 = &env->fighters[0];
    Fighter* f1 = &env->fighters[1];
    if (f0->stocks <= 0 || f1->stocks <= 0 || env->tick >= env->max_ticks) {
        int outcome = (f0->stocks > f1->stocks) - (f0->stocks < f1->stocks);
        if (outcome == 0) {
            outcome = (f0->damage < f1->damage) - (f0->damage > f1->damage);
        }
        end_episode(env, outcome);
        return;
    }
    compute_observations(env);
}

float to_px_x(float x) {
    return WINDOW_WIDTH/2 + x*PX_PER_M;
}

float to_px_y(float y) {
    return WINDOW_HEIGHT*0.68f - y*PX_PER_M;
}

// Hold Left Shift: A/D move, W jump, J jab, K heavy, L block, I grab
void human_controls(Brawl* env) {
    if (!IsKeyDown(KEY_LEFT_SHIFT) || env->num_agents < 1) {
        return;
    }
    float* act = env->agents[0].actions;
    act[0] = IsKeyDown(KEY_A) ? 1 : IsKeyDown(KEY_D) ? 2 : 0;
    act[1] = IsKeyDown(KEY_W);
    act[2] = IsKeyDown(KEY_J) ? MOVE_JAB : IsKeyDown(KEY_K) ? MOVE_HEAVY
        : IsKeyDown(KEY_L) ? MOVE_BLOCK : IsKeyDown(KEY_I) ? MOVE_GRAB : MOVE_NONE;
}

void draw_fighter(Fighter* f) {
    float left = to_px_x(f->x - BODY_W/2);
    float top = to_px_y(f->y + BODY_H);
    float w = BODY_W*PX_PER_M, h = BODY_H*PX_PER_M;
    Color body = f->color;
    if (f->hitstun > 0 && (f->hitstun/2) % 2 == 0) {
        body = PUFF_WHITE;
    }
    if (f->invuln > 0) {
        body.a = 120;
    }
    DrawRectangleRounded((Rectangle){left, top, w, h}, 0.3f, 6, body);
    float visor_x = f->facing > 0 ? left + w*0.45f : left + w*0.1f;
    DrawRectangle(visor_x, top + h*0.15f, w*0.45f, h*0.12f, PUFF_BACKGROUND);
    if (f->blocking) {
        Color shield = {120, 200, 255, (unsigned char)(60 + f->guard)};
        DrawCircleLines(to_px_x(f->x), to_px_y(f->y + BODY_H/2), h*0.65f, shield);
        DrawCircle(to_px_x(f->x), to_px_y(f->y + BODY_H/2), h*0.6f,
            (Color){120, 200, 255, 50});
    }
    if (f->move != MOVE_NONE && f->move != MOVE_BLOCK && move_active(f)) {
        Move m = MOVES[f->move];
        float front = f->x + f->facing*BODY_W/2;
        float x0 = fminf(front, front + f->facing*m.reach);
        Color c = f->move == MOVE_GRAB ? PUFF_WHITE : HIT_COLOR;
        DrawRectangle(to_px_x(x0), to_px_y(f->y + BODY_H - 0.3f),
            m.reach*PX_PER_M, (BODY_H - 0.6f)*PX_PER_M, c);
    }
}

void puf_render(Brawl* env) {
    if (!IsWindowReady()) {
        SetConfigFlags(FLAG_MSAA_4X_HINT);
        InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PufferLib Brawl");
        SetTargetFPS(30);
    }
    if (IsKeyDown(KEY_ESCAPE)) {
        exit(0);
    }
    human_controls(env);
    BeginDrawing();
    ClearBackground(PUFF_BACKGROUND);
    DrawRectangle(to_px_x(-STAGE_HALF), to_px_y(0), 2*STAGE_HALF*PX_PER_M,
        STAGE_THICK*PX_PER_M, STAGE_COLOR);
    DrawRectangle(to_px_x(-STAGE_HALF), to_px_y(0), 2*STAGE_HALF*PX_PER_M, 4, PUFF_WHITE);
    for (int i = 0; i < 2; i++) {
        draw_fighter(&env->fighters[i]);
    }
    for (int i = 0; i < 2; i++) {
        Fighter* f = &env->fighters[i];
        int x = i == 0 ? WINDOW_WIDTH/2 - 260 : WINDOW_WIDTH/2 + 120;
        DrawText(TextFormat("%d%%", (int)f->damage), x, WINDOW_HEIGHT - 90, 40, f->color);
        for (int s = 0; s < f->stocks; s++) {
            DrawCircle(x + 10 + 26*s, WINDOW_HEIGHT - 30, 9, f->color);
        }
    }
    DrawText(TextFormat("%ds", (env->max_ticks - env->tick)/30), WINDOW_WIDTH/2 - 20,
        20, 30, PUFF_WHITE);
    EndDrawing();
    puf_web_vsync();
}

void puf_close(Brawl* env) {
    if (IsWindowReady()) {
        CloseWindow();
    }
}
