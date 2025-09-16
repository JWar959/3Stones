/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/

#pragma once
#include "player.hpp"
#include <functional>

class Human : public Player {
public:
  using HelpFn = std::function<Move(Board&, Coord, Stone, const Inventory&)>;
  explicit Human(const std::string& name) : Player(name, PlayerKind::Human) {}

  Move chooseMove(Board& board, Coord oppLast) override;

  void setHelpCallback(HelpFn cb) { help_ = std::move(cb); }

private:
  HelpFn help_;
};
