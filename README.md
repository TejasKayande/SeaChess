# SeaChess

## Overview

**SeaChess** is my second iteration of a chess engine written in C/C++.

This version is a **complete architectural rewrite** of a previous implementation, with a focus on building a scalable and maintainable engine core.

---

## Why a Rewrite?

The first version achieved functional progress, including board representation and core mechanics. However, as complexity increased, several issues became clear:

* Tight coupling between board state, rules, and move logic
* Implicit state handling (deriving state from board instead of tracking it explicitly)
* Poor separation between engine, platform, and rendering layers
* Increasing difficulty in extending features like move generation and validation

Rather than continuing to patch these issues, the engine was rebuilt with a stronger architectural foundation.

---

## Core Design Principles

* **Separation of concerns**: Attack generation, move generation, and board state are isolated
* **Explicit data flow**: No hidden or implicit state derivation
* **Bitboard-first design**: Performance-oriented representation from the ground up
* **Scalability**: Designed to support search, evaluation, and multiple frontends

---

## Architecture

The engine is structured around two core layers:

### Attack Layer

Handles raw geometric movement independent of game rules:

* Precomputed attack tables:

  * Pawn
  * Knight
  * King

* Dynamic attack generation for sliding pieces:

  * Bishop
  * Rook
  * Queen

This layer is **pure and stateless**, operating only on board geometry and occupancy.

---

### Pseudo-Legal Move Generation

Applies game rules on top of attack data:

* Filters out friendly collisions
* Identifies captures vs quiet moves
* Generates move lists per piece type

This separation ensures:

* Cleaner logic
* Easier debugging
* Extensibility toward legal move generation

---

## Improvements Over Previous Version

### Structural Improvements

* Clear separation between:

  * Attack generation
  * Move generation
  * Board representation

* Reduced coupling across components

* Removal of implicit state inference

* More predictable and testable logic

---

### Performance Improvements

* Efficient bitboard iteration using LSB extraction
* Precomputed attack tables for constant-time lookups
* Occupancy-aware sliding piece generation
* Reduced redundant computation in move filtering

---

### Code Quality Improvements

* Consistent data flow and naming
* Elimination of fragile, indirect logic
* Easier reasoning about move generation
* Designed for future extensibility

---

## Current Features

* Bitboard-based board representation

* Pseudo-legal move generation for:

  * Pawn
  * Knight
  * Bishop
  * Rook
  * Queen
  * King

* Precomputed attack tables:

  * Pawn
  * Knight
  * King

* Occupancy-based sliding attack generation

---

## Work in Progress / Next Steps

* Legal move generation (check detection)
* Pawn promotions
* En passant
* Castling
* Move encoding refinement
* Search (Minimax / Alpha-Beta pruning)
* Evaluation function

---

## Known Limitations (Current Stage)

* Move encoding is minimal and will be extended to support:

  * Promotions
  * Special moves

* Pawn logic will be further modularized for:

  * Pushes
  * Captures
  * Special cases

* Some internal APIs (e.g., square vs index usage) will be standardized

---

## Tech Stack

* C / C++ (core engine)
* Bitboards for performance-critical logic

Planned:

* WebAssembly (WASM) for browser execution
* C++ backend server exposing engine APIs
* Multi-platform UI clients (web, desktop)

---

## Lessons Learned

* Architectural mistakes scale faster than features
* Separating concerns early prevents exponential complexity
* Rewriting with better structure is often more efficient than patching
* Performance optimizations are only meaningful when built on clean design

---

## Status

Work in progress...

---

## Author

Built as part of a deeper exploration into:

* Low-level systems programming
* Performance-oriented design
* Game engine architecture

---
