/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-16
*********************************************************************/

#include "board.hpp"
#include <iostream>
#include <iomanip>
#include "types.hpp"


Board::Board() { initShape(); }

// File: board.cpp
// Signature: Board::initShape()
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
void Board::initShape() {
  // Row pocket counts: 3,5,7,9,11,10,11,9,7,5,3
  int counts[11] = {3,5,7,9,11,10,11,9,7,5,3};
  for (int r=0; r<11; ++r) {
    for (int c=0; c<11; ++c) {
      grid_[r][c].valid = false;
      grid_[r][c].s = Stone::Empty;
    }
    
      if (r == 5) {
      // Problem solution row 6 should be fully populated except the center void
      // traversing through columns 0 through 10
      for (int c=0; c<11; ++c) grid_[r][c].valid = true;
        // center void at (6,6) 
        grid_[r][5].valid = false;                          
        continue;
      }

    int cnt = counts[r];
    int start = (11 - cnt) / 2;
    for (int c=start; c<start+cnt; ++c) {
      grid_[r][c].valid = true;
    }
  }
}

// File: board.cpp
// Signature: Board::getStone(int r, int c)
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
Stone Board::getStone(int r, int c) const {
    return at(r,c).s;
}

// File: board.cpp
// Signature: Board::setStone(int r, int c, Stone s)
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
bool Board::setStone(int r, int c, Stone s) {
    if (!isValidPocket(r,c)) return false;
    grid_[r][c].s = s;
    return true;
}

// File: board.cpp
// Signature: Board::restoreStone(int r, int c, Stone prev)
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
void Board::restoreStone(int r, int c, Stone prev) {
    grid_[r][c].s = prev;
}

// File: board.cpp
// Signature: Board::inBounds(int r, int c)
/* *********************************************************************
Function Name: inBounds
Purpose: Implements a cohesive step of checking if a cell is within the 
bounadries of the board.
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
bool Board::inBounds(int r, int c) const {
  return r>=0 && r<11 && c>=0 && c<11;
}

// File: board.cpp
// Signature: Board::isValidPocket(int r, int c)
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
bool Board::isValidPocket(int r, int c) const {
  return inBounds(r,c) && grid_[r][c].valid;
}

// File: board.cpp
// Signature: Board::isEmpty(int r, int c)
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
bool Board::isEmpty(int r, int c) const {
  return isValidPocket(r,c) && grid_[r][c].s == Stone::Empty;
}

// File: board.cpp
// Signature: Board::place(int r, int c, Stone s)
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
bool Board::place(int r, int c, Stone s) {
  if (!isEmpty(r,c)) return false;
  grid_[r][c].s = s;
  return true;
}

// File: board.cpp
// Signature: Board::hasEmptyInRow(int r)
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
bool Board::hasEmptyInRow(int r) const {
    for (int c = 0; c < (int)grid_[r].size(); ++c)
        if (isValidPocket(r,c) && grid_[r][c].s == Stone::Empty) return true;
    return false;
}

// File: board.cpp
// Signature: Board::hasEmptyInCol(int c)
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
bool Board::hasEmptyInCol(int c) const {
    for (int r = 0; r < (int)grid_.size(); ++r)
        if (isValidPocket(r,c) && grid_[r][c].s == Stone::Empty) return true;
    return false;
}

// File: board.cpp
// Signature: Board::debugPrintScoringWindows(const Coord& p, Stone myColor)
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
void Board::debugPrintScoringWindows(const Coord& p, Stone myColor) const {
  Stone oppColor = (myColor == Stone::B ? Stone::W : Stone::B);

  auto allIn = [](Stone a, Stone b, Stone c, Stone color){
    return ((a==color || a==Stone::C) &&
            (b==color || b==Stone::C) &&
            (c==color || c==Stone::C));
  };

  const int dr[4] = { 1, 0, 1, -1};
  const int dc[4] = { 0, 1, 1,  1};

  for (int d=0; d<4; ++d) {
    for (int start=-2; start<=0; ++start) {
      Coord a{ p.r + (start    )*dr[d], p.c + (start    )*dc[d] };
      Coord b{ p.r + (start + 1)*dr[d], p.c + (start + 1)*dc[d] };
      Coord c{ p.r + (start + 2)*dr[d], p.c + (start + 2)*dc[d] };

      if (!(isValidPocket(a.r,a.c) && isValidPocket(b.r,b.c) && isValidPocket(c.r,c.c))) continue;

      Stone s1 = at(a.r,a.c).s;
      Stone s2 = at(b.r,b.c).s;
      Stone s3 = at(c.r,c.c).s;

      if (s1 == Stone::Empty || s2 == Stone::Empty || s3 == Stone::Empty) continue;

      bool allClear      = (s1==Stone::C && s2==Stone::C && s3==Stone::C);
      bool containsClear = (s1==Stone::C || s2==Stone::C || s3==Stone::C);

      int addMe  = (!allClear && allIn(s1,s2,s3,myColor))  ? 1 : 0;
      int addOpp = (!allClear && containsClear && allIn(s1,s2,s3,oppColor)) ? 1 : 0;

      if (addMe || addOpp) {
        std::cout << "[score] placed=(" << (p.r+1) << "," << (p.c+1) << ")  "
                  << "win=(" << (a.r+1) << "," << (a.c+1) << ")-("
                              << (b.r+1) << "," << (b.c+1) << ")-("
                              << (c.r+1) << "," << (c.c+1) << ")  "
                  << "stones=" << stoneToChar(s1) << stoneToChar(s2) << stoneToChar(s3)
                  << "  me+=" << addMe << "  opp+=" << addOpp << "\n";
      }
    }
  }
}

// File: board.cpp
// Signature: Board::at(int r, int c)
/* *********************************************************************
Function Name: at
Purpose: Access a pocket by coordinates.
Parameters:
            r, int, passed by value. Purpose: input parameter
            c, int, passed by value. Purpose: input parameter
Return Value: Pocket&. Reference to internal object; valid while the parent object exists.
Algorithm:
            1) Perform the documented steps to achieve the stated purpose.
            2) Handle edge-cases per rules.
            3) Return the result or mutate state.
Reference: None
********************************************************************* */
const Pocket& Board::at(int r, int c) const { return grid_[r][c]; }
Pocket& Board::at(int r, int c) { return grid_[r][c]; }

// File: board.cpp
// Signature: Board::print()
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

void Board::print() const {
  std::cout << "\n    ";
  for (int c=1; c<=11; ++c) std::cout << std::setw(2) << c << " ";
  std::cout << "\n";
  for (int r=0; r<11; ++r) {
    std::cout << std::setw(2) << (r+1) << "  ";
    for (int c=0; c<11; ++c) {
      if (!at(r,c).valid) { std::cout << "   "; continue; }
      std::cout << std::setw(2) << stoneToChar(at(r,c).s) << " ";
    }
    std::cout << "\n";
  }
}

std::pair<int,int> Board::scoreFromPlacement(const Coord& p, Stone played, Stone myColor) const {
  // scoring depends on the actual stones on the board, not the chosen color directly
  (void)played; 

  auto oppColor = (myColor == Stone::B ? Stone::W : Stone::B);

  // Helper to check a 3-cell window and return points for (me, opp)
  auto scoreTriple = [&](Coord a, Coord b, Coord c) -> std::pair<int,int> {
    // Ensure pockets are valid
    if (!(isValidPocket(a.r,a.c) && isValidPocket(b.r,b.c) && isValidPocket(c.r,c.c)))
        return {0,0};

    // Cache stone values to avoid repeated at() calls
    Stone s1 = at(a.r,a.c).s;
    Stone s2 = at(b.r,b.c).s;
    Stone s3 = at(c.r,c.c).s;

    // Any Empty -> no score
    if (s1 == Stone::Empty || s2 == Stone::Empty || s3 == Stone::Empty)
        return {0,0};

    auto allIn = [&](Stone color){
      return ((s1==color || s1==Stone::C) &&
              (s2==color || s2==Stone::C) &&
              (s3==color || s3==Stone::C));
    };

    bool allClear = (s1==Stone::C && s2==Stone::C && s3==Stone::C);
    bool containsClear = (s1==Stone::C || s2==Stone::C || s3==Stone::C);

    // mover scores on mover-or-clear (not all clears)
    int me  = (!allClear && allIn(myColor))  ? 1 : 0;
    
    // opponent scores this only if a clear is involved
    int opp = (!allClear && containsClear && allIn(oppColor)) ? 1 : 0;
    
    return {me, opp};
  };

  // We only need 4 base directions; we’ll slide 3 windows that INCLUDE p:
  // [-2,-1,0], [-1,0,1], [0,1,2] along each axis.
  const int dr[4] = { 1, 0, 1, -1};
  const int dc[4] = { 0, 1, 1,  1};

  int mePts = 0, oppPts = 0;

  for (int d=0; d<4; ++d) {
    for (int start=-2; start<=0; ++start) {
      Coord a{ p.r + (start    )*dr[d], p.c + (start    )*dc[d] };
      Coord b{ p.r + (start + 1)*dr[d], p.c + (start + 1)*dc[d] };
      Coord c{ p.r + (start + 2)*dr[d], p.c + (start + 2)*dc[d] };

      // Ensure the window actually includes the placed cell p (it always will by construction),
      // and that the midpoints are within the board bounds; validity of pockets is checked inside.
      auto [me, opp] = scoreTriple(a,b,c);

      mePts  += me;
      oppPts += opp;
    }
  }
  return {mePts, oppPts};
}
