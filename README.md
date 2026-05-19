# SCC.131 Reaction Time Game

## BBC micro:bit v2 ARM Assembly Reaction Timer

A reaction-time measurement game implemented for the BBC micro:bit v2 using C/C++ and ARM assembly.

---

# Project Overview

This project was developed as part of the SCC.131 coursework assignment.  
The aim of the project is to create a reaction-time game that combines:

- C/C++ programming
- ARM assembly programming
- hardware interaction on the BBC micro:bit
- real-time timing measurement

The game measures how quickly a player reacts after a visual signal appears on the micro:bit LED display.

---

# Features

## Core Features

- C/C++ game logic using the CODAL runtime
- ARM assembly timer function
- Real-time reaction measurement
- Random delay generation
- LED matrix feedback
- Button-based controls
- micro:bit v2 compatibility

---

## Additional Features

- High score tracking
- Average score calculation
- Statistics display
- Runtime score history
- False-start prevention
- Randomized reaction cue timing

---

# Hardware Used

- BBC micro:bit v2
- ARM Cortex-M4 processor
- 5x5 LED matrix
- Button A
- Button B

---

# Technologies Used

| Technology | Purpose |
|---|---|
| C++ | Main game logic |
| ARM Assembly | Reaction timer |
| CODAL | micro:bit runtime |
| ARM GCC | Compilation |
| Python build.py | Build system |

---

# Project Structure

```text
microbit-v2-samples/
│
├── source/
│   ├── main.cpp
│   └── timer.s
│
├── build.py
├── libraries/
└── README.md
