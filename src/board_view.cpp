/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Final
* Class:  CMPS 366
* Date:  9-23-2025
*********************************************************************/

#include "board_view.hpp"
#include <iostream>
#include <iomanip>

// File: board_view.cpp
// Signature: BoardView::print(const Board& b)
/* *********************************************************************
  Function Name: print
  Purpose: Render the current board for the user interface.
  Parameters:
    b, a Board (by const reference) – the board to display.
  Return Value: none
  Algorithm:
    1) Delegate to Board::print() for the formatted grid.
  Reference: None
********************************************************************* */
void BoardView::print(const Board& b) const {
  // delegated to Board's quick printer for now
  b.print(); 
}
