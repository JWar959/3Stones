/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 1
* Class:  CMPS 366
* Date:  2025-09-09
*********************************************************************/

// human.cpp
#include "human.hpp"
#include <iostream>
#include <limits>
#include <cctype>

static int readIntInRange(const std::string& prompt, int lo, int hi) {
  while (true) {
    std::cout << prompt;
    int x;
    if (std::cin >> x && x >= lo && x <= hi) return x;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input. Try again.\n";
  }
}

// Prompts for stone, supports 'H' to ask for help via callback
static Stone readStoneFromInv(const Inventory& inv,
                              Board& board,
                              Coord oppLast,
                              Human::HelpFn recommendCb,
                              Stone myColor) {
  while (true) {
    std::cout << "Stone to play (B/W/C) or H for help: ";
    char ch;
    if (!(std::cin >> ch)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    }
    ch = std::toupper(static_cast<unsigned char>(ch));

    if (ch == 'H') {
      if (!recommendCb) {
        std::cout << "Help not available right now.\n";
        continue;
      }
      Move rec = recommendCb(board, oppLast, myColor, inv);
      std::cout << "Help: try " << stoneToString(rec.played)
                << " at (" << rec.pos.r + 1 << "," << rec.pos.c + 1 << ")";
      if (!rec.rationale.empty()) std::cout << " — " << rec.rationale;
      std::cout << "\n";
      continue;
    }

    if (ch == 'B' && inv.black > 0) return Stone::B;
    if (ch == 'W' && inv.white > 0) return Stone::W;
    if (ch == 'C' && inv.clear > 0) return Stone::C;

    std::cout << "Not available. Choose again.\n";
  }
}

Move Human::chooseMove(Board& board, Coord oppLast) {
  Move m;
  m.played = readStoneFromInv(this->inv(), board, oppLast, this->help_, this->inv().myColor);
  m.pos.r  = readIntInRange("Row (1-11): ", 1, 11) - 1;
  m.pos.c  = readIntInRange("Col (1-11): ", 1, 11) - 1;
  m.isValid = true;
  return m;
}