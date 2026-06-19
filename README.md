<div align="center">

<img width="100%" src="https://svg-banners.vercel.app/api?type=glitch&text1=Chess%20Engine&width=900&height=160&textColor1=58a6ff&textColor2=ffa657" />

<h3>CLI Chess Game — C++ Implementation with Linked Lists & Stacks</h3>

<p><i>Full legal move validation, undo/redo, and castling — built entirely from scratch in C++ using core data structures.</i></p>

<br/>

![C++](https://img.shields.io/badge/C++17-0d1f3c?style=flat-square&logo=c%2B%2B&logoColor=58A6FF)
![Data Structures](https://img.shields.io/badge/Linked_Lists_%26_Stacks-0d1f3c?style=flat-square&logoColor=58A6FF)
![OOP](https://img.shields.io/badge/OOP-0d1f3c?style=flat-square&logoColor=58A6FF)
![GCC](https://img.shields.io/badge/GCC_%2F_Clang-0a1a0a?style=flat-square&logo=gnu&logoColor=22c55e)
![CLI](https://img.shields.io/badge/CLI_App-0a1a0a?style=flat-square&logoColor=22c55e)
![License](https://img.shields.io/badge/License-MIT-1a0000?style=flat-square&logoColor=cc3333)

<br/>

<a href="#overview">Overview</a> &nbsp;·&nbsp;
<a href="#architecture">Architecture</a> &nbsp;·&nbsp;
<a href="#core-logic">Core Logic</a> &nbsp;·&nbsp;
<a href="#data-structures">Data Structures</a> &nbsp;·&nbsp;
<a href="#usage">Usage</a>

</div>

---

## Overview

A fully functional CLI chess engine built in C++ as a Programming Fundamentals project. The board is implemented as a **doubly linked list** rather than a 2D array — a deliberate data structures exercise. Move history is tracked via **stacks** to enable true undo/redo without any external libraries.

```text
Language        →  C++17 — no external libraries, stdlib only
Board Model     →  Doubly linked list (64 Node objects, bidirectional traversal)
Move History    →  Stack-based undo/redo across the full game
Validation      →  Per-piece legal move logic + castling + repetition detection
Players         →  Two-player local — uppercase (White) vs lowercase (Black) 
```

**Engineering decisions worth noting:**
- Board implemented as a doubly linked list instead of a 2D array — traversal mirrors how piece movement logic naturally chains cells together
- Undo/redo use two separate stacks: popping from the undo stack pushes onto the redo stack, giving correct bidirectional history with O(1) operations
- `unordered_map<string, int> board_state_count` tracks position frequency for threefold repetition detection — the board state is hashed to a string key at each move
- Castling eligibility tracked via boolean flags (`white_king_moved`, `black_king_moved`, rook equivalents) rather than scanning move history on each turn, keeping validation O(1)

---

## Architecture

```
main.cpp
│
├── Global State
│   ├── board_state_count (unordered_map)   ← repetition detection
│   ├── white_king_moved / black_king_moved  ← castling eligibility
│   └── rook movement flags (4 total)
│
├── Structs
│   ├── Move      ← from, to, moved piece, captured piece
│   └── Node      ← board cell: piece char, position, movement status
│
├── Class: LinkedList (Chessboard)
│   ├── init()                              ← standard starting position
│   ├── getCell(position)                   ← O(n) traversal to cell
│   ├── printBoard()                        ← CLI board render
│   ├── undoMove()                          ← pop undo stack, push redo stack
│   └── redoMove()                          ← pop redo stack, push undo stack
│
├── Move Validation
│   ├── valid_move(piece, is_white)         ← dispatcher
│   ├── pawn_move()
│   ├── knight_move()
│   ├── rook_move()
│   ├── bishop_move()
│   ├── queen_move()
│   └── king_move()
│
├── Special Rules
│   ├── updateMovementFlags(Node*)          ← updates castling flags post-move
│   ├── executeKingSideCastling()
│   └── executeQueenSideCastling()
│
└── Game Loop
    └── turn_change(bool&)                  ← alternates White / Black
```

---

## Data Structures

### Chessboard — Doubly Linked List

Each of the 64 squares is a `Node` object linked bidirectionally. Piece movement traverses the list by following `next` / `prev` pointers rather than computing array indices.

```cpp
struct Node {
    char piece;         // piece identifier ('K', 'q', '.' for empty)
    string position;    // algebraic notation e.g. "e4"
    bool hasMoved;      // used for castling and pawn first-move logic
    Node* next;
    Node* prev;
};
```

### Move History — Dual Stack

```cpp
stack<Move> undoStack;   // push on every executed move
stack<Move> redoStack;   // populated when undoing; cleared on new move
```

### Board State Tracking — Hash Map 

```cpp
unordered_map<string, int> board_state_count;
// key: serialized board string  →  value: occurrence count
// threefold repetition → draw condition
```

---

## Core Logic

### Move Validation

Each piece has its own validation function enforcing legal movement rules:

| Piece | Key Rules Enforced |
|:--|:--|
| Pawn | Forward-only movement · diagonal capture · en passant ready · first-move double step |
| Knight | L-shape jump · only piece that can skip over others |
| Rook | Straight-line · path clear check · castling flag update |
| Bishop | Diagonal-only · path clear check |
| Queen | Rook + Bishop combined rules |
| King | Single-step any direction · castling eligibility · no moving into check |

### Castling

Both king-side and queen-side castling implemented with full eligibility checks:
- Neither king nor relevant rook has previously moved
- No pieces between king and rook
- King not currently in check, not passing through check, not landing in check

### Undo / Redo

```text
Execute Move  →  push to undoStack, clear redoStack
Undo          →  pop undoStack → restore board state → push to redoStack
Redo          →  pop redoStack → re-apply move → push back to undoStack
```

---

## File Structure

```
chess-engine/
│
├── main.cpp       # All game logic — board, pieces, validation, game loop
└── README.md
```

---

## Usage

**Prerequisites:** GCC or Clang · C++17 support

```bash
# Clone the repository
git clone https://github.com/nayar-900/chess-engine.git
cd chess-engine

# Compile
g++ -std=c++17 main.cpp -o chess

# Run
./chess
```

### Gameplay

| Player | Pieces | Notation |
|:--|:--|:--|
| Player 1 (White) | Uppercase — `K Q R B N P` | Moves first |
| Player 2 (Black) | Lowercase — `k q r b n p` | Moves second |

- Enter moves in the format prompted by the program
- Type `undo` to reverse the last move
- Type `redo` to reapply an undone move

---

## Author

**Muhammad Rayan Badar** — BS Computer Science, Namal University Mianwali

<a href="https://www.linkedin.com/in/rayan-badar-b64542367/"><img src="https://img.shields.io/badge/LinkedIn-0a1628?style=flat-square&logo=linkedin&logoColor=58A6FF" /></a>
&nbsp;
<a href="mailto:rayanbadar900@gmail.com"><img src="https://img.shields.io/badge/Email-0a1628?style=flat-square&logo=gmail&logoColor=cc3333" /></a>
&nbsp;
<a href="https://github.com/nayar-900"><img src="https://img.shields.io/badge/GitHub-0a1628?style=flat-square&logo=github&logoColor=ffffff" /></a>

---

<div align="center">

![footer](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/colored.png)

<sub>Chess Engine &nbsp;·&nbsp; Namal University — Programming Fundamentals &nbsp;·&nbsp; MIT License</sub>

</div>
