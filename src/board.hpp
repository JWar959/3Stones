/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 1
* Class:  CMPS 366 
* Date:  2025-09-09
*********************************************************************/

#pragma once
#include "types.hpp"
#include <array>
#include <utility>

class Board {
public:
  Board();
  bool inBounds(int r, int c) const;
  bool isValidPocket(int r, int c) const;
  bool isEmpty(int r, int c) const;
  bool place(int r, int c, Stone s);
  const Pocket& at(int r, int c) const;
  bool hasEmptyInRow(int r) const;
  bool hasEmptyInCol(int c) const;
  Pocket& at(int r, int c);

  // Scoring helper 
  std::pair<int,int> scoreFromPlacement(const Coord& p, Stone played, Stone myColor) const;

  // quick printer (BoardView provides a nicer one)
  void print() const; 

private:
  std::array<std::array<Pocket,11>,11> grid_;
  void initShape();
};
