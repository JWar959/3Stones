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

class Serializer {
public:
  static bool save(const Board& b,
                   const Player& human, const Player& computer,
                   bool nextIsHuman, Coord lastOpp,
                   const std::string& filename);

  static bool load(Board& b,
                   Player& human, Player& computer,
                   bool& nextIsHuman, Coord& lastOpp,
                   const std::string& filename);
};
