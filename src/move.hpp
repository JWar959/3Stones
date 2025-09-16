/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 1
* Class:  CMPS 366
* Date:  2025-09-09
*********************************************************************/

#pragma once
#include "types.hpp"

struct Move {
  Coord pos;
  Stone played{Stone::Empty};
  bool isValid{false};
  // Explaination for UI/help
  std::string rationale;
};
