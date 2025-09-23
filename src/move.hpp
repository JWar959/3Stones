/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/

#pragma once
#include "types.hpp"

/* *********************************************************************
Function Name: (struct) Move
Purpose: Represent a single play in the game: pocket, stone, validity, and
         optional rationale text for help/UX.
Parameters: N/A (aggregate type).
Return Value: N/A
Reference: None
********************************************************************* */
struct Move {
  Coord pos;
  Stone played{Stone::Empty};
  bool isValid{false};
  // Explaination for UI/help
  std::string rationale;
};
