/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/

#pragma once
#include "player.hpp"
#include "board.hpp"
#include <vector>

class Computer : public Player {
public:
  explicit Computer(const std::string& name) : Player(name, PlayerKind::Computer) {}

  Move chooseMove(Board& board, Coord lastOpp) override;
  
  // Recommendation for human based off of their CORRECT color
  Move recommendForHuman(Board& board,
                       Coord lastOpp,
                       Stone humanColor,
                       const Inventory& humanInv);


  // list all human plays for the help screen
  struct ScoredLine {
    Move m;
    int myPts{0};
    int oppBestPts{0};
    int utility{0};
    bool blocked{false};
  };
  std::vector<ScoredLine> listAllHumanCandidates(Board& board, Coord lastOpp,
                                                 Stone humanColor, const Inventory& humanInv);
private:
  struct Scored {
    Move m{};
    int myPts{0};
    int oppBestPts{0};
    int utility{0};
    // will show if this move blocks an immediate threat
    bool blocked{false};  
  };

  Scored evaluateBestFor(Board& board,
                         Coord lastOpp,
                         Stone actorColor,
                         const Inventory& actorInv,
                         Stone opponentColor);

  int opponentBestResponsePts(Board& board, Coord ourMovePos, Stone opponentColor);

  void enumLegalCells(Board& board, Coord lastOpp, std::vector<Coord>& out);
  void candidateStones(const Inventory& inv, std::vector<Stone>& out) const;

  bool isOpponentThreatAt(Board& board, const Coord& p, Stone opponentColor);
  static std::string rationaleText(const Scored& s);
};
