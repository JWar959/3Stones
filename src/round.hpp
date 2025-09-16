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
  Round(Board& board, Player& human, Player& computer);

  void decideFirstPlayerWithCoinCall();
  bool isOver() const;
  Player& current();
  Player& other();
  bool applyMove(const Move& m);
  bool currentIsHuman() const { return turn_ == human_; }

  // Helper function to load previous game
  void initFromLoad(bool nextIsHuman, Coord lastOpp) {
    lastOppCoord_ = lastOpp;
    turn_ = nextIsHuman ? human_ : computer_;
  }

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

  bool anyAvailableOnRowOrCol(int row, int col) const;
};
