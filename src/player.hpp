/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Final
* Class:  CMPS 366
* Date:  9-23-2025
*********************************************************************/

#pragma once
#include "types.hpp"
#include "move.hpp"
//#include "board.hpp"
#include <string>


class Board;

enum class PlayerKind { Human, Computer };

struct Inventory {
  int black{15};
  int white{15};
  int clear{6};
  int points{0};
  int roundsWon{0};
  // assigned after coin toss & color selection
  Stone myColor{Stone::B};

  // adding a function here to help set these values when loading in a saved game from a file
  Inventory() = default;
  Inventory(Stone color, int pts, int nb, int nw, int nc, int rw)
    : black(nb), white(nw), clear(nc), points(pts), roundsWon(rw), myColor(color) {}
};

class Player {
public:

  /* *********************************************************************
  Function Name: Player (constructor)
  Purpose: Construct a player (human or computer) with a name and kind;
          initialize inventory counters.
  Parameters:
    name, a std::string (by const reference) – player’s display name.
    kind, a PlayerKind (by value) – Human or Computer.
  Return Value: none
  Reference: None
  ********************************************************************* */
  Player(std::string name, PlayerKind kind) : name_(std::move(name)), kind_(kind) {}
  virtual ~Player() = default;
  virtual Move chooseMove(Board& board, Coord lastOpp) = 0;

  /* *********************************************************************
  Function Name: name
  Purpose: Selector – read-only access to player’s name.
  Parameters: none
  Return Value: const std::string& – player name.
  Reference: None
  ********************************************************************* */
  const std::string& name() const { return name_; }

  /* *********************************************************************
  Function Name: kind
  Purpose: Selector – read-only access to player type.
  Parameters: none
  Return Value: PlayerKind – Human or Computer.
  Reference: None
  ********************************************************************* */
  PlayerKind kind() const { return kind_; }

  /* *********************************************************************
  Function Name: inv (const / non-const)
  Purpose: Selector/Mutator access to the player’s inventory (stones, points).
  Parameters: none
  Return Value:
    const Inventory& / Inventory& – reference to inventory.
  Reference: None
  ********************************************************************* */
  const Inventory& inv() const { return inv_; }
  Inventory& inv() { return inv_; }

  /* *********************************************************************
  Function Name: setInventory
  Purpose: Replace the player’s inventory atomically (used by deserialization).
  Parameters:
    in, an Inventory (by const reference) – replacement inventory values.
  Return Value: none
  Reference: None
  ********************************************************************* */
  void setInventory(const Inventory& in) { inv_ = in; }

protected:
  std::string name_;
  PlayerKind kind_;
  Inventory inv_;
};
