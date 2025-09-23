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

/* *********************************************************************
Function Name: initFromLoad
Purpose: Complete Round initialization after loading from a save:
         restore whose turn, last opponent coord, and player colors.
Parameters:
  nextIsHuman, a bool (by value) – who moves next.
  lastOpp, a Coord (by const reference) – last opponent move from file.
Return Value: none
Algorithm:
  1) Set turn_ based on nextIsHuman.
  2) Set lastOppCoord_ to lastOpp.
  3) Ensure black_/white_ pointers match Player inventories.
Reference: None
********************************************************************* */
void Round::initFromLoad(bool nextIsHuman, Coord lastOpp) {
  // Re-derive black_/white_ from inventories
  black_  = (human_->inv().myColor == Stone::B) ? human_ : computer_;
  white_  = (human_->inv().myColor == Stone::W) ? human_ : computer_;

  // Set next to move
  turn_ = nextIsHuman ? human_ : computer_;

  // Set the “opponent’s last move” anchor for row/col restriction
  lastOppCoord_ = lastOpp;
}

/* *********************************************************************
Function Name: Round
Purpose: Manage one round of 3-Stones: turn order, legality checks,
         scoring updates, serialization prompts, and round end.
Parameters:
  board, a Board (by reference) – the shared game board.
  human, a Player (by reference) – human participant.
  computer, a Player (by reference) – computer participant.
Return Value: none
Reference: None
********************************************************************* */
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

/* *********************************************************************
Function Name: anyAvailableOnRowOrCol
Purpose: Check the row/column constraint: whether row r or column c has
         any empty valid pocket.
Parameters:
  r, an int (by value) – row index.
  c, an int (by value) – column index.
Return Value: bool – true if the rule can be satisfied somewhere.
Reference: None
********************************************************************* */
bool Round::anyAvailableOnRowOrCol(int row, int col) const {
  for (int i=0;i<11;++i) {
    if (board_.isValidPocket(row,i) && board_.isEmpty(row,i)) return true;
    if (board_.isValidPocket(i,col) && board_.isEmpty(i,col)) return true;
  }
  return false;
}

/* *********************************************************************
Function Name: applyMove
Purpose: Validate and apply a move: enforce row/col rule, place stone,
         update inventories and scores, advance the turn, and print a
         user-friendly explanation required for the demo.
Parameters:
  m, a Move (by const reference) – the move to apply.
Return Value: bool – true if the move was applied; false if illegal.
Algorithm:
  1) Reject if pocket invalid or non-empty.
  2) If not first move: require same row or column as lastOppCoord_,
     unless both that row and column are full.
  3) Place stone; compute (myPts, oppPts) via Board::scoreFromPlacement.
  4) Update both players’ points and inventories.
  5) Build lastExplanation_ string describing color, pocket, and points.
  6) If serialization chosen, write file and end round; else switch turn.
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

  // Update the players inventories
  Inventory& inv = turn_->inv();
  if (m.played==Stone::B && inv.black>0) inv.black--;
  else if (m.played==Stone::W && inv.white>0) inv.white--;
  else if (m.played==Stone::C && inv.clear>0) inv.clear--;
  else return false;

  // Output the players updated inventories
  auto printInv = [&](const Player& P) {
      const auto& I = P.inv();   // adjust if your accessor differs
      std::cout << "Stones Remaining " << P.name()
                << ": B=" << I.black     
                << " W=" << I.white     
                << " C=" << I.clear   
                << "\n";
  };

  printInv(*human_);
  printInv(*computer_);

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
