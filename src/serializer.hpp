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

  /* *********************************************************************
  Function Name: Serializer::save
  Purpose: Persist the full game state to a text file in the required layout
          so the grader can resume from scenarios.
  Parameters:
    b, a Board (by const reference) – board to serialize.
    human, a Player (by const reference) – human state.
    computer, a Player (by const reference) – computer state.
    nextIsHuman, a bool (by value) – who moves next.
    lastOpp, a Coord (by const reference) – last opponent move or (-1,-1).
    filename, a std::string (by const reference) – path to write.
  Return Value: bool – true on success, false on I/O or format error.
  Algorithm:
    1) Open file; write “Human Player: …”, “Computer Player: …”, “Next Player: …”.
    2) Write “Board:” followed by 11 rows in the diamond shape with O/B/W/C.
    3) Close file and return status.
  Reference: None
  ********************************************************************* */
  static bool save(const Board& b,
                   const Player& human, const Player& computer,
                   bool nextIsHuman, Coord lastOpp,
                   const std::string& filename);

  /* *********************************************************************
  Function Name: Serializer::load
  Purpose: Recreate a game state from a text file in the assignment’s format.
  Parameters:
    b, a Board (by reference) – board to fill.
    human, a Player (by reference) – human to fill.
    computer, a Player (by reference) – computer to fill.
    nextIsHuman, a bool (by reference) – receives next mover.
    lastOpp, a Coord (by reference) – receives last opponent move.
    filename, a std::string (by const reference) – path to read.
  Return Value: bool – true if parsed and state populated; false otherwise.
  Algorithm:
    1) Read and parse the labeled sections in order.
    2) Parse inventories, next player, and last move coordinates.
    3) Rebuild the board grid rows; validate characters and shape.
  Reference: None
  ********************************************************************* */
  static bool load(Board& b,
                   Player& human, Player& computer,
                   bool& nextIsHuman, Coord& lastOpp,
                   const std::string& filename);
};
