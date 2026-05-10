# SCC.131 Reaction Time Game

A reaction time measurement game implemented for the BBC micro:bit using C/C++ and ARM v7 assembly.

## Features

- **C/C++ Interface**: User interaction and game flow control
- **ARM Assembly Timer**: Accurate cycle-based timing using DWT_CYCCNT
- **Accurate Measurement**: Nanosecond precision using CPU cycle counter
- **High Score Tracking**: Tracks and displays best reaction time
- **False Start Detection**: Detects cheating attempts
- **Audio/Visual Cues**: Sound and LED feedback

## Building

### Prerequisites

- micro:bit SDK (yotta or mbed-cli)
- ARM GCC compiler
- Python 2.7+ (for build tools)

### Build Commands

```bash
# Using mbed-cli (Recommended)
mbed compile -t GCC_ARM -m MICROBIT

# Using yotta
yotta build
```

### Flashing

```bash
# Copy .hex file to micro:bit
cp BUILD/MICROBIT/GCC_ARM/reaction_time_game.hex /media/MICROBIT/
```

## How to Play

1. Press **Button A** to start the game
2. Device displays countdown (1-10 seconds)
3. Listen for audio cue when ready
4. **Press Button A as quickly as possible**
5. Your reaction time in milliseconds is displayed
6. High score is tracked across games

## Technical Details

### Part 1: C/C++ Interface (10%)
- Event-driven button handling
- Random delay generation (1-10 seconds)
- Game state management
- Display output

### Part 2: Assembly Timer (60%)
- Uses **DWT_CYCCNT** register for cycle counting
- Polling-based button detection
- Cycle-to-millisecond conversion
- Accurate to CPU cycle precision

### Part 3: Integration (10%)
- Both C++ and assembly work together
- Seamless function calls between languages
- Results displayed on micro:bit matrix

### Part 4: Additional Features (10%)
- High score persistence during runtime
- False start detection (reacts before cue)
- Error handling and validation
- Visual feedback for different game states

## Typical Reaction Times

- **Excellent**: < 150 ms
- **Good**: 150-250 ms
- **Average**: 250-300 ms
- **Slow**: > 300 ms

## Marking Scheme

- Part 1: C/C++ Interface (10%)
- Part 2: Assembly Timer (60%)
- Part 3: Integration & Flashing (10%)
- Part 4: Additional Requirement (10%)
- Code Style & Documentation (5%)
- Version Control (5%)

## Implementation Notes

✅ **DWT_CYCCNT** provides nanosecond-level precision  
✅ **Polling I/O** chosen for simplicity (can be justified in demo)  
✅ **Random delay** between 1-10 seconds as required  
✅ **Error handling** for false starts  
✅ **High score tracking** as additional feature  
✅ **Full documentation** and comments included  

## License

This project is for educational purposes only.