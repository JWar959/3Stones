/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/


#include "computer.hpp"
#include <limits>
#include <cstdlib>
#include <vector>
#include <algorithm>

// We need to store a list of all possible recommended moves to the player
std::vector<Computer::ScoredLine>
Computer::listAllHumanCandidates(Board& board, Coord lastOpp,
                                 Stone humanColor, const Inventory& humanInv) {
  Stone oppColor = (humanColor == Stone::B ? Stone::W : Stone::B);

  std::vector<Coord> cells;
  enumLegalCells(board, lastOpp, cells);

  std::vector<Stone> stones;
  candidateStones(humanInv, stones); // includes both colors + clear if available

  std::vector<ScoredLine> out;
  out.reserve(cells.size() * stones.size());

  for (const Coord& p : cells) {
    bool blocks = isOpponentThreatAt(board, p, oppColor);
    for (Stone s : stones) {
      Stone prev = board.getStone(p.r, p.c);
      board.setStone(p.r, p.c, s);

      auto deltas = board.scoreFromPlacement(p, s, humanColor);
      int myPts    = deltas.first;
      int oppBest  = opponentBestResponsePts(board, p, oppColor);
      int util     = (myPts * 100) - (oppBest * 100) + (blocks ? 1000 : 0);
      if (s == Stone::C) util -= 1;

      Move m{p, s, true};
      out.push_back(ScoredLine{m, myPts, oppBest, util, blocks});

      board.restoreStone(p.r, p.c, prev);
    }
  }
  // sort best-first for nice display
  std::sort(out.begin(), out.end(), [](const ScoredLine& a, const ScoredLine& b){
    if (a.utility != b.utility) return a.utility > b.utility;
    if (a.blocked != b.blocked) return a.blocked;      // prefer blocks
    if (a.myPts   != b.myPts)   return a.myPts > b.myPts;
    return a.oppBestPts < b.oppBestPts;
  });
  return out;
}

Move Computer::recommendForHuman(Board& board, Coord lastOpp,
                                 Stone humanColor, const Inventory& humanInv) {
  auto all = listAllHumanCandidates(board, lastOpp, humanColor, humanInv);
  if (all.empty()) return Move{}; // invalid

  // Bias: prefer human's own color unless an opponent color move gives strictly
  // higher utility OR is the only move that blocks an immediate threat.
  int bestIdxOwn = -1, bestIdxAny = 0;
  for (int i = 0; i < (int)all.size(); ++i) {
    if (all[i].m.played == humanColor || all[i].m.played == Stone::C) {
      if (bestIdxOwn == -1) bestIdxOwn = i;
    }
    if (all[i].utility > all[bestIdxAny].utility) bestIdxAny = i;
  }

  // Check if there exists an opponent color move that is a strictly better blocker.
  int bestOppIdx = -1;
  for (int i = 0; i < (int)all.size(); ++i) {
    if (all[i].m.played != humanColor && all[i].m.played != Stone::C) {
      if (bestOppIdx == -1 || all[i].utility > all[bestOppIdx].utility) bestOppIdx = i;
    }
  }

  int pick = bestIdxOwn;
  if (pick == -1) pick = bestIdxAny; // no own/clear available

  // If an opponent color move strictly improves utility or is the only blocking move, allow it
  if (bestOppIdx != -1) {
    bool onlyBlockIsOpp = false;
    bool anyOwnBlocks = false;
    for (const auto& s : all) {
      if (s.blocked && (s.m.played == humanColor || s.m.played == Stone::C)) { anyOwnBlocks = true; break; }
    }
    if (!anyOwnBlocks) {
      for (const auto& s : all) { if (s.blocked && s.m.played != humanColor && s.m.played != Stone::C) { onlyBlockIsOpp = true; break; } }
    }
    if (onlyBlockIsOpp || all[bestOppIdx].utility > all[pick].utility) pick = bestOppIdx;
  }

  auto chosen = all[pick];
  // Rationale string
  Computer::Scored s;
  s.m = chosen.m; s.myPts = chosen.myPts; s.oppBestPts = chosen.oppBestPts; s.utility = chosen.utility; s.blocked = chosen.blocked;
  chosen.m.rationale = "Recommendation: " + rationaleText(s);
  return chosen.m;
}



// Helper: list legal target cells based on last opponent move and row/col rule
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
  if (inv.myColor == Stone::B) {
    if (inv.black>0) out.push_back(Stone::B);
    if (inv.white>0) out.push_back(Stone::W);
  } else {
    if (inv.white>0) out.push_back(Stone::W);
    if (inv.black>0) out.push_back(Stone::B);
  }
  if (inv.clear>0) out.push_back(Stone::C);
}

// Would opponent score immediately if they played at p right now?
bool Computer::isOpponentThreatAt(Board& board, const Coord& p, Stone opponentColor) {
  if (!board.isValidPocket(p.r,p.c) || !board.isEmpty(p.r,p.c)) return false;

  Stone prev = board.getStone(p.r, p.c);

  board.setStone(p.r, p.c, opponentColor);
  auto ptsOwn = board.scoreFromPlacement(p, opponentColor, opponentColor).first;
  board.restoreStone(p.r, p.c, prev);
  if (ptsOwn >= 1) return true;

  board.setStone(p.r, p.c, Stone::C);
  auto ptsClear = board.scoreFromPlacement(p, Stone::C, opponentColor).first;
  board.restoreStone(p.r, p.c, prev);
  return ptsClear >= 1;
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

Computer::Scored Computer::evaluateBestFor(Board& board, Coord lastOpp,
                         Stone actorColor, const Inventory& actorInv,
                         Stone opponentColor) {
  std::vector<Coord> cells;
  enumLegalCells(board, lastOpp, cells);

  std::vector<Stone> stones;
  candidateStones(actorInv, stones);

  Scored best{};
  best.utility = std::numeric_limits<int>::min();

  for (const Coord& p : cells) {
    // Check explicit block: if opponent could score at p now, occupying p is a block
    bool blocks = isOpponentThreatAt(board, p, opponentColor);

    for (Stone s : stones) {
      Stone prev = board.getStone(p.r, p.c);
      board.setStone(p.r, p.c, s);

      auto deltas = board.scoreFromPlacement(p, s, actorColor);
      int myPts = deltas.first;
      int oppBest = opponentBestResponsePts(board, p, opponentColor);

      // Utility: maximize self; minimize opponent; prioritize explicit blocks.
      int util = (myPts * 100) - (oppBest * 100) + (blocks ? 1000 : 0);

      // conserve clears slightly
      if (s == Stone::C) util -= 1; 

      bool better = (util > best.utility) ||
                    (util == best.utility && blocks && !best.blocked) ||
                    (util == best.utility && myPts > best.myPts) ||
                    (util == best.utility && myPts == best.myPts && oppBest < best.oppBestPts);

      if (better) {
        best.utility = util;
        best.myPts = myPts;
        best.oppBestPts = oppBest;
        best.m = Move{p, s, true};
        best.blocked = blocks;
      }
      board.restoreStone(p.r, p.c, prev);
    }
  }
  return best;
}

static std::string posStr(const Coord& p) {
  return "(" + std::to_string(p.r+1) + "," + std::to_string(p.c+1) + ")";
}

std::string Computer::rationaleText(const Scored& s) {
  std::string why;
  why += "It selected " + stoneToString(s.m.played) + " at " + posStr(s.m.pos) + " ";
  why += "(+" + std::to_string(s.myPts) + " now";
  if (s.oppBestPts > 0) why += ", opp next best +" + std::to_string(s.oppBestPts);
  why += ").";
  if (s.blocked) why += " Blocks an immediate opponent score.";
  return why;
}

Move Computer::chooseMove(Board& board, Coord lastOpp) {
  const auto& invMe = this->inv();
  Stone myColor  = invMe.myColor;
  Stone oppColor = (myColor == Stone::B ? Stone::W : Stone::B);

  auto scored = evaluateBestFor(board, lastOpp, myColor, invMe, oppColor);

  if (!scored.m.isValid) {
    std::vector<Coord> cells;
    enumLegalCells(board, lastOpp, cells);
    if (!cells.empty()) {
      Coord p = cells[std::rand() % cells.size()];
      Stone s = myColor;
      if ((myColor==Stone::B && invMe.black==0) || (myColor==Stone::W && invMe.white==0)) {
        s = (invMe.clear>0) ? Stone::C : oppColor;
      }
      return Move{p, s, true, "Fallback move (no evaluated best)."};
    }
  }

  scored.m.rationale = rationaleText(scored);
  return scored.m;
}

/* Move Computer::recommendForHuman(Board& board, Coord lastOpp, Stone humanColor, const Inventory& humanInv) {
  Stone compColor = (humanColor == Stone::B ? Stone::W : Stone::B);
  auto scored = evaluateBestFor(board, lastOpp, humanColor, humanInv, compColor);
  scored.m.rationale = "Recommendation for you: " + rationaleText(scored);
  return scored.m;
} */


