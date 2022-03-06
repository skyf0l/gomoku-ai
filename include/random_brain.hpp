/**
 * @file randombrain.hpp
 * @brief Random move brain
 */

#ifndef RANDOMBRAIN_HPP
#define RANDOMBRAIN_HPP

#include <vector>

#include "core/brain_core.hpp"

namespace gmk::randbrain {

enum CellState {
    EMPTY,
    ME,
    OPPONENT,
    BLOCKED,
};

typedef std::vector<std::vector<CellState>> Board;

class RandomBrain : public BrainCore {
public:
    RandomBrain();
    ~RandomBrain();

    bool isFree(std::uint32_t x, std::uint32_t y);
    bool resetBoard();

    // needded by BrainCore
    bool brainInit() override;
    bool brainRestart() override;
    void brainInfo(const InfoType &info) override;
    void brainTurn() override;
    bool brainMyMove(std::uint32_t x, std::uint32_t y) override;
    bool brainOpponentMove(std::uint32_t x, std::uint32_t y) override;
    bool brainBlock(std::uint32_t x, std::uint32_t y) override;
    void brainTakeback(std::uint32_t x, std::uint32_t y) override;
    void brainEnd() override;

protected:
    Board m_board;
};

}

#endif /* RANDOMBRAIN_HPP */