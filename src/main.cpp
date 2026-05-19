#include "MicroBit.h"
#include <cstdlib>
#include <ctime>
#include <cstring>

// Maximum number of reaction scores stored
#define MAX_SCORES 10

// micro:bit CPU clock speed (64 MHz)
#define SYSTEM_CLOCK 64000000

// Minimum and maximum random delay before signal appears
#define MIN_DELAY 1000
#define MAX_DELAY 5000

MicroBit uBit;

// Assembly function used to measure reaction time in CPU cycles
extern "C" uint32_t measure_reaction_time(void);

/*
 * Structure used to store player statistics
 * - scores[]     : stores individual reaction times
 * - score_count  : number of saved scores
 * - best_score   : fastest reaction time recorded
 */
struct ScoreData
{
    uint32_t scores[MAX_SCORES];
    uint8_t score_count;
    uint32_t best_score;
};

ScoreData score_data;

/*
 * Convert CPU clock cycles into milliseconds.
 * Formula:
 * milliseconds = cycles / (clock frequency / 1000)
 */
uint32_t cycles_to_ms(uint32_t cycles)
{
    return cycles / (SYSTEM_CLOCK / 1000);
}

/*
 * Display a number on the micro:bit LED screen.
 */
void display_number(int num)
{
    uBit.display.clear();
    uBit.display.print(ManagedString(num));
}

/*
 * Initialize all score data.
 * Sets all values to 0 and assigns a very large
 * value as the initial best score.
 */
void init_scores()
{
    memset(&score_data, 0, sizeof(score_data));
    score_data.best_score = 999999;
}

/*
 * Save a new reaction score.
 * Also updates the best score if the new score is faster.
 */
void save_score(uint32_t score)
{
    // Store score if storage is not full
    if (score_data.score_count < MAX_SCORES)
    {
        score_data.scores[score_data.score_count++] = score;
    }

    // Update best score
    if (score < score_data.best_score)
    {
        score_data.best_score = score;
    }
}

/*
 * Calculate the average reaction time.
 * Returns 0 if no scores exist.
 */
uint32_t average_score()
{
    if (score_data.score_count == 0)
        return 0;

    uint32_t total = 0;

    // Add all stored scores
    for (int i = 0; i < score_data.score_count; i++)
    {
        total += score_data.scores[i];
    }

    return total / score_data.score_count;
}

/*
 * Display player statistics:
 * - Best score
 * - Average score
 */
void show_stats()
{
    uBit.display.scroll("BEST");
    display_number(score_data.best_score);
    fiber_sleep(1500);

    uBit.display.scroll("AVG");
    display_number(average_score());
    fiber_sleep(1500);
}

/*
 * Main reaction game logic.
 * The player waits for the "!" signal
 * and presses the button as fast as possible.
 */
void run_game()
{
    // Inform player to get ready
    uBit.display.scroll("READY");

    // Generate random waiting time
    int delay_ms = MIN_DELAY + rand() % (MAX_DELAY - MIN_DELAY);

    // Wait before showing signal
    fiber_sleep(delay_ms);

    // Display reaction signal
    uBit.display.print("!");

    // Measure reaction time in CPU cycles
    uint32_t cycles = measure_reaction_time();

    fiber_sleep(500);

    // Convert cycles into milliseconds
    uint32_t reaction_ms = cycles_to_ms(cycles);

    // Save reaction result
    save_score(reaction_ms);

    // Show player's reaction time
    uBit.display.scroll("TIME");
    display_number(reaction_ms);

    fiber_sleep(2000);

    // Show best score achieved so far
    uBit.display.scroll("BEST");
    display_number(score_data.best_score);

    fiber_sleep(2000);
}

/*
 * Program entry point.
 * Button A = Start reaction game
 * Button B = Show statistics
 */
int main()
{
    // Initialize micro:bit hardware
    uBit.init();

    // Seed random number generator using system timer
    srand(system_timer_current_time());

    // Initialize score storage
    init_scores();

    // Startup message
    uBit.display.scroll("START");

    while (1)
    {
        // Start game when Button A is pressed
        if (uBit.buttonA.isPressed())
        {
            run_game();

            // Wait until button is released
            while (uBit.buttonA.isPressed())
            {
                fiber_sleep(100);
            }
        }

        // Show statistics when Button B is pressed
        if (uBit.buttonB.isPressed())
        {
            show_stats();

            // Wait until button is released
            while (uBit.buttonB.isPressed())
            {
                fiber_sleep(100);
            }
        }

        // Small delay to reduce CPU usage
        fiber_sleep(100);
    }

    // Release the current fiber before exiting
    release_fiber();

    return 0;
}
