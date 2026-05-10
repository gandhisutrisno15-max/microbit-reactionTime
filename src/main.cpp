#include "MicroBit.h"
#include "config.h"
#include <cstdlib>
#include <ctime>
#include <cstring>

// Forward declaration of assembly function
extern "C" uint32_t measure_reaction_time(void);

MicroBit uBit;

// Global variables for game state
uint32_t reaction_time_cycles = 0;
bool game_started = false;
int game_count = 0;

// Score storage structure
struct ScoreData {
    uint32_t scores[MAX_SCORES];
    uint8_t score_count;
    uint32_t highest_score;
    uint32_t lowest_score;
};

ScoreData score_data;

// Function to convert cycles to milliseconds
uint32_t cycles_to_ms(uint32_t cycles) {
    return (cycles / (SYSTEM_CLOCK / 1000));
}

// Function to display number on LED matrix
void display_number(int num) {
    uBit.display.clear();
    if (num < 0) {
        uBit.display.print("ERR");
        return;
    }
    uBit.display.print(num);
}

// Initialize score data from storage
void init_scores() {
    memset(&score_data, 0, sizeof(score_data));
    score_data.highest_score = 999999;
    score_data.lowest_score = 999999;
    score_data.score_count = 0;
}

// Save a new score
void save_score(uint32_t score_ms) {
    // Add score to array if we have space
    if (score_data.score_count < MAX_SCORES) {
        score_data.scores[score_data.score_count] = score_ms;
        score_data.score_count++;
    } else {
        // Shift scores and add new one at the end
        for (int i = 0; i < MAX_SCORES - 1; i++) {
            score_data.scores[i] = score_data.scores[i + 1];
        }
        score_data.scores[MAX_SCORES - 1] = score_ms;
    }
    
    // Update highest score
    if (score_ms < score_data.highest_score) {
        score_data.highest_score = score_ms;
    }
    
    // Find lowest score
    score_data.lowest_score = 0;
    for (int i = 0; i < score_data.score_count; i++) {
        if (score_data.scores[i] > score_data.lowest_score) {
            score_data.lowest_score = score_data.scores[i];
        }
    }
}

// Calculate average score
uint32_t get_average_score() {
    if (score_data.score_count == 0) return 0;
    
    uint32_t total = 0;
    for (int i = 0; i < score_data.score_count; i++) {
        total += score_data.scores[i];
    }
    return total / score_data.score_count;
}

// Display score history
void display_score_history() {
    uBit.display.clear();
    uBit.display.print("HST");
    wait(1000);
    
    if (score_data.score_count == 0) {
        uBit.display.clear();
        uBit.display.print("NONE");
        wait(2000);
        return;
    }
    
    // Display each score
    for (int i = 0; i < score_data.score_count; i++) {
        display_number(score_data.scores[i]);
        wait(1500);
    }
}

// Display statistics
void display_stats() {
    uBit.display.clear();
    uBit.display.print("STAT");
    wait(1000);
    
    if (score_data.score_count == 0) {
        uBit.display.clear();
        uBit.display.print("NONE");
        wait(2000);
        return;
    }
    
    // Display highest score
    uBit.display.clear();
    uBit.display.print("BEST");
    wait(1000);
    display_number(score_data.highest_score);
    wait(2000);
    
    // Display lowest score
    uBit.display.clear();
    uBit.display.print("WRST");
    wait(1000);
    display_number(score_data.lowest_score);
    wait(2000);
    
    // Display average score
    uint32_t avg = get_average_score();
    uBit.display.clear();
    uBit.display.print("AVG");
    wait(1000);
    display_number(avg);
    wait(2000);
    
    // Display total games played
    uBit.display.clear();
    uBit.display.print("CNT");
    wait(1000);
    display_number(score_data.score_count);
    wait(2000);
}

// Event handler for button A press
void on_button_a_pressed(MicroBitEvent e) {
    if (!game_started) {
        game_started = true;
    }
}

// Event handler for button B press (view statistics)
void on_button_b_pressed(MicroBitEvent e) {
    if (!game_started) {
        display_stats();
    }
}

// Main game loop
void run_game() {
    uBit.display.clear();
    uBit.display.print("RDY");
    wait(1000);
    
    // Random delay between 1 and 10 seconds
    int delay_ms = MIN_DELAY + (rand() % (MAX_DELAY - MIN_DELAY));
    
    uBit.display.clear();
    int countdown = delay_ms / 1000;
    for (int i = countdown; i > 0; i--) {
        display_number(i);
        wait(1000);
    }
    
    // Play audio/visual cue
    uBit.display.print("!");
    uBit.sound.playTone(CUE_FREQUENCY, CUE_DURATION);
    
    // Measure reaction time using assembly function
    uint32_t cycles = measure_reaction_time();
    reaction_time_cycles = cycles;
    
    // Convert cycles to milliseconds
    uint32_t reaction_ms = cycles_to_ms(cycles);
    
    // Check for false start or invalid time
    if (cycles == 0 || reaction_ms > 1000) {
        uBit.display.clear();
        uBit.display.print("CHEAT");
        wait(2000);
        return;
    }
    
    // Save the score
    save_score(reaction_ms);
    game_count++;
    
    // Display result
    uBit.display.clear();
    uBit.display.print(reaction_ms);
    wait(2000);
    
    // Display best score after result
    uBit.display.clear();
    uBit.display.print("BEST");
    wait(800);
    display_number(score_data.highest_score);
    wait(1500);
    
    // Display average
    uint32_t avg = get_average_score();
    uBit.display.clear();
    uBit.display.print("AVG");
    wait(800);
    display_number(avg);
    wait(1500);
    
    // Ask to play again
    uBit.display.clear();
    uBit.display.print("PLAY?");
}

// Main entry point
int main() {
    uBit.init();
    srand(time(NULL));
    
    // Initialize score storage
    init_scores();
    
    uBit.display.print("START");
    wait(2000);
    
    uBit.display.clear();
    uBit.display.print("A");
    wait(1000);
    
    // Event listeners
    uBit.buttonA.onPress(on_button_a_pressed);
    uBit.buttonB.onPress(on_button_b_pressed);
    
    while (1) {
        if (game_started) {
            game_started = false;
            run_game();
        }
        wait(100);
    }
    
    release_fiber();
    return 0;
}
