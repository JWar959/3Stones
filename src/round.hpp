/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/

#pragma once
#include "board.hpp"
#include "player.hpp"
#include <string>

class Round {
public:

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
  Round(Board& board, Player& human, Player& computer);

  void decideFirstPlayerWithCoinCall();
  bool isOver() const;
  Player& current();
  Player& other();

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
  bool applyMove(const Move& m);
  bool currentIsHuman() const { return turn_ == human_; }

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
  void initFromLoad(bool nextIsHuman, Coord lastOpp);

  std::string lastMoveExplanation() const { return lastExplanation_; }
  Coord lastOpponentCoord() const { return lastOppCoord_; }

private:
  Board& board_;
  Player* human_;
  Player* computer_;
  // player holding black stones
  Player* black_;
  // player holding white stones   
  Player* white_;
  // whose turn now   
  Player* turn_;    

  Coord lastOppCoord_{-1,-1};
  std::string lastExplanation_;

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
  bool anyAvailableOnRowOrCol(int row, int col) const;
};
