/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 2
* Class:  CMPS 366
* Date:  2025-09-016
*********************************************************************/

#pragma once
#include "player.hpp"
#include "board.hpp"
#include <vector>

class Computer : public Player {
  public:
    explicit Computer(const std::string& name) : Player(name, PlayerKind::Computer) {}

    // File: human.cpp
    // Signature: Human::chooseMove(Board& board, Coord oppLast)
    /* *********************************************************************
    Function Name: chooseMove
    Purpose: Interactively prompt the user for a move (row, column, and stone), supporting help and robust input validation.
    Parameters:
                board, Board&, passed by reference/pointer. Purpose: output via reference
                oppLast, Coord, passed by value. Purpose: board coordinate
    Return Value: Move. See function description for meaning.
    Algorithm:
                1) Perform the documented steps to achieve the stated purpose.
                2) Handle edge-cases per rules.
                3) Return the result or mutate state.
    Reference: None
    ********************************************************************* */
    Move chooseMove(Board& board, Coord lastOpp) override;
    
    /* *********************************************************************
    Function Name: recommendForHuman
    Purpose: Produce a recommended Move (with rationale) for the human player
            using the same evaluation the computer uses for itself.
    Parameters:
      board, a Board (by reference) – current board.
      lastOpp, a Coord (by value) – scopes legal targets.
      humanColor, a Stone (by value) – the human’s color (B or W).
      humanInv, an Inventory (by const reference) – human’s inventory.
    Return Value: Move – the recommended play with rationale populated.
    Algorithm:
      1) Call evaluateBestFor(board, lastOpp, humanColor, humanInv, compColor).
      2) Copy rationale; return the Move.
    Reference: None
    ********************************************************************* */
    Move recommendForHuman(Board& board,
                        Coord lastOpp,
                        Stone humanColor,
                        const Inventory& humanInv);


    // list all human plays for the help screen
    struct ScoredLine {
      Move m;
      int myPts{0};
      int oppBestPts{0};
      int utility{0};
      bool blocked{false};
    };

    // File: computer.hpp
    // Signature: listAllHumanCandidates(Board& board, Coord lastOpp,
    //                                              Stone humanColor, const Inventory& humanInv)
    /* *********************************************************************
    Function Name: listAllHumanCandidates
    Purpose: Lists all of the possible moves the human can take.
    Parameters:
                board, Board&, passed by reference/pointer. Purpose: output via reference
                lastOpp, Coord, passed by value. Purpose: board coordinate
                humanColor, Stone, passed by value. Purpose: input parameter
                humanInv, const Inventory&, passed by reference/pointer. Purpose: input parameter
    Return Value: std::vector<ScoredLine>. See function description for meaning.
    Algorithm:
                1) Perform the documented steps to achieve the stated purpose.
                2) Handle edge-cases per rules.
                3) Return the result or mutate state.
    Reference: None
    ********************************************************************* */
    std::vector<ScoredLine> listAllHumanCandidates(Board& board, Coord lastOpp,
                                                  Stone humanColor, const Inventory& humanInv);
  private:
    struct Scored {
      Move m{};
      int myPts{0};
      int oppBestPts{0};
      int utility{0};
      // will show if this move blocks an immediate threat
      bool blocked{false};  
    };

    /* *********************************************************************
    Function Name: evaluateBestFor
    Purpose: Score all legal (cell, stone) pairs for the actor and pick the best
            according to utility combining myPts, oppBestPts, and blocking.
    Parameters:
      board, a Board (by reference) – current board.
      lastOpp, a Coord (by value) – last opponent move for row/col scope.
      actorColor, a Stone (by value) – mover’s color (B or W).
      actorInv, an Inventory (by const reference) – mover’s inventory.
      opponentColor, a Stone (by value) – opponent color (B or W).
    Return Value: Scored – best move with fields (Move m, myPts, oppBestPts, utility, blocked).
    Algorithm:
      1) Build legal cells and candidate stones from actorInv.
      2) For each (cell, stone):
          a) Temporarily place, compute (myPts, oppPts) via Board::scoreFromPlacement.
          b) Estimate oppBestPts with opponentBestResponsePts.
          c) Compute utility with your net/priority scheme; mark blocked if threat removed.
          d) Store a ScoredLine for help listing.
          e) Undo placement.
      3) Return the argmax by utility, with a human-readable rationale.
    Reference: None
    ********************************************************************* */
    Scored evaluateBestFor(Board& board,
                          Coord lastOpp,
                          Stone actorColor,
                          const Inventory& actorInv,
                          Stone opponentColor);

    /* *********************************************************************
    Function Name: opponentBestResponsePts
    Purpose: For a hypothetical ourMovePos, compute the maximum points the
            opponent could earn with their next play (1-ply lookahead).
    Parameters:
      board, a Board (by reference) – current board (temporarily modified).
      ourMovePos, a Coord (by value) – the pocket we hypothetically fill.
      opponentColor, a Stone (by value) – opponent’s color (B or W).
    Return Value: int – best (maximum) opponent points from one response.
    Algorithm:
      1) For each legal opponent target and each stone candidate in their inventory:
          a) Temporarily place stone, call scoreFromPlacement, record points.
          b) Undo placement.
      2) Return the maximum points observed.
    Reference: None
    ********************************************************************* */
    int opponentBestResponsePts(Board& board, Coord ourMovePos, Stone opponentColor);

    /* *********************************************************************
    Function Name: enumLegalCells
    Purpose: Collect all empty pockets in the opponent’s last row and column,
            unless both are full—then collect all empty valid pockets.
    Parameters:
      board, a Board (by reference) – current board.
      lastOpp, a Coord (by value) – opponent’s last move, or (-1,-1) at start.
      out, a std::vector<Coord> (by reference) – receives legal targets.
    Return Value: none
    Algorithm:
      1) If lastOpp invalid or both its row/col have no empties, scan entire board.
      2) Else, scan row(lastOpp) and col(lastOpp) for empty valid pockets.
    Reference: None
    ********************************************************************* */
    void enumLegalCells(Board& board, Coord lastOpp, std::vector<Coord>& out);
    
    /* *********************************************************************
    Function Name: candidateStones
    Purpose: Enumerate which stones (B/W/C) the actor can still play given inventory.
    Parameters:
      inv, an Inventory (by const reference) – actor’s counts.
      out, a std::vector<Stone> (by reference) – receives available stones.
    Return Value: none
    Algorithm:
      1) If inv.myColorCount > 0 push myColor.
      2) If inv.otherColorCount > 0 push other color (if allowed by rules).
      3) If inv.clearCount > 0 push Stone::C.
    Reference: None
    ********************************************************************* */
    void candidateStones(const Inventory& inv, std::vector<Stone>& out) const;

    /* *********************************************************************
    Function Name: isOpponentThreatAt
    Purpose: Detect whether the opponent could immediately score at pocket p
            (used to mark [blocks] and to prioritize blocks).
    Parameters:
      board, a Board (by reference) – current board.
      p, a Coord (by const reference) – pocket to test.
      opponentColor, a Stone (by value) – opponent’s color (B or W).
    Return Value: bool – true if opponent has a 3-in-a-row opportunity at p.
    Algorithm:
      1) For each applicable 3-pocket window containing p, check if it is
        opponent-or-clear without empties (not all clear).
    Reference: None
    ********************************************************************* */
    bool isOpponentThreatAt(Board& board, const Coord& p, Stone opponentColor);
    
    /* *********************************************************************
    Function Name: rationaleText
    Purpose: Build a one-line human explanation of the chosen move’s reasoning.
    Parameters:
      s, a Scored (by const reference) – fields (myPts, oppBestPts, utility, blocked).
    Return Value: std::string – explanation suitable for UI/help.
    Algorithm:
      1) Compose text including net value and whether it blocks a threat.
    Reference: None
    ********************************************************************* */
    static std::string rationaleText(const Scored& s);
};
