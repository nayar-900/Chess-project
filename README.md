# Chess Game Implementation

This repository contains a C++ implementation of a basic chess game using linked lists and stacks. The game includes features such as move validation, undo/redo functionality, and castling. Below is a detailed breakdown of the project, its components, and how to use it.

## Features
- **Chessboard Representation**: The board is implemented as a doubly linked list.
- **Move Validation**: Includes specific logic for each chess piece.
- **Undo/Redo Moves**: Tracks moves using stacks to allow undo and redo operations.
- **Castling**: Implements both king-side and queen-side castling.

## Prerequisites
- A C++ compiler (e.g., GCC or Clang).
- Basic knowledge of C++ programming.
- Familiarity with chess rules.

## File Structure
- **`main.cpp`**: Contains the core logic of the chess game.
- **`README.md`**: This file, explaining the project.

## Code Overview
### Global Variables
- **`unordered_map<string, int> board_state_count`**: Tracks the frequency of board states for detecting repetition.
- Flags such as `white_king_moved`, `black_king_moved`, etc., to track special conditions like castling.

### Structs
- **`Move`**: Represents a move with details such as starting and ending positions, moved piece, and captured piece.
- **`Node`**: Represents a cell on the chessboard with attributes like the piece it holds, its position, and movement status.

### Classes
- **`LinkedList`**: Represents the chessboard as a doubly linked list. Includes methods for:
  - Initializing the board.
  - Retrieving a specific cell.
  - Printing the board.
  - Undoing and redoing moves.

### Core Functions
- **`turn_change(bool&)`**: Alternates between player turns.
- **`valid_move(char, bool)`**: Validates if a move is allowed for the current player.
- **`pawn_move`**, **`knight_move`**, **`rook_move`**, **`bishop_move`**, **`queen_move`**, **`king_move`**: Define movement rules for each piece.
- **`updateMovementFlags(Node*)`**: Updates special flags based on piece movements.
- **`executeKingSideCastling`**, **`executeQueenSideCastling`**: Handle castling logic.

### Initialization
The **`init`** function sets up the chessboard with the standard initial configuration.

## Usage
### Compilation
Use a C++ compiler to compile the program:
```bash
g++ main.cpp -o chess
```

### Execution
Run the compiled program:
```bash
./chess
```

### Gameplay
- The game alternates turns between Player 1 (uppercase pieces) and Player 2 (lowercase pieces).
- Enter moves in the format specified by the program.
- Use the undo/redo functionality as needed.

## Acknowledgments
- Special thanks to resources and references used during development.

Enjoy playing chess and contributing to this project!

