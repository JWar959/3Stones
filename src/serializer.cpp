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
        if (r <= 0 || c <= 0) {
            // If we're here, then it means there was no last move yet
            lastOpp = {-1, -1};     
        } else {
            // If we're here, than there was a last move saved, and file is 1-based so we'll subtract 1 to match the board
            lastOpp = {r-1, c-1};   
        }
    }

    // Board label
    /*
    Instead of guessing a range, ask the Board which pockets are valid on that row 
    and then map the tokens to those positions, in order. This will automatically handle the center void and 
    the tapered octagon edges
    */
    if (!std::getline(in, line) || !starts_with_label(line, "Board:")) return false;

    // Reset board to its shape (valid masks set, stones empty)
    b = Board();

    for (int r = 0; r < 11; ++r) {
        if (!std::getline(in, line)) return false;

        // tokenize row into chars (O/W/B/C), ignore spacing/indentation
        std::istringstream row(line);
        std::vector<char> toks;
        std::string tok;
        while (row >> tok) {
            if (!tok.empty()) toks.push_back(std::toupper(static_cast<unsigned char>(tok[0])));
        }

        // gather valid columns for this row from the board's mask
        std::vector<int> validCols;
        for (int c = 0; c < 11; ++c) {
            if (b.isValidPocket(r, c)) validCols.push_back(c);
        }

        // expect exactly one token per valid pocket
        if ((int)toks.size() != (int)validCols.size()){
            return false;
        } 

        // place tokens into those valid pockets
        for (size_t i = 0; i < validCols.size(); ++i) {
            char ch = toks[i];
            Stone s = Stone::Empty;
            if (ch == 'B') s = Stone::B;
            else if (ch == 'W') s = Stone::W;
            else if (ch == 'C') s = Stone::C;
            // 'O' or anything else stays Empty
            if (s != Stone::Empty) b.place(r, validCols[i], s);
        }
    }

    return true;
}
