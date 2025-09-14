/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 1
* Class:  CMPS 366
* Date:  2025-09-09
*********************************************************************/

#include "serializer.hpp"
#include <fstream>
#include <sstream>

static char stoneChar(Stone s) {
  switch (s) { case Stone::B: return 'B'; case Stone::W: return 'W'; case Stone::C: return 'C'; default: return 'O'; }
}

bool Serializer::save(const std::string& path, const Player& human, const Player& cpu,
                      const Round& round, const Board& board) {
  std::ofstream out(path);
  if (!out) return false;
  const auto& hi = human.inv();
  const auto& ci = cpu.inv();
  out << "Human Player: "   << (hi.myColor==Stone::B ? "Black " : "White ")
      << hi.points << " " << hi.black << " " << hi.white << " " << hi.clear << " " << hi.roundsWon << "\n";
  out << "Computer Player: " << (ci.myColor==Stone::B ? "Black " : "White ")
      << ci.points << " " << ci.black << " " << ci.white << " " << ci.clear << " " << ci.roundsWon << "\n";
  auto last = round.lastOpponentCoord();
  out << "Next Player: " << ("Human") << " "
      << (last.r>=0? last.r+1: -1) << " " << (last.c>=0? last.c+1: -1) << "\n";
  out << "Board:\n";
  for (int r=0;r<11;++r) {
    bool first=true;
    for (int c=0;c<11;++c) {
      const Pocket& pk = board.at(r,c);
      if (!pk.valid) continue;
      if (!first) out << " ";
      first=false;
      out << stoneChar(pk.s);
    }
    out << "\n";
  }
  return true;
}

bool Serializer::load(const std::string& path, Player& human, Player& cpu,
                      Round& round, Board& board) {
  (void)path; (void)human; (void)cpu; (void)round; (void)board;
  // Milestone 1: not implemented yet
  return false;
}
