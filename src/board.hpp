/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-16
*********************************************************************/

#pragma once
#include "types.hpp"
#include <array>
#include <utility>
#include <vector>

class Board {
public:
  
  // File: board.hpp
  // Signature: Board()
  /* *********************************************************************
  Function Name: Board
  Purpose: Implements a cohesive step of the 3 Stones gameplay in this module.
  Parameters:
              None
  Return Value: void. Performs side effects only.
  Algorithm:
              1) Perform the documented steps to achieve the stated purpose.
              2) Handle edge-cases per rules.
              3) Return the result or mutate state.
  Reference: None
  ********************************************************************* */
  Board();

  // File: board.hpp
  // Signature: inBounds(int r, int c)
  /* *********************************************************************
  Function Name: inBounds
  Purpose: Implements a cohesive step of the 3 Stones gameplay in this module.
  Parameters:
              r, int, passed by value. Purpose: input parameter
              c, int, passed by value. Purpose: input parameter
  Return Value: bool. True on success; false on failure.
  Algorithm:
              1) Perform the documented steps to achieve the stated purpose.
              2) Handle edge-cases per rules.
              3) Return the result or mutate state.
  Reference: None
  ********************************************************************* */
  
  bool inBounds(int r, int c) const;

  // File: board.hpp
  // Signature: isValidPocket(int r, int c)
  /* *********************************************************************
  Function Name: isValidPocket
  Purpose: Check whether a (row,col) pocket exists within the octagon board shape.
  Parameters:
              r, int, passed by value. Purpose: input parameter
              c, int, passed by value. Purpose: input parameter
  Return Value: bool. True if the stated condition holds; false otherwise.
  Algorithm:
              1) Check inputs.
              2) Evaluate the condition in constant time.
              3) Return the boolean result.
  Reference: None
  ********************************************************************* */
  bool isValidPocket(int r, int c) const;
  
  // File: board.hpp
  // Signature: isEmpty(int r, int c)
  /* *********************************************************************
  Function Name: isEmpty
  Purpose: Check whether a pocket is valid and currently empty.
  Parameters:
              r, int, passed by value. Purpose: input parameter
              c, int, passed by value. Purpose: input parameter
  Return Value: bool. True if the stated condition holds; false otherwise.
  Algorithm:
              1) Check inputs.
              2) Evaluate the condition in constant time.
              3) Return the boolean result.
  Reference: None
  ********************************************************************* */
  bool isEmpty(int r, int c) const;

  // File: board.hpp
  // Signature: place(int r, int c, Stone s)
  /* *********************************************************************
  Function Name: place
  Purpose: Attempt to place a stone in the specified pocket if it is empty.
  Parameters:
              r, int, passed by value. Purpose: input parameter
              c, int, passed by value. Purpose: input parameter
              s, Stone, passed by value. Purpose: input parameter
  Return Value: bool. True on success; false on failure.
  Algorithm:
              1) Perform the documented steps to achieve the stated purpose.
              2) Handle edge-cases per rules.
              3) Return the result or mutate state.
  Reference: None
  ********************************************************************* */
  bool place(int r, int c, Stone s);
  
  const Pocket& at(int r, int c) const;
  // File: board.hpp
  // Signature: hasEmptyInRow(int r)
  /* *********************************************************************
  Function Name: hasEmptyInRow
  Purpose: Determine if any valid pocket in the given row is empty (enables row move rule).
  Parameters:
              r, int, passed by value. Purpose: input parameter
  Return Value: bool. True if the stated condition holds; false otherwise.
  Algorithm:
              1) Check inputs.
              2) Evaluate the condition in constant time.
              3) Return the boolean result.
  Reference: None
  ********************************************************************* */
  
  bool hasEmptyInRow(int r) const;

  // File: board.hpp
  // Signature: hasEmptyInCol(int c)
  /* *********************************************************************
  Function Name: hasEmptyInCol
  Purpose: Determine if any valid pocket in the given column is empty (enables column move rule).
  Parameters:
              c, int, passed by value. Purpose: input parameter
  Return Value: bool. True if the stated condition holds; false otherwise.
  Algorithm:
              1) Check inputs.
              2) Evaluate the condition in constant time.
              3) Return the boolean result.
  Reference: None
  ********************************************************************* */
  bool hasEmptyInCol(int c) const;
  Pocket& at(int r, int c);

  // Scoring helper 
  std::pair<int,int> scoreFromPlacement(const Coord& p, Stone played, Stone myColor) const;

  // File: board.hpp
  // Signature: debugPrintScoringWindows(const Coord& p, Stone myColor)
  /* *********************************************************************
  Function Name: debugPrintScoringWindows
  Purpose: Print the scoring windows that include a given pocket for debugging rationale.
  Parameters:
              p, const Coord&, passed by reference/pointer. Purpose: board coordinate
              myColor, Stone, passed by value. Purpose: input parameter
  Return Value: void. Performs side effects only.
  Algorithm:
              1) Format human-readable output.
              2) Print labels and values clearly aligned.
              3) Return to caller.
  Reference: None
  ********************************************************************* */
  void debugPrintScoringWindows(const Coord& p, Stone myColor) const;

  // File: board.hpp
  // Signature: print()
  /* *********************************************************************
  Function Name: print
  Purpose: Render the board with row/column headers and stones.
  Parameters:
              None
  Return Value: void. Performs side effects only.
  Algorithm:
              1) Format human-readable output.
              2) Print labels and values clearly aligned.
              3) Return to caller.
  Reference: None
  ********************************************************************* */
  void print() const; 
  // force set (valid pocket required), returns old->new validity
  
  // File: board.hpp
  // Signature: setStone(int r, int c, Stone s)
  /* *********************************************************************
  Function Name: setStone
  Purpose: Force set a pocket's stone (valid pocket required); return whether validity status changed.
  Parameters:
              r, int, passed by value. Purpose: input parameter
              c, int, passed by value. Purpose: input parameter
              s, Stone, passed by value. Purpose: input parameter
  Return Value: bool. True on success; false on failure.
  Algorithm:
              1) Validate inputs.
              2) Assign the new value to the field.
              3) Return status if applicable.
  Reference: None
  ********************************************************************* */
  bool setStone(int r, int c, Stone s);
  
  // File: board.hpp
  // Signature: getStone(int r, int c)
  /* *********************************************************************
  Function Name: getStone
  Purpose: Read the stone at a pocket without modifying the board.
  Parameters:
              r, int, passed by value. Purpose: input parameter
              c, int, passed by value. Purpose: input parameter
  Return Value: Stone. See function description for meaning.
  Algorithm:
              1) Return the underlying field without modification.
  Reference: None
  ********************************************************************* */
  Stone getStone(int r, int c) const;
  
  // File: board.hpp
  // Signature: restoreStone(int r, int c, Stone prev)
  /* *********************************************************************
  Function Name: restoreStone
  Purpose: Restore the previous stone at a pocket (undo helper).
  Parameters:
              r, int, passed by value. Purpose: input parameter
              c, int, passed by value. Purpose: input parameter
              prev, Stone, passed by value. Purpose: input parameter
  Return Value: void. Performs side effects only.
  Algorithm:
              1) Perform the documented steps to achieve the stated purpose.
              2) Handle edge-cases per rules.
              3) Return the result or mutate state.
  Reference: None
  ********************************************************************* */
  void  restoreStone(int r, int c, Stone prev);

  private:
  std::array<std::array<Pocket,11>,11> grid_;

  // File: board.hpp
  // Signature: initShape()
  /* *********************************************************************
  Function Name: initShape
  Purpose: Initialize the 11×11 octagon board shape, marking valid/invalid pockets.
  Parameters:
              None
  Return Value: void. Performs side effects only.
  Algorithm:
              1) Initialize 11×11 grid of pockets to invalid.
              2) Mark octagon-shaped valid pockets per row constraints and set stones to empty.
              3) Ensure center void is invalid and all indices are in range.
  Reference: None
  ********************************************************************* */
  void initShape();
};
