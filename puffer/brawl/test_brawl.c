// Bot-vs-bot sanity check for brawl.h: win rates, KO rate, episode length,
// observation range and raw env throughput. Optional arg: render one game.
#include <stdio.h>
#include <time.h>
#include "brawl.h"

const char* BOT_NAMES[] = {"idle", "rush", "smart"};

void alloc_agents(Brawl* env) {
    for (int i = 0; i < 2; i++) {
        env->agents[i].observations = calloc(OBS_SIZE, sizeof(obs_t));
        env->agents[i].actions = calloc(NUM_ATNS, sizeof(float));
        env->agents[i].rewards = calloc(1, sizeof(float));
        env->agents[i].terminals = calloc(1, sizeof(float));
    }
}

// Seat 0 is a learning slot driven by bot p0; seat 1 is the env's own bot p1.
void step_bots(Brawl* env, int p0, int p1) {
    env->bot_policy = p0;
    puf_bot(env, 0);
    for (int a = 0; a < NUM_ATNS; a++) {
        env->agents[0].actions[a] = env->bot_actions[0][a];
    }
    env->bot_policy = p1;
    puf_step(env);
}

int main(int argc, char** argv) {
    Brawl env = {.num_agents = 1, .num_bots = 1, .max_ticks = 2700, .stocks = 3,
        .damage_reward = 0.01f, .rng = 7};
    alloc_agents(&env);
    env.agents[0].policy = 0;
    if (argc > 1) {
        puf_reset(&env);
        while (1) {
            step_bots(&env, atoi(argv[1]), argc > 2 ? atoi(argv[2]) : BOT_SMART);
            puf_render(&env);
        }
    }
    int games = 300;
    long total_steps = 0;
    int bad_obs = 0;
    clock_t start = clock();
    for (int p0 = BOT_IDLE; p0 <= BOT_SMART; p0++) {
        for (int p1 = BOT_IDLE; p1 <= BOT_SMART; p1++) {
            env.log = (Log){0};
            puf_reset(&env);
            int done = 0;
            float ret = 0;
            while (done < games) {
                step_bots(&env, p0, p1);
                total_steps++;
                ret += env.agents[0].rewards[0];
                for (int o = 0; o < OBS_SIZE; o++) {
                    float v = env.agents[0].observations[o];
                    if (!isfinite(v) || fabsf(v) > 10.0f) {
                        bad_obs++;
                    }
                }
                done += env.agents[0].terminals[0] > 0;
            }
            Log l = env.log;
            printf("%-5s vs %-5s  p0 win %.2f  draws %.2f  len %6.0f  p0 KOs %.2f"
                "  p0 dmg %6.1f  p0 return %+.2f\n",
                BOT_NAMES[p0], BOT_NAMES[p1], l.perf/l.n, l.draw_rate/l.n,
                l.episode_length/l.n, l.kos/l.n, l.damage_dealt/l.n, ret/games);
        }
    }
    double secs = (double)(clock() - start)/CLOCKS_PER_SEC;
    printf("bad obs values: %d\n", bad_obs);
    printf("%.1fM env steps/s on one core\n", total_steps/secs/1e6);
    return bad_obs > 0;
}
