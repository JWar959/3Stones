/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 1
* Class:  CMPS 366
* Date:  2025-09-09
*********************************************************************/

#include "computer.hpp"
#include <vector>
#include <random>
#include <chrono>

Move Computer::chooseMove(const Board& board, const Coord& oppLast) {
  (void)oppLast;
  std::vector<Coord> legal;
  for (int r=0;r<11;++r) for (int c=0;c<11;++c) {
    if (board.isValidPocket(r,c) && board.isEmpty(r,c)) legal.push_back({r,c});
  }
  Move m;
  if (legal.empty()) return m;
  auto seed = (unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> dist(0, (int)legal.size()-1);
  m.pos = legal[dist(gen)];

  // Prefer own color, fallback to available stones
  Stone s = inv().myColor;
  if (s==Stone::B && inv().black==0) s = (inv().white>0?Stone::W:(inv().clear>0?Stone::C:Stone::B));
  if (s==Stone::W && inv().white==0) s = (inv().black>0?Stone::B:(inv().clear>0?Stone::C:Stone::W));
  if ((s==Stone::B && inv().black==0) || (s==Stone::W && inv().white==0)) s = Stone::C;

  m.played = s;
  m.isValid = true;
  return m;
}
