/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/

#include "serializer.hpp"
#include <cstring>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cctype>




static char stoneChar(Stone s) { return stoneToChar(s); }

// Helper function to help with whitespace issues
static inline void trim(std::string& s) {
    auto notsp = [](int ch){ return !std::isspace(ch); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), notsp));
    s.erase(std::find_if(s.rbegin(), s.rend(), notsp).base(), s.end());
}

// helper function to help deal with formatting during reading / loading files
static bool starts_with_label(std::string s, const char* label) {
    trim(s);
    // compare only up to label length; tolerate trailing spaces after colon
    if (s.size() < std::strlen(label)) return false;
    return std::equal(label, label+std::strlen(label), s.begin());
}

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

bool Serializer::load(Board& b, Player& human, Player& cpu,
                      bool& nextIsHuman, Coord& lastOpp, const std::string& filename)
{
    std::ifstream in(filename);
    if (!in) return false;

    std::string line;

    // Human Player
    if (!std::getline(in, line) || !starts_with_label(line, "Human Player:")) return false;
    {
        std::istringstream iss(line.substr(line.find(':')+1));
        std::string color; int pts, nb, nw, nc, rounds;
        if (!(iss >> color >> pts >> nb >> nw >> nc >> rounds)) return false;
        Stone humanColor = (color == "Black" ? Stone::B : Stone::W);
        Inventory inv(humanColor, pts, nb, nw, nc, rounds);
        human.setInventory(inv);
    }

    // Computer Player
    if (!std::getline(in, line) || !starts_with_label(line, "Computer Player:")) return false;
    {
        std::istringstream iss(line.substr(line.find(':')+1));
        std::string color; int pts, nb, nw, nc, rounds;
        if (!(iss >> color >> pts >> nb >> nw >> nc >> rounds)) return false;
        Stone cpuColor = (color == "Black" ? Stone::B : Stone::W);
        Inventory inv(cpuColor, pts, nb, nw, nc, rounds);
        cpu.setInventory(inv);
    }

    // Next Player
    if (!std::getline(in, line) || !starts_with_label(line, "Next Player:")) return false;
    {
        std::istringstream iss(line.substr(line.find(':')+1));
        std::string who; int r, c;
        if (!(iss >> who >> r >> c)) return false;
        nextIsHuman = (who == "Human");
        // If the player coming in had no last move, we'll handle it safely below
        if (r <= 0 || c <= 0) lastOpp = {-1, -1};
        else                  lastOpp = {r-1, c-1};
    }

    // Board label (tolerate trailing space)
    if (!std::getline(in, line) || !starts_with_label(line, "Board:")) return false;

    // expected pocket counts per row in order
    const int counts[11] = {3,5,7,9,11,10,11,9,7,5,3};
    // clear board first
    b = Board();

    for (int r = 0; r < 11; ++r) {
        if (!std::getline(in, line)) return false;
        std::istringstream row(line);
        std::vector<char> toks;
        std::string tok;
        while (row >> tok) {
            if (!tok.empty()) toks.push_back(std::toupper(static_cast<unsigned char>(tok[0])));
        }
        if ((int)toks.size() != counts[r]) return false;

        // compute column start so tokens line up into the octagon pockets
        int start = (11 - counts[r]) / 2;
        int ci = 0;
        for (int c = 0; c < 11; ++c) {
            if (c < start || c >= start + counts[r]) continue; // outside octagon
            if (r == 5 && c == 5) continue; // center void
            char ch = toks[ci++];
            Stone s = Stone::Empty;
            if (ch == 'B') s = Stone::B;
            else if (ch == 'W') s = Stone::W;
            else if (ch == 'C') s = Stone::C;
            else s = Stone::Empty; // treat unknown as empty
            if (s != Stone::Empty) b.place(r, c, s);
        }
    }

    return true;
}
