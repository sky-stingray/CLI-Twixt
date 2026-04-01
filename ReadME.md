# CLI - TWIXT

## Project Structure

```
.
├── game_types.h         # Common data structures and enums
├── board.h/c            # Board initialization and display
├── game_logic.h/c       # Game rules and peg placement logic
├── win_checker.h/c      # Win condition checking algorithms
├── input_handler.h/c    # User input handling
├── main.c               # Main game loop
├── Makefile             # Build configuration
└── README.md            # This file
```

## Module Descriptions

### game_types.h
- Defines all shared data structures (GameState, Player, Position, etc.)
- Contains game constants (BOARD_SIZE, MAX_CONNECTION)

### board module
- `init_board()`: Initializes empty game board
- `display_board()`: Renders the game board to terminal
- `get_player_symbol()`: Returns colored/uncolored player symbols

### game_logic module
- `is_valid_position()`: Validates peg placement rules
- `connections_intersect()`: Checks if two line segments intersect
- `is_valid_connection()`: Validates connection between pegs
- `add_connection()`: Creates bidirectional connection between pegs
- `place_peg()`: Places peg and auto-connects to nearby pegs

### win_checker module
- `check_winner()`: Determines if either player has won
- `has_path_to_goal()`: Checks if player has connected path to goal
- `dfs_path()`: Depth-first search to find winning paths

### input_handler module
- `get_player_move()`: Handles user input and validates format

## Building

### Compile the game:
```bash
make game
```

### Compile everything:
```bash
make all
```

## Running

### Run the game:
```bash
make run_game
# or
./game
```

## Game Rules

- **Board**: 24x24 grid
- **RED Goal**: Connect from top row (row 1) to bottom row (row 22)
- **BLACK Goal**: Connect from left column (col 1) to right column (col 22)
- **Movement**: Pegs connect via knight's moves (L-shaped)
- **Connections**: Auto-created between pegs 1 knight-move apart
- **Invalid Moves**: Lines cannot intersect with existing connections

## Clean Build

```bash
make clean
```

This removes all compiled objects and executables.