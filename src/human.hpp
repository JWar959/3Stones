/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Final
* Class:  CMPS 366
* Date:  9-23-2025
*********************************************************************/

#pragma once
#include "player.hpp"
#include <functional>
#include <string>

class Human : public Player {
public:
  using HelpFn = std::function<Move(Board&, Coord, Stone, const Inventory&)>;
  explicit Human(const std::string& name) : Player(name, PlayerKind::Human) {}
  
  /* *********************************************************************
  Function Name: inputName
  Purpose: Read and return the user’s name from standard input.
  Parameters: none
  Return Value: std::string – non-empty trimmed name.
  Algorithm:
    1) Prompt until a non-empty line is entered.
    2) Return the trimmed string.
  Reference: None
  ********************************************************************* */
  static std::string inputName();

  /* *********************************************************************
  Function Name: chooseMove
  Purpose: Interactively collect a legal move from the human given the
          row/column constraint from the opponent’s last move.
  Parameters:
    board, a Board (by reference) – current board to validate targets.
    oppLast, a Coord (by value) – last pocket played by the opponent, or (-1,-1) at start.
  Return Value: Move – user’s chosen move (position, stone, rationale empty).
  Algorithm:
    1) Ask for stone (B/W/C) or help (H); if H, call help callback and re-prompt.
    2) Read row and column (1..11), convert to 0-based.
    3) Validate pocket legality: in shape, empty, and satisfies row/column rule
      unless both the row and column are full.
    4) Mark move valid and return.
  Reference: None
  ********************************************************************* */
  Move chooseMove(Board& board, Coord oppLast) override;

  /* *********************************************************************
  Function Name: setHelpCallback
  Purpose: Install the computer’s help callback so Human can request advice.
  Parameters:
    cb, a HelpFn (by value) – function object to compute a recommendation.
  Return Value: none
  Reference: None
  ********************************************************************* */
  void setHelpCallback(HelpFn cb) { help_ = std::move(cb); }
  
  /* *********************************************************************
  Function Name: setHumanName
  Purpose: Mutator – set the human player’s display name.
  Parameters:
    name, a std::string (by const reference) – name to assign.
  Return Value: none
  Reference: None
  ********************************************************************* */
  void setHumanName(const std::string& name);
  
  /* *********************************************************************
  Function Name: humanName
  Purpose: Selector – get the human player’s display name.
  Parameters: none
  Return Value: const std::string& – current name.
  Reference: None
  ********************************************************************* */
  const std::string& humanName() const { return humanName_; }
  
  /* *********************************************************************
  Function Name: getHumanName
  Purpose: Return a copy of the human player’s display name.
  Parameters: none
  Return Value: std::string – current name.
  Reference: None
  ********************************************************************* */
  std::string getHumanName();

private:
  HelpFn help_;
  std::string humanName_;
};
