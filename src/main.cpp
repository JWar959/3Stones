/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/

#include "board.hpp"
#include "board_view.hpp"
#include "human.hpp"
#include "computer.hpp"
#include "round.hpp"
#include "serializer.hpp"
#include "tournament.hpp"
#include <iostream>
#include <string>   // for std::string
#include <cctype>   // for std::toupper

int main() {
  std::cout << "Welcome to 3 Stones!\n";

  Board board;
  BoardView view;
  Human human("Human");
  Computer cpu("Computer");
  Tournament tour;

  char mode;
  std::cout << "(N)ew game or (R)esume from file? ";
  std::cin >> mode; 
  mode = std::toupper(static_cast<unsigned char>(mode));

  bool nextIsHuman = true;
  Coord lastOpp{-1,-1};

  if (mode == 'R') {
    std::string fname;
    std::cout << "Enter filename: ";
    std::cin >> fname;
    if (!Serializer::load(board, human, cpu, nextIsHuman, lastOpp, fname)) {
      std::cout << "Load failed. Starting new game.\n";
      nextIsHuman = true; 
      lastOpp = {-1,-1};
    }
  }

  Round round(board, human, cpu);
  if (mode == 'R') {
    round.initFromLoad(nextIsHuman, lastOpp);
  } else {
    round.decideFirstPlayerWithCoinCall();
  }

  // Wire help: let Human call into Computer’s recommender when 'H' is pressed
  human.setHelpCallback([&cpu](Board& b, Coord lastOpp, Stone humanColor, const Inventory& humanInv) {
    return cpu.recommendForHuman(b, lastOpp, humanColor, humanInv);
  });

  while (!round.isOver()) {
    view.print(board);

    bool moved = false;
    int retries = 0;
    const int MAX_RETRIES = 500; // safety cap

    while (!moved && retries < MAX_RETRIES) {
      Player& cur = round.current();
      Move m = cur.chooseMove(board, round.lastOpponentCoord());

      if (round.applyMove(m)) {
        std::cout << round.lastMoveExplanation() << "\n";
        if (!m.rationale.empty() && !round.currentIsHuman()) { // last mover was computer
          std::cout << m.rationale << "\n";
        }
        moved = true;

        // Offer serialize and quit option
        std::cout << "Serialize and quit? (Y/N): ";
        char ans; 
        std::cin >> ans; 
        ans = std::toupper(static_cast<unsigned char>(ans));
        if (ans == 'Y') {
          bool nextIsHumanOut = round.currentIsHuman();     // whose turn next
          Coord lastOppOut    = round.lastOpponentCoord();  // last move (0-based)
          if (Serializer::save(board, human, cpu, nextIsHumanOut, lastOppOut, "save.txt")) {
            std::cout << "Saved to save.txt. Exiting...\n";
            return 0;
          } else {
            std::cout << "Save failed.\n";
          }
        }
      } else {
        // Illegal move occurred
        if (round.currentIsHuman()) {
          std::cout << "Illegal move; try again.\n";
          // Human will be re-prompted next iteration
        } else {
          // Computer: silent retry
        }
        ++retries;
      }
    }

    if (!moved) {
      std::cout << "Turn skipped: no legal move found.\n";
      break; // exit the outer while
    }
  } // while (!round.isOver())

  // After the loop
  tour.recordRoundOutcome(human, cpu);
  std::cout << "Round over. Points - Human: " << human.inv().points
            << ", Computer: " << cpu.inv().points << "\n";
  std::cout << "Tournament winner so far: " << tour.winnerName(human, cpu) << "\n";

  return 0;
}
