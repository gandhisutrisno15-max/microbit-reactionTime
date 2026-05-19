#include "MicroBit.h"
#include <cstdlib>
#include <ctime>
#include <cstring>

#define MAX_SCORES 10
#define SYSTEM_CLOCK 64000000
#define MIN_DELAY 1000
#define MAX_DELAY 5000

MicroBit uBit;

//  Timing
extern "C" uint32_t measure_reaction_time(void);

struct ScoreData
{
    uint32_t scores[MAX_SCORES];
    uint8_t score_count;
    uint32_t best_score;
};

ScoreData score_data;

uint32_t cycles_to_ms(uint32_t cycles)
{
    return cycles / (SYSTEM_CLOCK / 1000);
}

void display_number(int num)
{
    uBit.display.clear();
    uBit.display.print(ManagedString(num));
}

void init_scores()
{
    memset(&score_data, 0, sizeof(score_data));
    score_data.best_score = 999999;
}

void save_score(uint32_t score)
{
    if (score_data.score_count < MAX_SCORES)
    {
        score_data.scores[score_data.score_count++] = score;
    }

    if (score < score_data.best_score)
    {
        score_data.best_score = score;
    }
}

uint32_t average_score()
{
    if (score_data.score_count == 0)
        return 0;

    uint32_t total = 0;

    for (int i = 0; i < score_data.score_count; i++)
    {
        total += score_data.scores[i];
    }

    return total / score_data.score_count;
}

void show_stats()
{
    uBit.display.scroll("BEST");
    display_number(score_data.best_score);
    fiber_sleep(1500);

    uBit.display.scroll("AVG");
    display_number(average_score());
    fiber_sleep(1500);
}

void run_game()
{
    uBit.display.scroll("READY");

    int delay_ms = MIN_DELAY + rand() % (MAX_DELAY - MIN_DELAY);

    fiber_sleep(delay_ms);

    uBit.display.print("!");

    uint32_t cycles = measure_reaction_time();

    fiber_sleep(500);

    uint32_t reaction_ms = cycles_to_ms(cycles);

    save_score(reaction_ms);

    uBit.display.scroll("TIME");
    display_number(reaction_ms);

    fiber_sleep(2000);

    uBit.display.scroll("BEST");
    display_number(score_data.best_score);

    fiber_sleep(2000);
}

int main()
{
    uBit.init();

    srand(system_timer_current_time());

    init_scores();

    uBit.display.scroll("START");

    while (1)
    {
        if (uBit.buttonA.isPressed())
        {
            run_game();

            while (uBit.buttonA.isPressed())
            {
                fiber_sleep(100);
            }
        }

        if (uBit.buttonB.isPressed())
        {
            show_stats();

            while (uBit.buttonB.isPressed())
            {
                fiber_sleep(100);
            }
        }

        fiber_sleep(100);
    }

    release_fiber();

    return 0;
}
