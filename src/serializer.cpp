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
                      bool& nextIsHuman, Coord& lastOpp,
                      const std::string& filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in) return false;

    // Read entire file into a single string
    std::string all((std::istreambuf_iterator<char>(in)),
                    std::istreambuf_iterator<char>());

    // Normalize whitespace a bit (optional but resilient)
    auto squeeze = [](std::string s) {
        // turn CRLF into LF, ensure spaces between labels if missing
        for (auto& ch : s) if (ch == '\r') ch = '\n';
        return s;
    };
    all = squeeze(all);

    // Find label positions
    auto find_lbl = [&](const char* lbl)->std::size_t {
        return all.find(lbl);
    };

    const char* L_HUM = "Human Player:";
    const char* L_CPU = "Computer Player:";
    const char* L_NEXT= "Next Player:";
    const char* L_BRD = "Board:";

    std::size_t pH = find_lbl(L_HUM);
    std::size_t pC = find_lbl(L_CPU);
    std::size_t pN = find_lbl(L_NEXT);
    std::size_t pB = find_lbl(L_BRD);

    if (pH == std::string::npos || pC == std::string::npos ||
        pN == std::string::npos || pB == std::string::npos)
        return false;

    auto after = [&](std::size_t p, const char* lbl){ return p + std::strlen(lbl); };

    // Slice sections (trimmed later by stringstreams)
    std::string humanPart = all.substr(after(pH,L_HUM), pC - after(pH,L_HUM));
    std::string cpuPart   = all.substr(after(pC,L_CPU), pN - after(pC,L_CPU));
    std::string nextPart  = all.substr(after(pN,L_NEXT), pB - after(pN,L_NEXT));
    std::string boardPart = all.substr(after(pB,L_BRD)); // to end

    // ---- Parse human ----
    {
        std::istringstream iss(humanPart);
        std::string color; int pts, nb, nw, nc, rounds;
        if (!(iss >> color >> pts >> nb >> nw >> nc >> rounds)) return false;
        Stone humanColor = (color == "Black" ? Stone::B : Stone::W);
        Inventory inv(humanColor, pts, nb, nw, nc, rounds);
        human.setInventory(inv);
    }

    // ---- Parse computer ----
    {
        std::istringstream iss(cpuPart);
        std::string color; int pts, nb, nw, nc, rounds;
        if (!(iss >> color >> pts >> nb >> nw >> nc >> rounds)) return false;
        Stone cpuColor = (color == "Black" ? Stone::B : Stone::W);
        Inventory inv(cpuColor, pts, nb, nw, nc, rounds);
        cpu.setInventory(inv);
    }

    // ---- Parse next player ----
    {
        std::istringstream iss(nextPart);
        std::string who; int r, c;
        if (!(iss >> who >> r >> c)) return false;
        nextIsHuman = (who == "Human");
        if (r <= 0 || c <= 0) lastOpp = {-1,-1};
        else lastOpp = {r-1, c-1}; // file is 1-based
    }

    // ---- Parse board tokens (works for single-line or multi-line) ----
    // Tokenize everything after "Board:" by whitespace
    std::istringstream bss(boardPart);
    std::vector<char> tokens;
    std::string t;
    while (bss >> t) {
        if (!t.empty())
            tokens.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(t[0]))));
    }

    // Reset/shape the board
    b = Board();

    // Place tokens row-by-row into valid pockets
    std::size_t idx = 0;
    for (int r = 0; r < 11; ++r) {
        // collect valid columns for this row
        std::vector<int> validCols;
        for (int c = 0; c < 11; ++c)
            if (b.isValidPocket(r, c)) validCols.push_back(c);

        if (idx + validCols.size() > tokens.size()) return false;

        for (std::size_t i = 0; i < validCols.size(); ++i) {
            char ch = tokens[idx++];
            Stone s = Stone::Empty;
            if (ch == 'B') s = Stone::B;
            else if (ch == 'W') s = Stone::W;
            else if (ch == 'C') s = Stone::C;
            if (s != Stone::Empty) b.place(r, validCols[i], s);
        }
    }

    return true;
}
