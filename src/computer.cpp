/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 1
* Class:  CMPS 366
* Date:  2025-09-09
*********************************************************************/

#include "computer.hpp"
#include <vector>
#include <limits>
#include <cstdlib>

// Helper: list legal target cells based on last opponent move and row/col rule
#include "computer.hpp"
#include <limits>
#include <cstdlib>  // rand

void Computer::enumLegalCells(Board& board, Coord lastOpp, std::vector<Coord>& out) {
  out.clear();
  if (lastOpp.r == -1) {
    for (int r=0;r<11;++r) for (int c=0;c<11;++c)
      if (board.isValidPocket(r,c) && board.isEmpty(r,c)) out.push_back({r,c});
    return;
  }
  bool rowOpen = board.hasEmptyInRow(lastOpp.r);
  bool colOpen = board.hasEmptyInCol(lastOpp.c);
  for (int r=0;r<11;++r) for (int c=0;c<11;++c) {
    if (!board.isValidPocket(r,c) || !board.isEmpty(r,c)) continue;
    if (!rowOpen && !colOpen) { out.push_back({r,c}); continue; }
    if (r==lastOpp.r || c==lastOpp.c) out.push_back({r,c});
  }
}

void Computer::candidateStones(const Inventory& inv, std::vector<Stone>& out) const {
  out.clear();
  // prefer own color first if available
  if (inv.myColor == Stone::B) {
    if (inv.black>0) out.push_back(Stone::B);
    if (inv.white>0) out.push_back(Stone::W);
  } else {
    if (inv.white>0) out.push_back(Stone::W);
    if (inv.black>0) out.push_back(Stone::B);
  }
  if (inv.clear>0) out.push_back(Stone::C);
}

int Computer::opponentBestResponsePts(Board& board, Coord ourMovePos, Stone opponentColor) {
  std::vector<Coord> oppCells;
  enumLegalCells(board, ourMovePos, oppCells);
  int best = 0;
  for (const Coord& p : oppCells) {
    Stone prev = board.getStone(p.r, p.c);
    board.setStone(p.r, p.c, opponentColor);
    auto pts = board.scoreFromPlacement(p, opponentColor, opponentColor);
    best = std::max(best, pts.first);
    board.restoreStone(p.r, p.c, prev);

    prev = board.getStone(p.r, p.c);
    board.setStone(p.r, p.c, Stone::C);
    auto ptsC = board.scoreFromPlacement(p, Stone::C, opponentColor);
    best = std::max(best, ptsC.first);
    board.restoreStone(p.r, p.c, prev);
  }
  return best;
}

Computer::Scored Computer::evaluateBestFor(Board& board,
                         Coord lastOpp,
                         Stone actorColor,
                         const Inventory& actorInv,
                         Stone opponentColor) {
  std::vector<Coord> cells;
  enumLegalCells(board, lastOpp, cells);

  std::vector<Stone> stones;
  candidateStones(actorInv, stones);

  Scored best{};
  best.utility = std::numeric_limits<int>::min();

  for (const Coord& p : cells) {
    for (Stone s : stones) {
      Stone prev = board.getStone(p.r, p.c);
      board.setStone(p.r, p.c, s);

      auto deltas = board.scoreFromPlacement(p, s, actorColor);
      int myPts = deltas.first;
      int oppBest = opponentBestResponsePts(board, p, opponentColor);

      int util = (myPts * 100) - (oppBest * 100);
      if (s == Stone::C) util -= 1;

      bool better = (util > best.utility) ||
                    (util == best.utility && myPts > best.myPts) ||
                    (util == best.utility && myPts == best.myPts && oppBest < best.oppBestPts);

      if (better) {
        best.utility = util;
        best.myPts = myPts;
        best.oppBestPts = oppBest;
        best.m = Move{p, s, true};
      }
      board.restoreStone(p.r, p.c, prev);
    }
  }
  return best;
}

Move Computer::chooseMove(Board& board, Coord lastOpp) {
  const auto& myInv = this->inv();
  Stone myColor = myInv.myColor;
  Stone oppColor = (myColor == Stone::B ? Stone::W : Stone::B);

  auto scored = evaluateBestFor(board, lastOpp, myColor, myInv, oppColor);

  if (!scored.m.isValid) {
    std::vector<Coord> cells;
    enumLegalCells(board, lastOpp, cells);
    if (!cells.empty()) {
      Coord p = cells[std::rand() % cells.size()];
      Stone s = myColor;
      if ((myColor==Stone::B && myInv.black==0) || (myColor==Stone::W && myInv.white==0)) {
        if (myInv.clear>0) s = Stone::C;
        else s = oppColor;
      }
      return Move{p, s, true};
    }
  }
  return scored.m;
}

Move Computer::recommendForHuman(Board& board, Coord lastOpp, Stone humanColor, const Inventory& humanInv) {
  Stone computerColor = (humanColor == Stone::B ? Stone::W : Stone::B);
  auto scored = evaluateBestFor(board, lastOpp, humanColor, humanInv, computerColor);
  return scored.m;
}


