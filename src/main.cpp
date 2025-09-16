/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 1
* Class:  CMPS 366
* Date:  2025-09-09
*********************************************************************/

#include "board.hpp"
#include "board_view.hpp"
#include "human.hpp"
#include "computer.hpp"
#include "round.hpp"
#include "serializer.hpp"
#include "tournament.hpp"
#include <iostream>

int main() {
  std::cout << "Welcome to 3 Stones!\n";
  Board board;
  BoardView view;
  Human human("Human");
  Computer cpu("Computer");
  Tournament tour;

  Round round(board, human, cpu);
  round.decideFirstPlayerWithCoinCall();

  // Wire help: let Human call into Computer’s recommender when 'H' is pressed
  human.setHelpCallback([&cpu](Board& b, Coord lastOpp, Stone humanColor, const Inventory& humanInv) {
    return cpu.recommendForHuman(b, lastOpp, humanColor, humanInv);
  });

  while (!round.isOver()) {
    view.print(board);

    bool moved = false;
    int retries = 0;
    const int MAX_RETRIES = 500; // safety cap to avoid infinite loops on bugs

    while (!moved && retries < MAX_RETRIES) {
      Player& cur = round.current();
      Move m = cur.chooseMove(board, round.lastOpponentCoord());

      if (round.applyMove(m)) {
        std::cout << round.lastMoveExplanation() << "\n";
        moved = true;
      } else {
        if (round.currentIsHuman()) {
          std::cout << "Illegal move; try again.\n";
          // The human's chooseMove() will re-prompt on the next loop iteration.
        } else {
          // Computer: silent retry (no spam).
        }
        ++retries;
      }
    }

    if (!moved) {
      std::cout << "Turn skipped: no legal move found.\n";
      break; // exit the outer while
    }
  } // <-- closes while (!round.isOver())

  // These belong AFTER the loop
  tour.recordRoundOutcome(human, cpu);
  std::cout << "Round over. Points - Human: " << human.inv().points
            << ", Computer: " << cpu.inv().points << "\n";
  std::cout << "Tournament winner so far: " << tour.winnerName(human, cpu) << "\n";

  return 0;
}

