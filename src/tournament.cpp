/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Final
* Class:  CMPS 366
* Date:  9-23-2025
*********************************************************************/
#include "tournament.hpp"
#include <iostream>

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
void Tournament::recordRoundOutcome(Player& p1, Player& p2) {
  if (p1.inv().points > p2.inv().points){
    p1.inv().roundsWon++;
  } 
  else if (p2.inv().points > p1.inv().points) {
    p2.inv().roundsWon++;
  }
  else{
    std::cout << "\nNo round winner awarded.\n";
  }
}

/* *********************************************************************
Function Name: winnerName
Purpose: Report the tournament winner’s name based on roundsWon totals.
Parameters:
  p1, a Player (by const reference).
  p2, a Player (by const reference).
Return Value: std::string – p1.name(), p2.name(), or "Draw" if tied.
Reference: None
********************************************************************* */
std::string Tournament::winnerName(const Player& p1, const Player& p2) const {
  if (p1.inv().roundsWon > p2.inv().roundsWon) return p1.name();
  if (p2.inv().roundsWon > p1.inv().roundsWon) return p2.name();
  return "Draw";
}

/* *********************************************************************
Function Name: reset
Purpose: Reset roundsWon counters to start a fresh tournament.
Parameters:
  p1, a Player (by reference).
  p2, a Player (by reference).
Return Value: none
Reference: None
********************************************************************* */
void Tournament::reset(Player& p1, Player& p2) {
  p1.inv().roundsWon = 0; p2.inv().roundsWon = 0;
}
