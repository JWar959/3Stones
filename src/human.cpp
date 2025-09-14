/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 1
* Class:  CMPS 366
* Date:  2025-09-09
*********************************************************************/

#include "human.hpp"
#include <iostream>
#include <limits>
#include <cctype>

static int readIntInRange(const std::string& prompt, int lo, int hi) {
  while (true) {
    std::cout << prompt;
    int x;
    if (std::cin >> x && x>=lo && x<=hi) return x;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input. Try again.\n";
  }
}

static Stone readStoneFromInv(const Inventory& inv) {
  while (true) {
    std::cout << "Stone to play (B/W/C): ";
    char ch; if (!(std::cin >> ch)) { std::cin.clear(); continue; }
    ch = std::toupper(static_cast<unsigned char>(ch));
    if (ch=='B' && inv.black>0) return Stone::B;
    if (ch=='W' && inv.white>0) return Stone::W;
    if (ch=='C' && inv.clear>0) return Stone::C;
    std::cout << "Not available. Choose again.\n";
  }
}

Move Human::chooseMove(const Board& board, const Coord& oppLast) {
  // legality is enforced in Round::applyMove
  (void)board; (void)oppLast; 
  Move m;
  m.played = readStoneFromInv(this->inv());
  m.pos.r = readIntInRange("Row (1-11): ", 1, 11) - 1;
  m.pos.c = readIntInRange("Col (1-11): ", 1, 11) - 1;
  m.isValid = true;
  return m;
}
