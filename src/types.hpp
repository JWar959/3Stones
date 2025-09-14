/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 1
* Class:  CMPS 366
* Date:  2025-09-09
*********************************************************************/

#pragma once
#include <string>

enum class Stone { Empty, B, W, C };

struct Coord { int r{0}; int c{0}; };

struct Pocket { bool valid{false}; Stone s{Stone::Empty}; };

// char version
inline char stoneToChar(Stone s) {
  switch (s) {
    case Stone::B: return 'B';
    case Stone::W: return 'W';
    case Stone::C: return 'C';
    default:       return 'O';
  }
}

// string wrapper
inline std::string stoneToString(Stone s) {
  return std::string(1, stoneToChar(s));
}
