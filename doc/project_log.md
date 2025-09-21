# Project Log – 3 Stones (C++)

- **Progress so far**
  - Reviewed project instructions and rubric.  
    - Files: doc/manual.md, doc/project_log.md, cppRubric.docx  
    - Time: 30m  
    - Notes: Clarified submission requirements and grading expectations.  

  - Created initial folder structure and C++ skeleton stubs.  
    - Files: src/main.cpp, board.hpp/.cpp, player.hpp, human.hpp/.cpp, computer.hpp/.cpp, round.hpp/.cpp, tournament.hpp/.cpp, serializer.hpp/.cpp, types.hpp, move.hpp, board_view.*  
    - Time: 1h 30m  
    - Notes: Established baseline classes, headers, and wiring for the game loop.  

  - Implemented basic Board shape and ASCII printing.  
    - Files: board.hpp/.cpp, board_view.*  
    - Time: 1h  
    - Notes: Octagon 11×11 with center void, row/col numbering.  

  - Added Human player input (row, col, stone).  
    - Files: human.hpp/.cpp  
    - Time: 45m  
    - Notes: Prompt for input; includes basic inventory check.  

  - Added Computer random move generator.  
    - Files: computer.hpp/.cpp  
    - Time: 40m  
    - Notes: Selects random legal move; strategies to come in Milestone 2.  

  - Implemented Round logic (turn alternation, legality, applyMove).  
    - Files: round.hpp/.cpp  
    - Time: 1h  
    - Notes: Enforced row/col rule with fallback. Added simple move explanation.  

  - Implemented Tournament tracking (round outcome, winner).  
    - Files: tournament.hpp/.cpp  
    - Time: 20m  
    - Notes: Records rounds won and determines winner.  

  - Added Serializer save stub.  
    - Files: serializer.hpp/.cpp  
    - Time: 30m  
    - Notes: Basic text save implemented; load deferred to Milestone 2.  

  - Verified compilation and run inside VS Code terminal.  
    - Files: all (build process)  
    - Time: 20m  
    - Notes: Game loop runs successfully; human/computer turns alternate.  

  - Configured VS Code build and debug (tasks.json and launch.json).  
    - Files: .vscode/tasks.json, .vscode/launch.json  
    - Time: 30m  
    - Notes: Play button now builds all sources with g++ and runs `three_stones`.  

**Total time:** 6h 5m

2025-09-14

- Edited `board.cpp` to fix comparisons in `hasEmptyInRow` and `hasEmptyInCol` so that they compare `Pocket.s` against `Stone::Empty` instead of comparing `Pocket` to `Stone`. (20 minutes)

- Updated `types.hpp` to clean up stone conversion helpers:
  - Removed duplicate overloads of `stoneToChar`.
  - Kept a `char stoneToChar(Stone)` and added `std::string stoneToString(Stone)` wrapper. (15 minutes)

- Enhanced `Round::applyMove` in `round.cpp` to build a clear `lastExplanation_` string that reports the stone placed, its coordinates, and points scored for both players. (25 minutes)

- Modified `main.cpp` game loop:
  - Added retry logic so human players are explicitly re-prompted on illegal moves.
  - Suppressed “Illegal move” spam for the computer by retrying silently.
  - Added a maximum retry safeguard to avoid infinite loops. (40 minutes)

- Fixed misplaced closing brace in `main.cpp` so round wrap-up and tournament messages appear *after* the loop. (10 minutes)

**Total time today:** ~1 hour 50 minutes


---

# 📝 `doc/project_log.md` (Milestone 2 entry)

```markdown
# Project Log — John Warren
CMPS 366 — 3 Stones
Milestone 2

---

## 2025-09-16
**Design (0.5h)**  
- Reviewed Milestone 2 requirements from instructions and rubric.
- Planned strategy improvements for Computer: self-3, block-3.

**Coding (1.5h)**  
- Implemented `Computer::recommendForHuman` and move rationale strings.
- Updated `Human` to support help key (`H`) and print best suggestion.
- Extended `Round::applyMove` to record rationale.
- Fixed Player constructor usage in Human/Computer to align with `PlayerKind`.
- Integrated serialization prompt after each move.

**Debugging (1.0h)**  
- Resolved multiple compile errors (curly bracket mismatch, missing includes, wrong type signatures).
- Fixed `stoneToChar` overload issues and vector-related errors in Computer class.
- Corrected help callback wiring (`helpCb` → `help`).
- Adjusted serialization load to restore `lastOppCoord` and next player correctly.
- Verified color consistency in help mode (suggestions match human’s actual color).

**Testing (1.0h)**  
- Ran full round to confirm:
  - Computer move rationales print.
  - Help suggestions appear with correct color.
  - Illegal move spam suppressed.
  - Serialization works: save to `save.txt` and resume properly.
- Verified scoring triples in rows and diagonals award points correctly.

**Total: 4.0h**

---

## Summary
Milestone 2 is complete.  
- Computer implements at least two strategies (completing own 3-in-a-row, blocking opponent).  
- Computer provides rationale for each move.  
- Human help mode suggests legal plays and best strategy.  
- Full serialization/resume is operational.  
- Documentation (manual, log, rubric) updated.

# Project Log — John Warren
## CMPS 366 — 3 Stones
## Sep 20, 2025

Sep 20, 2025

Implemented robust file parsing in serializer.cpp: added trim and starts_with_label, tolerated extra spaces, and matched the professor’s sample format for “Human Player”, “Computer Player”, “Next Player”, and “Board:” lines 
(1.5 hours).

Fixed strlen compile error by including <cstring> and switched equality check to use label-length comparison in starts_with_label 
(20 mins).

Reworked inventory loading path: added Player::setInventory(const Inventory&) and used it during load to set myColor, counts, points, and roundsWon atomically; removed illegal brace-initializer assignment 
(45 mins).

Verified loading of professor’s gen.txt and other samples; identified mismatch in our save format vs. load format and standardized both directions (save now mirrors load, including spacing and row tokenization) 
(1 hour).

Persisted and restored turn state: wrote/parsed “Next Player: … r c” so round.initFromLoad(nextIsHuman, lastOpp) restores whose turn it is and the last move coordinate; fixed duplicate stub definition vs. implementation 
(30 mins).

Main loop UX: on resume, printed “Next to move” and “Last move … at (r,c)” with reminder of the row/column restriction; added reminder line before each turn if a last move exists 
(45 mins).

Enforced legality on resume: confirmed “must play in same row/col (unless both full)” by checking anyAvailableOnRowOrCol() and rejecting moves off-line; reproduced and fixed a case where off-line move was incorrectly accepted after load 
(1 hour).

Help system stability: reconnected Human help callback naming (help_), ensured ‘H’ re-prompts instead of consuming a stone, and displayed the computer’s suggestion with rationale when available 
(30 mins).

Messaging polish: prefixed move explanations with actor (“Human”/“Computer”), used stoneToString() consistently in messages, and confirmed score deltas print after each legal move 
(30 mins).

Name prompt: added Human::inputName() and integrated a friendly welcome banner; wired the name into the “Next to move”/“Last move” display for clarity 
(30 mins).

Total hours today: 7 hours, 10 minutes
