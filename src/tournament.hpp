/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 1
* Class:  CMPS 366
* Date:  2025-09-09
*********************************************************************/

#pragma once
#include "player.hpp"
#include <string>

class Tournament {
public:
  void recordRoundOutcome(Player& p1, Player& p2);
  std::string winnerName(const Player& p1, const Player& p2) const;
  void reset(Player& p1, Player& p2);
};
