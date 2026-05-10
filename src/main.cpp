#include "MicroBit.h"
#include "config.h"
#include <cstdlib>
#include <ctime>

// Forward declaration of assembly function
extern "C" uint32_t measure_reaction_time(void);

MicroBit uBit;

// Global variables
uint32_t reaction_time_cycles = 0;
bool game_started = false;
int high_score = 0;

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

// Event handler for button A press
void on_button_a_pressed(MicroBitEvent e) {
    if (!game_started) {
        game_started = true;
        uBit.display.print("GO!");
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
    
    // Display result
    uBit.display.clear();
    uBit.display.print(reaction_ms);
    wait(2000);
    
    // Update high score
    if (reaction_ms < high_score || high_score == 0) {
        high_score = reaction_ms;
        uBit.display.print("NEW");
        wait(1000);
    }
    
    // Check for false start
    if (cycles == 0) {
        uBit.display.print("CHEAT");
        wait(1000);
    }
    
    // Ask to play again
    uBit.display.clear();
    uBit.display.print("PLAY?");
}

// Main entry point
int main() {
    uBit.init();
    srand(time(NULL));
    
    uBit.display.print("START");
    
    // Event listener for button A
    uBit.buttonA.onPress(on_button_a_pressed);
    
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