/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/

#pragma once
#include <string>

enum class Stone { Empty, B, W, C };

struct Coord { int r{0}; int c{0}; };

struct Pocket { bool valid{false}; Stone s{Stone::Empty}; };

/* *********************************************************************
Function Name: stoneToChar
Purpose: Convert a Stone enum to a single character for display/serialization.
Parameters:
  s, a Stone (by value) – stone to convert.
Return Value: char – 'B','W','C', or 'O' for empty.
Reference: None
********************************************************************* */
inline char stoneToChar(Stone s) {
  switch (s) {
    case Stone::B: return 'B';
    case Stone::W: return 'W';
    case Stone::C: return 'C';
    default:       return 'O';
  }
}

/* *********************************************************************
Function Name: stoneToString
Purpose: Convert a Stone enum to a single-character std::string for UI text.
Parameters:
  s, a Stone (by value) – stone to convert.
Return Value: std::string – "B","W","C", or "O".
Reference: None
********************************************************************* */
inline std::string stoneToString(Stone s) {
  return std::string(1, stoneToChar(s));
}
