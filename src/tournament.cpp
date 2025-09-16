/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/
#include "tournament.hpp"

void Tournament::recordRoundOutcome(Player& p1, Player& p2) {
  if (p1.inv().points > p2.inv().points) p1.inv().roundsWon++;
  else if (p2.inv().points > p1.inv().points) p2.inv().roundsWon++;
}

std::string Tournament::winnerName(const Player& p1, const Player& p2) const {
  if (p1.inv().roundsWon > p2.inv().roundsWon) return p1.name();
  if (p2.inv().roundsWon > p1.inv().roundsWon) return p2.name();
  return "Draw";
}

void Tournament::reset(Player& p1, Player& p2) {
  p1.inv().roundsWon = 0; p2.inv().roundsWon = 0;
}
