/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 1
* Class:  CMPS 366 
* Date:  2025-09-09
*********************************************************************/

#include "board.hpp"
#include <iostream>
#include <iomanip>

Board::Board() { initShape(); }

void Board::initShape() {
  // Row pocket counts: 3,5,7,9,11,10,11,9,7,5,3 ; with a center void at row 5 (0-indexed)
  int counts[11] = {3,5,7,9,11,10,11,9,7,5,3};
  for (int r=0; r<11; ++r) {
    for (int c=0; c<11; ++c) {
      grid_[r][c].valid = false;
      grid_[r][c].s = Stone::Empty;
    }
    int cnt = counts[r];
    int start = (11 - cnt) / 2;
    for (int c=start; c<start+cnt; ++c) {
      grid_[r][c].valid = true;
    }
    if (r == 5) grid_[r][5].valid = false; // center void
  }
}

bool Board::inBounds(int r, int c) const {
  return r>=0 && r<11 && c>=0 && c<11;
}

bool Board::isValidPocket(int r, int c) const {
  return inBounds(r,c) && grid_[r][c].valid;
}

bool Board::isEmpty(int r, int c) const {
  return isValidPocket(r,c) && grid_[r][c].s == Stone::Empty;
}

bool Board::place(int r, int c, Stone s) {
  if (!isEmpty(r,c)) return false;
  grid_[r][c].s = s;
  return true;
}

bool Board::hasEmptyInRow(int r) const {
    for (int c = 0; c < (int)grid_[r].size(); ++c)
        if (isValidPocket(r,c) && grid_[r][c].s == Stone::Empty) return true;
    return false;
}

bool Board::hasEmptyInCol(int c) const {
    for (int r = 0; r < (int)grid_.size(); ++r)
        if (isValidPocket(r,c) && grid_[r][c].s == Stone::Empty) return true;
    return false;
}

const Pocket& Board::at(int r, int c) const { return grid_[r][c]; }
Pocket& Board::at(int r, int c) { return grid_[r][c]; }

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
  (void)played; // scoring depends on the actual stones on the board, not the chosen color directly

  auto oppColor = (myColor == Stone::B ? Stone::W : Stone::B);

  // Helper to check a 3-cell window and return points for (me, opp)
  auto scoreTriple = [&](Coord a, Coord b, Coord c) -> std::pair<int,int> {
    // All three must be valid pockets and not Empty
    auto ok = [&](const Coord& q){
      return isValidPocket(q.r,q.c) && at(q.r,q.c).s != Stone::Empty;
    };
    if (!(ok(a) && ok(b) && ok(c))) return {0,0};

    Stone s1 = at(a.r,a.c).s;
    Stone s2 = at(b.r,b.c).s;
    Stone s3 = at(c.r,c.c).s;

    auto allIn = [&](Stone color){
      // “All stones are player's color or Clear”
      return ((s1==color || s1==Stone::C) &&
              (s2==color || s2==Stone::C) &&
              (s3==color || s3==Stone::C));
    };
    auto allClear = (s1==Stone::C && s2==Stone::C && s3==Stone::C);

    int me = 0, opp = 0;
    if (!allClear) {
      if (allIn(myColor))  me  = 1;
      if (allIn(oppColor)) opp = 1;
    }
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
