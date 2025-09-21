/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/
#include "human.hpp"
#include "types.hpp"
#include <iostream>
#include <limits>
#include <cctype>
#include <string>
#include <fstream>

// ---------- small local helpers (file-scope) ----------

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

// Reads a name and returns it
std::string Human::inputName()
{
    // VariAable Declorations
    std::string name;
    
    bool checkpoint = true;
    // Exhaust Leading nextline characters
    do{

      // Reset the checkpoint to true so future iterations don't get stuck in the loop
      checkpoint = true;
      std::cout <<"Please enter your first name" << std::endl;
      while( std::cin.peek() == '\n' )
        {
            std::cin.ignore();
        }
      
      // Input Event
      
      std::getline( std::cin, name );

      for (char c : name) {
          // Check if each character is an alphabet (a-z, A-Z)
          // You might extend this to include other characters like hyphens or apostrophes
          // if your definition of a word allows them.
          
          if (!std::isalpha(c)) {
              // Contains non-alphabetic characters
              checkpoint = false; 
          }
        }

        if(name.size() == 0){
            checkpoint = false;
          } 
    }while(!checkpoint);
    
    // Return name
    return name;
}

// Setter for humanName member variable
void Human::setHumanName(const std::string& name) {
  humanName_ = name;
  // sync the Player::name_ here as well so name() returns the new name
  name_ = name;       
}

// Getter function for humanName member variable
std::string Human::getHumanName(){
  return this->humanName_;
}

// Reads a stone and optionally uses the help callback when user enters 'H'
static Stone readStoneFromInv(const Inventory& inv,
                              Board& board,
                              Coord oppLast,
                              const Human::HelpFn& help,
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
      if (help) {
          Move rec = help(board, oppLast, myColor, inv);
          if (rec.isValid) {
              std::cout << "Help: Try " << stoneToString(rec.played)
                        << " at (" << rec.pos.r+1 << "," << rec.pos.c+1 << ").\n";
              if (!rec.rationale.empty()) std::cout << rec.rationale << "\n";
          } else {
              std::cout << "Help not available right now.\n";
          }
      } else {
          std::cout << "Help not available.\n";
      }
    continue; // re-prompt for B/W/C or another H (important!)
    }

    if (ch == 'B' && inv.black > 0) return Stone::B;
    if (ch == 'W' && inv.white > 0) return Stone::W;
    if (ch == 'C' && inv.clear > 0) return Stone::C;

    std::cout << "Not available. Choose again.\n";
  }
}

// ---------- Human implementation ----------

Move Human::chooseMove(Board& board, Coord oppLast) {
  Move m;
  Stone myColor = this->inv().myColor; // human’s actual color
  m.played = readStoneFromInv(this->inv(), board, oppLast, help_, myColor);
  m.pos.r = readIntInRange("Row (1-11): ", 1, 11) - 1;
  m.pos.c = readIntInRange("Col (1-11): ", 1, 11) - 1;
  m.isValid = true;
  return m;
}
