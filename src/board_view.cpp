/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/

#include "board_view.hpp"
#include <iostream>
#include <iomanip>

void BoardView::print(const Board& b) const {
  b.print(); // delegate to Board's quick printer for now
}
