/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 1
* Class:  CMPS 366
* Date:  2025-09-09
*********************************************************************/

#pragma once
#include "player.hpp"

class Human : public Player {
public:
  Human(std::string name) : Player(std::move(name), PlayerKind::Human) {}
  Move chooseMove(const Board& board, const Coord& oppLast) override;
};
