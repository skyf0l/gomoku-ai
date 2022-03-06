# Gomoku - Epitech Artificial Intelligence Project

Implementation of a Gomoku game bot in cpp for Piskvork gomoku manager.

# PPAY ai characteristics

- Minimax algorithm
- Alpha-beta pruning
- Move order generator
- Transposition table
- Symmetry detection
- Optimized heuristic evaluation
- Loosing and winning detection

# How create my bot

Look at [random_brain.cpp](src/random_brain.cpp), you need to implement some functions like `brainInit`, `brainRestart`, `brainMove` and `brainEnd`.

Derived from [brain_code](include/core/brain_core.hpp), all communications with the manager are done through this class.
You can use it to get infos about the game, to send commands to the manager and to get the response from the manager.

## Beware of -42 Epitech students
