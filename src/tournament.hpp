/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/

#pragma once
#include "player.hpp"
#include <string>

class Tournament {
public:

  /* *********************************************************************
  Function Name: recordRoundOutcome
  Purpose: Increment roundsWon for the player who has more points
          at the end of a round.
  Parameters:
    p1, a Player (by reference) – first participant.
    p2, a Player (by reference) – second participant.
  Return Value: none
  Reference: None
  ********************************************************************* */
  void recordRoundOutcome(Player& p1, Player& p2);
  
  /* *********************************************************************
  Function Name: winnerName
  Purpose: Report the tournament winner’s name based on roundsWon totals.
  Parameters:
    p1, a Player (by const reference).
    p2, a Player (by const reference).
  Return Value: std::string – p1.name(), p2.name(), or "Draw" if tied.
  Reference: None
  ********************************************************************* */
  std::string winnerName(const Player& p1, const Player& p2) const;
  
  /* *********************************************************************
  Function Name: reset
  Purpose: Reset roundsWon counters to start a fresh tournament.
  Parameters:
    p1, a Player (by reference).
    p2, a Player (by reference).
  Return Value: none
  Reference: None
  ********************************************************************* */
  void reset(Player& p1, Player& p2);
};
