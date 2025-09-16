/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/

#pragma once
#include "types.hpp"
#include "move.hpp"
#include "board.hpp"
#include <string>

enum class PlayerKind { Human, Computer };

struct Inventory {
  int black{15};
  int white{15};
  int clear{6};
  int points{0};
  int roundsWon{0};
  Stone myColor{Stone::B}; // assigned after coin toss & color selection
};

class Player {
public:
  Player(std::string name, PlayerKind kind) : name_(std::move(name)), kind_(kind) {}
  virtual ~Player() = default;
  virtual Move chooseMove(Board& board, Coord lastOpp) = 0;

  const std::string& name() const { return name_; }
  PlayerKind kind() const { return kind_; }
  const Inventory& inv() const { return inv_; }
  Inventory& inv() { return inv_; }

protected:
  std::string name_;
  PlayerKind kind_;
  Inventory inv_;
};
