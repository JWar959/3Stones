/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/

#include "round.hpp"
#include <iostream>
#include <cctype>
#include <limits>
#include <cstdlib>
#include <string>

/*
This function makes sure that round gets fully intialized after a load.
It tracks which player is Black/White from the players inventories, whose turn
it is from nextIsHuman, and the opponents last move lastOppCoord_
*/
void Round::initFromLoad(bool nextIsHuman, Coord lastOpp) {
  // Re-derive black_/white_ from inventories
  black_  = (human_->inv().myColor == Stone::B) ? human_ : computer_;
  white_  = (human_->inv().myColor == Stone::W) ? human_ : computer_;

  // Set next to move
  turn_ = nextIsHuman ? human_ : computer_;

  // Set the “opponent’s last move” anchor for row/col restriction
  lastOppCoord_ = lastOpp;
}

Round::Round(Board& b, Player& human, Player& computer)
  : board_(b), human_(&human), computer_(&computer), black_(human_), white_(computer_), turn_(black_) {}

static char readChar(const std::string& prompt, const std::string& allowed) {
  while (true) {
    std::cout << prompt;
    char ch;
    if (std::cin >> ch) {
      ch = std::toupper(static_cast<unsigned char>(ch));
      if (allowed.find(ch) != std::string::npos) return ch;
    }
    std::cout << "Invalid. Try again.\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

/* *********************************************************************
Function Name: decideFirstPlayerWithCoinCall
Purpose: Handle coin toss and color selection; black moves first.
Parameters: None (uses std::cin for prompts)
Return Value: None
Algorithm:
  1) Human calls H/T; random toss decides winner.
  2) Winner picks color (B/W). The black color plays first.
  3) Assign myColor for inventories accordingly and set turn_.
Reference: None
********************************************************************* */
void Round::decideFirstPlayerWithCoinCall() {
  char call = readChar("Call the coin (H/T): ", "HT");
  char toss = "HT"[std::rand()%2];
  std::cout << "Coin shows: " << toss << "\n";
  bool humanWins = (call==toss);

  if (humanWins) {
    char color = readChar("Pick color (B/W): ", "BW");
    if (color=='B') {
      human_->inv().myColor = Stone::B;
      computer_->inv().myColor = Stone::W;
      black_ = human_; white_ = computer_; turn_ = black_;
    } else {
      human_->inv().myColor = Stone::W;
      computer_->inv().myColor = Stone::B;
      black_ = computer_; white_ = human_; turn_ = black_;
    }
  } else {
    // Computer picks black by default
    computer_->inv().myColor = Stone::B;
    human_->inv().myColor = Stone::W;
    black_ = computer_; white_ = human_; turn_ = black_;
    std::cout << "Computer picked Black and will start.\n";
  }
}

bool Round::isOver() const {
  const auto &bi = black_->inv(), &wi = white_->inv();
  int total = bi.black+bi.white+bi.clear + wi.black+wi.white+wi.clear;
  return total == 0;
}

Player& Round::current() { return *turn_; }
Player& Round::other()   { return (turn_==black_)? *white_ : *black_; }

bool Round::anyAvailableOnRowOrCol(int row, int col) const {
  for (int i=0;i<11;++i) {
    if (board_.isValidPocket(row,i) && board_.isEmpty(row,i)) return true;
    if (board_.isValidPocket(i,col) && board_.isEmpty(i,col)) return true;
  }
  return false;
}

/* *********************************************************************
Function Name: applyMove
Purpose: Enforce legality, place stone, update inventories and points,
         set last opponent coord, and switch turn.
Parameters: m (const Move&) - desired move
Return Value: bool - true if applied; false otherwise
Algorithm:
  1) Validate pocket (valid & empty).
  2) If not the first move, require same row or column as last opponent move,
     unless no pockets are available on that row/col.
  3) Decrement inventory for the stone used; place on board.
  4) Score newly formed triples (stub for M1).
  5) Store explanation, update lastOppCoord_, and switch turn.
Reference: None
********************************************************************* */
bool Round::applyMove(const Move& m) {
  if (!m.isValid) return false;
  if (!board_.isValidPocket(m.pos.r, m.pos.c) || !board_.isEmpty(m.pos.r, m.pos.c)) return false;

  if (lastOppCoord_.r != -1) {
    bool sameRow = (m.pos.r == lastOppCoord_.r);
    bool sameCol = (m.pos.c == lastOppCoord_.c);
    if (!sameRow && !sameCol) {
      if (anyAvailableOnRowOrCol(lastOppCoord_.r, lastOppCoord_.c)) {
        // still have legal pockets tied to last opponent move
        return false; 
      }
    }
  }

  Inventory& inv = turn_->inv();
  if (m.played==Stone::B && inv.black>0) inv.black--;
  else if (m.played==Stone::W && inv.white>0) inv.white--;
  else if (m.played==Stone::C && inv.clear>0) inv.clear--;
  else return false;

  // Place and score
  board_.place(m.pos.r, m.pos.c, m.played);
  auto deltas = board_.scoreFromPlacement(m.pos, m.played, inv.myColor);
  inv.points += deltas.first;
  auto [myAdd, oppAdd] = board_.scoreFromPlacement(m.pos, m.played, inv.myColor);

  // Here we're going to see if any points are being scored, and if they are, we're going to print a debug statement
  if (myAdd || oppAdd) {
      // only now print detailed windows (no spam from AI sims)
      board_.debugPrintScoringWindows(m.pos, inv.myColor);
  }

  other().inv().points += deltas.second;

  // Going to capture who just took a turn, so we can output it to the console along with what the turn was
  const std::string activePlayer = currentIsHuman() ? human_->name() : computer_->name();

  lastExplanation_ =
    activePlayer +
    std::string(" placed ") + stoneToString(m.played) + " at (" +
    std::to_string(m.pos.r + 1) + "," + std::to_string(m.pos.c + 1) + "). " +
    "Scored +" + std::to_string(deltas.first) + " for current player" +
    (deltas.second ? (", opponent +" + std::to_string(deltas.second) + ".")
                   : ".");

  // Print the current points after explanation
  std::cout << "Scores - " << human_->name() << ": " << human_->inv().points
          << ", " << computer_->name() << ": " << computer_->inv().points << "\n";

  lastOppCoord_ = m.pos;
  turn_ = &other();
  return true;
}
