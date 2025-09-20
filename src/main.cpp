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

  // We'll want to make sure we display which player made the last move (if any), as well
  // as what that move actually was. This ensures the player will know what move they can make 
  // next based off of the last move that gets displayed.
  Round round(board, human, cpu);
  if (mode == 'R') {
    std::cout << "\n=== Resumed Game ===\n";
    std::cout << "Next to move: " << (nextIsHuman ? human.name() : cpu.name()) << "\n";

    if (lastOpp.r >= 0 && lastOpp.c >= 0) {
      char st = stoneToChar(board.at(lastOpp.r, lastOpp.c).s);
      std::cout << "Last move: " 
                << (nextIsHuman ? cpu.name() : human.name())  // last mover is the other one
                << " played " << st 
                << " at (" << (lastOpp.r+1) << "," << (lastOpp.c+1) << ")\n";

      // Optional: remind the legal-placement rule
      std::cout << "Rule: Your move must be in row " << (lastOpp.r+1)
                << " or column " << (lastOpp.c+1)
                << " if any open pockets remain there.\n";
    } else {
      std::cout << "No moves have been played yet.\n";
    }
    std::cout << "====================\n\n";
  }

  // Wire help: let Human call into Computer’s recommender when 'H' is pressed
  human.setHelpCallback([&cpu](Board& b, Coord lastOpp, Stone humanColor, const Inventory& humanInv) {
    return cpu.recommendForHuman(b, lastOpp, humanColor, humanInv);
  });

  while (!round.isOver()) {
    view.print(board);
    // Print a reminder as to the rules of the game for ease of use
    if (round.lastOpponentCoord().r >= 0) {
      std::cout << "Legal placements must be in row " << (round.lastOpponentCoord().r+1)
                << " or column " << (round.lastOpponentCoord().c+1)
                << " (unless both are full).\n";
    }

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
