/*********************************************************************
* Name:  John Warren
* Project:  3 Stones – Milestone 1
* Class:  CMPS 366
* Date:  2025-09-09
*********************************************************************/

// human.hpp
#pragma once
#include "player.hpp"
#include <functional>
#include <string>

class Human : public Player {
public:
    explicit Human(const std::string& name) : Player(name, PlayerKind::Human) {}

    // A callback Human can invoke when the user presses 'H'
    // Signature: (board, lastOpp, humanColor, humanInventory) -> Move
    using HelpFn = std::function<Move(Board&, Coord, Stone, const Inventory&)>;
    void setHelpCallback(std::function<Move(Board&, Coord, Stone, const Inventory&)> cb) {
        helpCb_ = std::move(cb);
    }

    Move chooseMove(Board& board, Coord oppLast) override;

    const std::string& name() const { return name_; }
    Inventory& inv() { return inv_; }
    const Inventory& inv() const { return inv_; }

private:
    HelpFn help_;
    std::string name_;
    Inventory inv_;
    std::function<Move(Board&, Coord, Stone, const Inventory&)> helpCb_;
};
