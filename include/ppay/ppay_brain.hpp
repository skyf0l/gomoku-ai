/**
 * @file ppay_brain.hpp
 * @brief Random move brain
 */

#ifndef PPAY_PPAY_BRAIN_HPP
#define PPAY_PPAY_BRAIN_HPP

#include <vector>

#include "core/brain_core.hpp"

#include "position.hpp"
#include "solver.hpp"

namespace gmk::ppay {

class PPayBrain : public BrainCore {
public:
    PPayBrain();
    ~PPayBrain();

    bool isFree(std::uint32_t x, std::uint32_t y);
    bool resetBoard();

    // int negamax(const Position &pos, int deep, int alpha, int beta);
    Move find_best_move(const Position &pos);

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
    Position *m_currentPos;

    Solver *m_solver;
};
}

#endif /* PPAY_PPAY_BRAIN_HPP */