---
output:
  pdf_document: default
  html_document: default
---
# Project Manual — 3 Stones (Milestone 1)

## Bug Report
- No known bugs in Milestone 1 features (basic game loop works).
- Scoring is stubbed out (always returns 0) – this is expected for Week 1.
- Resume/load not implemented yet.

## Screenshots

### Coin toss
![Coin toss input/output](screenshots/coin-toss.png)

### Human move
![Human entering row/col and stone](screenshots/human_input.png)

### Computer move
![Computer placed move](screenshots/computer-move.png)

### Full board view
![Board with multiple stones and row/col numbers](screenshots/computer-move.png)


## Feature Report
### Missing (expected in later milestones)
- Advanced computer strategies.
- Help mode recommendations for the human player.
- Resume/load from file.
- Scoring logic for 3-in-a-row.


### Extra
- None implemented beyond requirements for Milestone 1.

## Data Structures / Classes
- **Board**: 11×11 octagon with 80 valid pockets, manages placement.
- **BoardView**: prints board with row/col numbers.
- **Move**: coordinate + stone struct.
- **Player (abstract)**: base for Human and Computer.
- **Human**: reads input, checks inventory.
- **Computer**: currently plays a random valid move.
- **Round**: manages turn order, legality, placement, updates last move.
- **Tournament**: tracks rounds won.
- **Serializer**: saves game state (load coming in Milestone 2).

## Log
See `project_log.md` (embed or append).

## How to Run
```bash
g++ src/*.cpp -std=c++17 -O2 -o three_stones
./three_stones

- Use the Serializer `save()` call (to be wired into menu) to write current state to a text file per the required format.

## Generative AI Assistance

I used generative AI (ChatGPT) in limited ways to support my development process:

- **Project Setup & Structure**  
  Helped outline a clean folder structure and starter C++ class skeletons to save time getting organized.  
  *(Category: design/documentation)*

- **Documentation Templates**  
  Assisted with creating the initial draft for the project manual and project log format.  
  *(Category: documentation)*

- **Coding Skeletons**  
  Supplied stubs for classes (Board, Player, Human, Computer, Round, Tournament, Serializer) so I could focus on filling in logic.  
  *(Category: coding skeletons, not final implementations)*

