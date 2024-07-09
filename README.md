# Go Game AI Player

This project is a C++ implementation of an AI player for the game of Go, capable of making optimal moves based on the current game state using the minimax algorithm with alpha-beta pruning.

## Requirements

- C++ compiler (e.g., g++)
- Input files: `input.txt`, `moves.txt`
- Output files: `output.txt`, `moves.txt`

## Getting Started

### Installation

Clone the repository to your local machine:
```bash
git clone https://github.com/your-username/go-game-ai-player.git
cd go-game-ai-player
```

### Compilation

Compile the `player.cpp` source code:
```bash
g++ player.cpp -o player
```

### Usage

1. **Input Files**:
   - **input.txt**: Contains the current and previous game states.
     ```
     <player_color>
     <previous_state_row_1>
     ...
     <previous_state_row_5>
     <current_state_row_1>
     ...
     <current_state_row_5>
     ```
     - `<player_color>`: '1' for black, '2' for white.
     - `<previous_state_row>` and `<current_state_row>`: Each row consists of 5 characters ('0' for empty, '1' for black, '2' for white).

   - **moves.txt**: Contains the number of moves made so far.
     ```
     <number_of_moves>
     ```

2. **Running the Program**:
   - Execute the compiled program:
     ```bash
     ./player
     ```

3. **Output Files**:
   - **output.txt**: Contains the AI's next move in the format `x,y` (row and column) or `PASS` if no valid move is found.
     ```
     <x>,<y>
     ```
     - `<x>`, `<y>`: Coordinates of the next move (0-indexed).

   - **moves.txt**: Updated with the number of moves made after each execution.
     ```
     <updated_number_of_moves>
     ```

### Example Workflow

1. **Initial Setup**:
   - Create `input.txt` and `moves.txt` with initial values as specified.
   
2. **Compile and Run**:
   - Compile the program using `g++`.
   - Run the compiled executable `player`.

3. **Check Outputs**:
   - Examine `output.txt` for the AI's calculated move.
   - Update `input.txt` and `moves.txt` with the new game state and move count for subsequent runs.

### Notes

- The AI player uses the minimax algorithm with alpha-beta pruning to evaluate possible moves based on current and previous game states.
- Adjust the `maxDepth` variable in `player.cpp` to control the depth of the search tree and influence AI's decision-making process.

---
