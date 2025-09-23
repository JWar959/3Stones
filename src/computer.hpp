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
    
    // File: computer.hpp
    // Signature: recommendForHuman(Board& board,
    //                    Coord lastOpp,
    //                    Stone humanColor,
    //                    const Inventory& humanInv)
    /* *********************************************************************
    Function Name: recommendForHuman
    Purpose: Recommend a move to the human player based off of their stone's color as well as
            the positions of the current stones on the board.
    Parameters:
                board, Board&, passed by reference/pointer. Purpose: output via reference
                lastOpp, Coord, passed by value. Purpose: board coordinate
                humanColor, Stone, passed by value. Purpose: input parameter
                humanInv, const Inventory&, passed by reference/pointer. Purpose: input parameter
    Return Value: Move. See function description for meaning.
    Algorithm:
                1) Perform the documented steps to achieve the stated purpose.
                2) Handle edge-cases per rules.
                3) Return the result or mutate state.
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

    // File: computer.hpp
    // Signature: evaluateBestFor(Board& board,
    //                      Coord lastOpp,
    //                      Stone actorColor,
    //                      const Inventory& actorInv,
    //                      Stone opponentColor)
    /* *********************************************************************
    Function Name: evaluateBestFor
    Purpose: Score a candidate move: immediate points for us, opponent's best reply, and net utility.
    Parameters:
                board, Board&, passed by reference/pointer. Purpose: output via reference
                lastOpp, Coord, passed by value. Purpose: board coordinate
                actorColor, Stone, passed by value. Purpose: input parameter
                actorInv, const Inventory&, passed by reference/pointer. Purpose: input parameter
                opponentColor, Stone, passed by value. Purpose: input parameter
    Return Value: Scored. See function description for meaning.
    Algorithm:
                1) Perform the documented steps to achieve the stated purpose.
                2) Handle edge-cases per rules.
                3) Return the result or mutate state.
    Reference: None
    ********************************************************************* */
    Scored evaluateBestFor(Board& board,
                          Coord lastOpp,
                          Stone actorColor,
                          const Inventory& actorInv,
                          Stone opponentColor);

    // File: computer.hpp
    // Signature: opponentBestResponsePts(Board& board, Coord ourMovePos, Stone opponentColor)
    /* *********************************************************************
    Function Name: opponentBestResponsePts
    Purpose: Compute the best points the opponent can earn in reply to our tentative move.
    Parameters:
                board, Board&, passed by reference/pointer. Purpose: output via reference
                ourMovePos, Coord, passed by value. Purpose: board coordinate
                opponentColor, Stone, passed by value. Purpose: input parameter
    Return Value: int. Count, index, points, or status code as described.
    Algorithm:
                1) Perform the documented steps to achieve the stated purpose.
                2) Handle edge-cases per rules.
                3) Return the result or mutate state.
    Reference: None
    ********************************************************************* */
    int opponentBestResponsePts(Board& board, Coord ourMovePos, Stone opponentColor);

    // File: computer.hpp
    // Signature: enumLegalCells(Board& board, Coord lastOpp, std::vector<Coord>& out)
    /* *********************************************************************
    Function Name: enumLegalCells
    Purpose: Enumerate all legal pockets given last opponent coordinate (row or column constraints).
    Parameters:
                board, Board&, passed by reference/pointer. Purpose: output via reference
                lastOpp, Coord, passed by value. Purpose: board coordinate
                out, std::vector<Coord>&, passed by reference/pointer. Purpose: board coordinate
    Return Value: void. Performs side effects only.
    Algorithm:
                1) Perform the documented steps to achieve the stated purpose.
                2) Handle edge-cases per rules.
                3) Return the result or mutate state.
    Reference: None
    ********************************************************************* */
    void enumLegalCells(Board& board, Coord lastOpp, std::vector<Coord>& out);
    
    // File: computer.hpp
    // Signature: candidateStones(const Inventory& inv, std::vector<Stone>& out)
    /* *********************************************************************
    Function Name: candidateStones
    Purpose: Predicate: return true if the condition holds.
    Parameters:
                inv, const Inventory&, passed by reference/pointer. Purpose: input parameter
                out, std::vector<Stone>&, passed by reference/pointer. Purpose: output via reference
    Return Value: void. Performs side effects only.
    Algorithm:
                1) Check inputs.
                2) Evaluate the condition in constant time.
                3) Return the boolean result.
    Reference: None
    ********************************************************************* */
    void candidateStones(const Inventory& inv, std::vector<Stone>& out) const;

    // File: computer.hpp
    // Signature: isOpponentThreatAt(Board& board, const Coord& p, Stone opponentColor)
    /* *********************************************************************
    Function Name: isOpponentThreatAt
    Purpose: Detect whether placing a stone blocks an opponent 3-in-a-row threat.
    Parameters:
                board, Board&, passed by reference/pointer. Purpose: output via reference
                p, const Coord&, passed by reference/pointer. Purpose: board coordinate
                opponentColor, Stone, passed by value. Purpose: input parameter
    Return Value: bool. True if the stated condition holds; false otherwise.
    Algorithm:
                1) Check inputs.
                2) Evaluate the condition in constant time.
                3) Return the boolean result.
    Reference: None
    ********************************************************************* */
    bool isOpponentThreatAt(Board& board, const Coord& p, Stone opponentColor);
    
    // File: computer.hpp
    // Signature: rationaleText(const Scored& s)
    /* *********************************************************************
    Function Name: rationaleText
    Purpose: Implements a cohesive step of the 3 Stones gameplay in this module.
    Parameters:
                s, const Scored&, passed by reference/pointer. Purpose: input parameter
    Return Value: std::string. Human-readable message or status.
    Algorithm:
                1) Perform the documented steps to achieve the stated purpose.
                2) Handle edge-cases per rules.
                3) Return the result or mutate state.
    Reference: None
    ********************************************************************* */
    static std::string rationaleText(const Scored& s);
};
