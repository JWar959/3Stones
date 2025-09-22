/*********************************************************************
* Name:  John Warren
* Project:  3 Stones
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
#include <string>
#include <cctype>
#include <sstream>
#include <cmath>  

// Function prototypes
// Reset per-round inventory/points but keep roundsWon (tournament stat)
static void resetForNewRound(Player& p);

int main() {

  Board board;
  BoardView view;
  Tournament tour;
  std::string firstName;

  // Welcome the player
  std::cout <<  "**************************\n";
  std::cout <<  "***Welcome to 3 Stones!***\n";
  std::cout <<  "**************************\n\n";

  firstName = Human::inputName();
  Human human(firstName);
  human.setHumanName(firstName);
  Computer cpu("Computer");

  // Wire help: let Human call into Computer’s recommender when 'H' is pressed
  human.setHelpCallback([&cpu](Board& b, Coord lastOpp, Stone humanColor, const Inventory& humanInv) {
    auto all = cpu.listAllHumanCandidates(b, lastOpp, humanColor, humanInv);
    if (all.empty()) return Move{}; // no legal plays

    std::cout << "\nAll legal plays (best first):\n";
    for (const auto& s : all) {
        std::cout << "  " << stoneToString(s.m.played)
                  << " at (" << (s.m.pos.r + 1) << "," << (s.m.pos.c + 1) << ")"
                  << "  me+=" << s.myPts
                  << "  opp_best=" << s.oppBestPts
                  << "  net=" << s.utility
                  << (s.blocked ? "  [blocks]" : "")
                  << "\n";
    }

    // Best candidate is first
    const auto& top = all.front();
    Move best = top.m;

    std::ostringstream os;
    os << "Strategy: " << (top.blocked ? "Block threat" : "Maximize net gain")
       << " | me+=" << top.myPts
       << " | opp_best=" << top.oppBestPts
       << " | net=" << top.utility;
    best.rationale = os.str();

    return best;
});

  // tournament loop: play one round, then ask to continue
  for (;;) {  
    // Fresh board every round
    board = Board();

    // Reset per-round inventories & points (roundsWon persists)
    resetForNewRound(human);
    resetForNewRound(cpu);

    // Let the user choose New vs Resume EVERY round (matches instructor text)
    char mode;
    while(true){
      std::cout << "(N)ew game or (R)esume from file? ";
      if (std::cin >> mode) {
        mode = std::toupper(static_cast<unsigned char>(mode));
        if (mode =='N' || mode =='R') break;
      }
      std::cout << "Invalid. Enter N or R.\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    bool nextIsHuman = true;
    Coord lastOpp{-1,-1};

    Round round(board, human, cpu);

    if (mode == 'R') {
      std::string fname;
      std::cout << "Enter filename: ";
      std::cin >> fname;

      if (!Serializer::load(board, human, cpu, nextIsHuman, lastOpp, fname)) {
        std::cout << "Load failed. Starting new game.\n";
        nextIsHuman = true;
        lastOpp = {-1,-1};
        round.decideFirstPlayerWithCoinCall();
      } else {
        std::cout << "\n=== Resumed Game ===\n";
        std::cout << "Next to move: " << (nextIsHuman ? human.name() : cpu.name()) << "\n";
        if (lastOpp.r >= 0 && lastOpp.c >= 0) {
          char st = stoneToChar(board.at(lastOpp.r, lastOpp.c).s);
          std::cout << "Last move: "
                    << (nextIsHuman ? cpu.name() : human.name())
                    << " played " << st << " at (" << (lastOpp.r+1) << "," << (lastOpp.c+1) << ")\n";
          std::cout << "Rule: Your move must be in row " << (lastOpp.r+1)
                    << " or column " << (lastOpp.c+1) << " if any open pockets remain there.\n";
        } else {
          std::cout << "No moves have been played yet.\n";
        }
        std::cout << "====================\n\n";

        // Tell Round whose turn is next and what the last move was
        round.initFromLoad(nextIsHuman, lastOpp);
      }
    } else {
      // New round: coin flip & color assignment happens here
      round.decideFirstPlayerWithCoinCall();
    }

    // === Your existing “play the round” loop (unchanged, just pasted here) ===
    while (!round.isOver()) {
      view.print(board);
      if (round.lastOpponentCoord().r >= 0) {
        std::cout << "Legal placements must be in row " << (round.lastOpponentCoord().r+1)
                  << " or column " << (round.lastOpponentCoord().c+1)
                  << " (unless both are full).\n";
      }

      bool moved = false;
      int retries = 0;
      const int MAX_RETRIES = 500;

      while (!moved && retries < MAX_RETRIES) {
        Player& cur = round.current();
        Move m = cur.chooseMove(board, round.lastOpponentCoord());

        if (round.applyMove(m)) {
          std::cout << round.lastMoveExplanation() << "\n";
          if (!m.rationale.empty() && !round.currentIsHuman()) {
            std::cout << m.rationale << "\n";
          }

          moved = true;

          // Offer serialize-and-quit
          char ans; 
          for (;;) {
            std::cout << "Serialize and quit? (Y/N): ";
            if (std::cin >> ans) {
              ans = std::toupper(static_cast<unsigned char>(ans));
              if (ans=='Y' || ans=='N') break;
            }
            std::cout << "Enter Y or N.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          }
          
          if (ans == 'Y') {
            bool nextIsHumanOut = round.currentIsHuman();
            Coord lastOppOut    = round.lastOpponentCoord();
            if (Serializer::save(board, human, cpu, nextIsHumanOut, lastOppOut, "save.txt")) {
              std::cout << "Saved to save.txt. Exiting...\n";
              return 0;
            } else {
              std::cout << "Save failed.\n";
            }
          }
        } else {
          if (round.currentIsHuman()) {
            std::cout << "Illegal move; try again.\n";
          }
          ++retries;
        }
      }

      if (!moved) {
        std::cout << "Turn skipped: no legal move found.\n";
        break;
      }
    } // end round

    // Announce round result & record it for the tournament
    tour.recordRoundOutcome(human, cpu);
    std::cout << "Round over. Points - " << human.name() << ": " << human.inv().points
              << ", " << cpu.name() << ": " << cpu.inv().points << "\n";
    if(human.inv().points > cpu.inv().points){
      std::cout << "Congratulations " << human.getHumanName() << "! You won the Round!\n"; 
    }
    else{
      std::cout<< "The Computer wins this round!\n";
    }          
    std::cout << "Total Rounds won - " << human.name() << ": " << human.inv().roundsWon
              << ", " << cpu.name() << ": " << cpu.inv().roundsWon << "\n";

    // Ask to play another round
    char again; 
    for (;;) {
      std::cout << "Play another round? (Y/N): ";
        if (std::cin >> again) {
          again = std::toupper(static_cast<unsigned char>(again));
          if (again=='Y' || again=='N') break;
        }
      std::cout << "Enter Y or N.\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    if (again != 'Y') {
      // Tournament winner by most rounds (NOT points)
      std::cout << "Tournament winner: " << tour.winnerName(human, cpu) << "\n";
      break;
    }
  }

  // After the loop
  tour.recordRoundOutcome(human, cpu);
  std::cout << "Round over. Points - Human: " << human.inv().points
            << ", Computer: " << cpu.inv().points << "\n";
  std::cout << "Tournament winner so far: " << tour.winnerName(human, cpu) << "\n";

  return 0;
}

static void resetForNewRound(Player& p) {
  Inventory inv = p.inv();
  inv.black = 15;
  inv.white = 15;
  inv.clear = 6;
  inv.points = 0;
  // myColor gets set by the coin toss
  // the Round::decideFirstPlayerWithCoinCall() will assign it.
  p.setInventory(inv);
}