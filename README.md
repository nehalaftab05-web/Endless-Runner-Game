# Endless-Runner-Game
# 🏃 Endless Runner — C++ Console Game

> A real-time, terminal-rendered endless runner built in C++ with obstacle avoidance, coin collection, a chasing enemy, dynamic difficulty scaling, and a persistent high score leaderboard.

**Language:** C++ · **Platform:** Windows (Console) · **Renderer:** Raw console via Win32 API

---

## 🎮 Gameplay

You are `P` — running through an infinite scrolling corridor. Dodge obstacles, collect coins, and survive as long as possible. After 2000 units of distance, an enemy `E` spawns and actively hunts you down.

```
##############################
#     C        ░░░           #
#          ░░░        C      #
#    ░░░                     #
#                 ░░░        #
#        C                   #
#                      P     #
##############################
Score: 80    Lives: 3    Distance: 1450
```

The game gets faster the further you go — and it never ends until you run out of lives.

---

## ✨ Features

| Feature | Details |
|---|---|
| **Scrolling map** | Obstacles and coins scroll downward each tick, simulating forward movement |
| **Jump & Slide** | Dodge obstacles above and below — can't jump and slide simultaneously |
| **Coin collection** | Coins award 10 points each on contact |
| **Enemy AI** | Spawns at distance ≥ 2000; tracks the player's X position and closes in each tick |
| **Enemy dodging** | Jump or slide at the moment of enemy contact to avoid a life loss |
| **Dynamic speed** | Game loop delay drops from 250ms → 50ms as distance increases |
| **Lives system** | 3 lives; lost on obstacle collision or enemy hit |
| **Persistent leaderboard** | Scores saved to `highscores.txt`; sorted and displayed on demand |
| **Unique name enforcement** | Duplicate player names are rejected before a run starts |

---

## 🕹️ Controls

| Key | Action |
|---|---|
| `A` | Move left |
| `D` | Move right |
| `W` | Jump (lasts 3 ticks) |
| `S` | Slide (lasts 3 ticks) |

> Input is non-blocking — the game continues scrolling whether or not you press a key.

---

## ⚡ Difficulty Scaling

Speed increases automatically based on distance traveled:

| Distance | Loop Delay |
|---|------------|
| 0 – 999 | 250ms |
| 1000 – 1999 | 200ms |
| 2000 – 3999 | 150ms ← Enemy spawns here |
| 4000 – 5999 | 100ms |
| 6000+ | 50ms |

---

## 🗺️ Map & Symbols

| Symbol | Meaning | Color |
|--------|---------|-------|
| `P` | Player | Green |
| `^` | Player (jumping) | Green |
| `v` | Player (sliding) | Green |
| `E` | Enemy | Red |
| `C` | Coin (+10 pts) | Yellow |
| `░` (219) | Obstacle (-1 life) | Gray |
| `#` | Wall | Magenta |

Map dimensions are defined in `game.h`:
```cpp
const int Map_width = 30, Map_height = 20;
```

---

## 🏗️ Architecture

```
main.cpp
└── showMenu()          — Main menu loop (Play / Scores / Exit)
    ├── runGame()        — Game state init + main game loop
    │   ├── setupMap()   — Initialize wall borders and empty cells
    │   ├── drawMap()    — Render full frame to console each tick
    │   ├── genObst_Coins() — Randomly populate new top row each scroll
    │   ├── scrollMap()  — Shift all rows down by one
    │   ├── updateEnemy()— Move enemy toward player; check collision
    │   ├── updateGame() — Handle input, jump/slide timers, collisions
    │   ├── saveScore()  — Append name/score/distance to highscores.txt
    │   └── GameOverScreen() — Display end screen
    └── viewScore()      — Read, sort (bubble), and display leaderboard

game.h
└── Constants, extern declarations, function prototypes
```

---

## 💾 Data Persistence

Scores are saved to `highscores.txt` in CSV format:

```
Alice,120,3500
Bob,80,2050
Zara,200,6100
```

Each row stores: `name,score,distance`

- Appended after every run via `ofstream` (`ios::app`)
- Read back and sorted by score (descending) using bubble sort for display
- Duplicate names are blocked — each name can only appear once across all runs

---

## 🧠 Implementation Notes

**Flicker-free rendering** — `drawMap()` resets the cursor to `(0, 0)` each frame instead of clearing the screen, preventing the console flash typical of `system("cls")`.

**Non-blocking input** — `_kbhit()` checks for a keypress each tick without halting the game loop. If no key is pressed, the game advances with a null input.

**Jump/Slide timers** — Both actions use a countdown counter (`jumpC`, `slide_C`) that decrements each tick, automatically resetting the state after 3 frames. This prevents action stacking.

**Enemy AI** — The enemy increments/decrements its X position toward the player's X each tick, and advances one row downward, creating a diagonal chase. A mid-air or sliding dodge at the moment of collision resets the enemy without a life penalty.

**Obstacle generation probability:**
```
10% chance → Obstacle
5%  chance → Coin  (1 in 20)
85% chance → Empty
```

---

## 🛠️ Build & Run

### Requirements
- Windows OS
- C++ compiler with Win32 support (`g++` via MinGW, or MSVC)

### Compile (MinGW / g++)

```bash
g++ main.cpp game.cpp -o EndlessRunner.exe -std=c++17
```

### Compile (MSVC)

```bash
cl main.cpp game.cpp /Fe:EndlessRunner.exe
```

### Run

```bash
./EndlessRunner.exe
```

> `highscores.txt` is created automatically in the same directory on the first completed run.

---

## 📂 File Structure

```
endless-runner/
│
├── main.cpp            # Menu, entry point
├── game.cpp            # All game logic, rendering, file I/O
├── game.h              # Constants, externs, function prototypes
├── highscores.txt      # Auto-generated leaderboard (CSV)
└── README.md
```

---

## 📄 License

Personal C++ project. Not licensed for redistribution or commercial use.
