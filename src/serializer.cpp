/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/

#include "serializer.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

static char stoneChar(Stone s) { return stoneToChar(s); }

static void writeBoard(std::ostream& out, const Board& b) {
  out << "Board:\n";
  for (int r=0;r<11;++r) {
    // indentation to match the octagon (spaces for invalid leading pockets)
    int leading = 0;
    for (int c=0;c<11;++c) if (!b.isValidPocket(r,c)) ++leading; else break;
    // convert leading invalid cells into padding roughly like the sample
    out << std::string((leading>0)?2:0, ' ');
    bool first = true;
    for (int c=0;c<11;++c) {
      if (!b.isValidPocket(r,c)) continue;
      if (!first) out << ' ';
      out << stoneChar(b.at(r,c).s);
      first = false;
    }
    out << "\n";
  }
}

bool Serializer::save(const Board& b,
                      const Player& human, const Player& computer,
                      bool nextIsHuman, Coord lastOpp,
                      const std::string& filename) {
  std::ofstream out(filename);
  if (!out) return false;

  // Human Player: <Color> <points> <#B> <#W> <#C> <roundsWon>
  const auto& hi = human.inv();
  const auto& ci = computer.inv();

  auto colorName = [](Stone s){ return (s==Stone::B?"Black":"White"); };

  out << "Human Player: " << colorName(hi.myColor) << ' '
      << hi.points << ' ' << hi.black << ' ' << hi.white << ' ' << hi.clear << ' ' << hi.roundsWon << "\n";
  out << "Computer Player: " << colorName(ci.myColor) << ' '
      << ci.points << ' ' << ci.black << ' ' << ci.white << ' ' << ci.clear << ' ' << ci.roundsWon << "\n";

  out << "Next Player: " << (nextIsHuman ? "Human" : "Computer") << ' '
      << (lastOpp.r==-1? -1 : lastOpp.r+1) << ' ' << (lastOpp.c==-1? -1 : lastOpp.c+1) << "\n";

  writeBoard(out, b);
  return true;
}

// NOTE: Implementing a robust parser is a bit longer; here’s a minimal loader
// that expects the exact format we write in save().
bool Serializer::load(Board& b,
                      Player& human, Player& computer,
                      bool& nextIsHuman, Coord& lastOpp,
                      const std::string& filename) {
  std::ifstream in(filename);
  if (!in) return false;

  std::string line;

  auto readInv = [&](Player& P, const std::string& prefix){
    std::getline(in, line);
    if (line.rfind(prefix, 0) != 0) return false;
    std::istringstream ss(line.substr(prefix.size()));
    std::string color; int pts, nb, nw, nc, rw;
    if (!(ss >> color >> pts >> nb >> nw >> nc >> rw)) return false;
    auto& I = P.inv();
    I.myColor = (color=="Black"? Stone::B : Stone::W);
    I.points = pts; I.black = nb; I.white = nw; I.clear = nc; I.roundsWon = rw;
    return true;
  };

  if (!readInv(human, "Human Player: ")) return false;
  if (!readInv(computer, "Computer Player: ")) return false;

  // Next Player
  std::getline(in, line);
  {
    if (line.rfind("Next Player: ", 0) != 0) return false;
    std::istringstream ss(line.substr(std::string("Next Player: ").size()));
    std::string who; int rr, cc;
    if (!(ss >> who >> rr >> cc)) return false;
    nextIsHuman = (who=="Human");
    if (rr<=0 || cc<=0) lastOpp = {-1,-1};
    else lastOpp = {rr-1, cc-1};
  }

  // Board header
  std::getline(in, line); // Should be "Board:"
  if (line != "Board:") return false;

  // Clear board and read 11 printed rows
  // (We’ll reconstruct by scanning tokens line-by-line)
  // First, blank the board:
  for (int r=0;r<11;++r) for (int c=0;c<11;++c)
    if (b.isValidPocket(r,c)) b.restoreStone(r,c,Stone::Empty);

  for (int r=0; r<11; ++r) {
    if (!std::getline(in, line)) return false;
    std::istringstream ss(line);
    std::vector<char> tokens;
    char tok;
    while (ss >> tok) tokens.push_back(tok);

    // Walk across columns; assign only to valid pockets, in left-to-right order
    int t = 0;
    for (int c=0;c<11;++c) {
      if (!b.isValidPocket(r,c)) continue;
      if (t >= (int)tokens.size()) return false;
      char ch = tokens[t++];
      Stone s = Stone::Empty;
      if (ch=='B') s=Stone::B;
      else if (ch=='W') s=Stone::W;
      else if (ch=='C') s=Stone::C;
      else s=Stone::Empty;
      b.restoreStone(r,c,s);
    }
  }
  return true;
}
