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


# Project Log — 2025-09-21 (Sun)

## Student: John Warren
### Course: CMPS 366 — 3 Stones
### Session Times (EST): 9:10–12:35, 1:15–4:45 (≈6h 55m total)

### Goals for Today

Add “Play another round?” tournament loop per spec.

Make Resume robust: load our save.txt and instructor samples (gen.txt, winning.txt).

Ensure resume shows who moves next and the last move (so row/col legality is clear).

Keep Help (H) correct for the human’s actual color; show rationale.

Minor polish: optional human name input at startup; avoid duplicate prompts.

### Work Completed
- A) Tournament replay loop

What: After each round, prompt “Play another round? (Y/N)” and either start a new Round or end and print tournament winner by rounds won (not points).

Where: main.cpp

Approach (pseudocode):

do {
    setup or resume round
    play round loop
    announce round winner; tally roundsWon
} while (user says Y)
announce tournament winner (by roundsWon)

Why: Matches “The Tournament” section of the assignment.

- B) Resume: loader compatibility & UX

What: Load both our own save.txt and instructor samples (gen.txt, winning.txt) reliably.

Where: serializer.cpp, main.cpp

Key changes:

starts_with_label() + trim() to tolerate spacing after labels.

Board parsing maps tokens to valid pockets per row using the board’s mask (handles tapered octagon + center void).

Next Player: parsing sets nextIsHuman and adjusts lastOpp 1-based→0-based; supports “no last move” with -1 -1.

In main.cpp, after successful load:

Print Next to move and Last move (with 1-based coords and stone).

Print row/col legality reminder.

Removed a duplicate resume block that caused the same file prompt twice.

- C) Help mode correctness

What: Ensure H suggestions honor human’s color and show rationale.

Where: human.cpp, main.cpp

Key changes:

human.setHelpCallback([&cpu](...) { return cpu.recommendForHuman(...); });

readStoneFromInv(...) re-prompts after H (non-consuming), prints rationale when available.

Verified hints as White and Black.

- D) Human name (optional)

What: Added friendly name prompt without breaking serialization paths.

Where: human.hpp/.cpp, main.cpp

Key changes:

Human::inputName() (safe getline w/ leading newline drain).

Human::setHumanName()/getHumanName() (non-breaking; display uses Player::name() for now).

Guarded so Resume flow still works regardless of input name.

- Files Touched (today)

src/main.cpp — tournament loop; single, non-duplicated resume flow; last-move banner; save-and-quit; name prompt.

src/serializer.cpp — tolerant label parsing; valid-pocket mapping; Next Player + lastOpp handling; save/write alignment.

src/human.hpp / src/human.cpp — help callback wiring; color-correct help; inputName, setHumanName, getHumanName.

- Tests Conducted

New -> move-> Save -> Resume (our save.txt)

Observed correct Next to move and Last move banner.

Row/col legality reminder printed.

Computer takes turn first when nextIsHuman == false.

Load instructor samples

gen.txt, winning.txt imported; board shape and stones placed match file.

Legality banner computed from lastOpp. 

Help mode

As White and Black, H suggests a stone color matching the human’s inventory/color and prints rationale.

Tournament flow

After round end, prompt “Play another round? (Y/N)”.

On Y, new round plays; on N, prints tournament winner by roundsWon.

Issues & Resolutions (today)

Duplicate resume prompts
Symptom: Asked for the filename twice.
Fix: Consolidated to a single resume branch in main.cpp; removed stale prompt block.

Instructor files not loading fully
Symptom: Board half-parsed, then “Load failed; starting new game.”
Fix: Use board’s valid pocket mask per row to map tokens; tolerant starts_with_label(); robust Next Player/lastOpp handling.

Help hints wrong color (earlier regression)
Fix: Pass human’s color into recommender; re-prompt after H.

Name prompt interfering with resume
Fix: Read name via getline before mode prompt, draining leading newlines; no impact on loader.

Conformance Notes (rubric alignment snapshot)

Serialization: Save/load works with both our save.txt and the instructor’s formats; last move and next player restored.

Help Mode: Displays a concrete recommended play (stone + pocket) and rationale; respects human’s color/inventory.

Computer Play: Computer only plays stones from its own inventory; prints brief rationale.

Legality: Enforced row/col rule relative to last opponent move; exception when both row & column are full.

Tournament: Replays rounds; winner by rounds won; final announcement on exit.

OOP: Virtual chooseMove, distinct Human/Computer classes, Round orchestrates flow, Serializer isolated, no globals.

Next Steps (for final polishing)

Docs refresh: Manual screenshots (New, Help, Save/Resume, Instructor sample), and today’s log entry.

Comments: Ensure each function header has Purpose/Params/Returns; keep pseudocode bullets for complex functions.

Constants: If any 11/center (5,5) remain in multiple spots, centralize (optional polish).

View separation: Keep model classes print-free where practical (we’re already close).

Status: Feature-complete for demo and rubric; ready for presentation rehearsal.

### Time Spent

Implementation & fixes: ~5h

Testing & verification: ~1h 55m