/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Final
* Class:  CMPS 366
* Date:  9-23-2025
*********************************************************************/

#pragma once
#include "board.hpp"

class BoardView {
public:

  // File: board_view.hpp
  // Signature: print(const Board& b)
  /* *********************************************************************
  Function Name: print
  Purpose: Render the board with row/column headers and stones.
  Parameters:
              b, const Board&, passed by reference/pointer. Purpose: input parameter
  Return Value: void. Performs side effects only.
  Algorithm:
              1) Format human-readable output.
              2) Print labels and values clearly aligned.
              3) Return to caller.
  Reference: None
  ********************************************************************* */
  void print(const Board& b) const;
};
