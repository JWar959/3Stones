/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 1
* Class:  CMPS 366
* Date:  2025-09-09
*********************************************************************/

#pragma once
#include "board.hpp"
#include "player.hpp"
#include "round.hpp"
#include <string>

class Serializer {
public:
  static bool save(const std::string& path, const Player& human, const Player& cpu,
                   const Round& round, const Board& board);
  static bool load(const std::string& path, Player& human, Player& cpu,
                   Round& round, Board& board); // Milestone 1: optional/minimal
};
