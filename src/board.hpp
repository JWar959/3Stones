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
  
/* *********************************************************************
Function Name: Board
Purpose: Construct an empty 3-Stones board with the assignment’s diamond
         shape and the center void.
Parameters: none
Return Value: none
Algorithm:
  1) Call initShape() to mark valid pockets in each row and the center void.
Reference: None
********************************************************************* */
  Board();

  /* *********************************************************************
  Function Name: inBounds
  Purpose: Tell whether (r,c) is inside the 11×11 array bounds.
  Parameters:
    r, an integer (by value) – row index (0..10).
    c, an integer (by value) – column index (0..10).
  Return Value: bool – true if 0 ≤ r,c ≤ 10, false otherwise.
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
  

  /* *********************************************************************
  Function Name: isEmpty
  Purpose: Tell whether (r,c) is a playable empty pocket.
  Parameters:
    r, an integer (by value) – row index.
    c, an integer (by value) – column index.
  Return Value: bool – true if valid pocket with Stone::Empty.
  Reference: None
  ********************************************************************* */
  bool isEmpty(int r, int c) const;

  /* *********************************************************************
  Function Name: place
  Purpose: Place a stone into an empty, valid pocket.
  Parameters:
    r, an integer (by value) – row index.
    c, an integer (by value) – column index.
    s, a Stone (by value) – stone to place.
  Return Value: bool – true and places the stone if legal; false otherwise.
  Reference: None
  ********************************************************************* */
  bool place(int r, int c, Stone s);
  
  const Pocket& at(int r, int c) const;

  /* *********************************************************************
  Function Name: hasEmptyInRow
  Purpose: Check if the given row has any empty valid pockets (for row/col rule).
  Parameters:
    r, an integer (by value) – row index.
  Return Value: bool – true if at least one empty valid pocket exists in row r.
  Reference: None
  ********************************************************************* */
  bool hasEmptyInRow(int r) const;

  /* *********************************************************************
  Function Name: hasEmptyInCol
  Purpose: Check if the given column has any empty valid pockets (for row/col rule).
  Parameters:
    c, an integer (by value) – column index.
  Return Value: bool – true if at least one empty valid pocket exists in col c.
  Reference: None
  ********************************************************************* */
  bool hasEmptyInCol(int c) const;
  Pocket& at(int r, int c);

  /* *********************************************************************
  Function Name: scoreFromPlacement
  Purpose: Compute the points awarded to the player who just played at pocket p,
          and any points awarded to the opponent due to clear stones.
  Parameters:
    p, a Coord (by value) – the pocket that was just played.
    played, a Stone (by value) – the stone that was played (B/W/C).
    myColor, a Stone (by value) – the mover’s color (B or W).
  Return Value: std::pair<int,int> – (myPointsGained, opponentPointsGained).
  Algorithm:
    1) For each of 4 directions (vertical, horizontal, two diagonals), slide
      3-pocket windows that include p.
    2) For each window with no empty pockets:
        a) If all three are mover-or-clear (not all clear), mover gets +1.
        b) If the triple contains clear and is opponent-or-clear (not all clear),
            opponent gets +1 (clear lets both sides score in the same window).
    3) Sum mover and opponent points across all windows and return.
  Reference: None
  ********************************************************************* */
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
  
  /* *********************************************************************
  Function Name: setStone
  Purpose: Force-set a stone in a valid pocket (used by serializer/undo).
  Parameters:
    r, an integer (by value) – row index.
    c, an integer (by value) – column index.
    s, a Stone (by value) – stone to set.
  Return Value: bool – true if pocket was valid and stone was set; false otherwise.
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

  /* *********************************************************************
  Function Name: initShape
  Purpose: Mark which pockets on the 11×11 grid are valid to play per spec
          (rows with counts 3,5,7,9,11,10,11,9,7,5,3 and a center void).
  Parameters: none
  Return Value: none
  Algorithm:
    1) For each row, compute starting column and mark 'cnt' consecutive pockets valid.
    2) Mark the center void (row 5, col 5) invalid.
  Reference: None
  ********************************************************************* */
  void initShape();
};
