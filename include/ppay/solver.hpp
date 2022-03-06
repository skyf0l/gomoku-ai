/**
 * @file solver.hpp
 * @brief Solver algorithm
 */

#ifndef PPAY_SOLVER_HPP
#define PPAY_SOLVER_HPP

#include <vector>
#ifdef __linux__
#include <chrono>
#elif _WIN32
#include <ctime>
#else
#error "Unsupported platform"
#endif

#include "core/brain_core.hpp"

#include "position.hpp"
#include "transposition_table.hpp"

namespace gmk::ppay {

// Retrun a response just few ms before the time is up
#define TIMEOUT_TURN_LIMIT 10

using Move = std::pair<int, int>;

class Solver {
public:
    Solver(int width, int height, uint32_t max_memory, uint32_t maxTime);
    ~Solver();

    void generateMovesOrder(const Position &pos);

    int minimax(const Position &pos, int deep, int alpha, int beta, bool maximizingPlayer);
    Move findBestMove(const Position &pos);

    inline bool isTooFar(const Position &pos, int x, int y)
    {
        return x < pos.getMinX() - m_awayLimit || x > pos.getMaxX() + m_awayLimit || y < pos.getMinY() - m_awayLimit || y > pos.getMaxY() + m_awayLimit;
    }

    inline uint32_t getRemainingTime() const
    {
#ifdef __linux__
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - m_startTurn;

        if (elapsed.count() > m_maxTime - TIMEOUT_TURN_LIMIT) {
            return 0;
        }
        return m_maxTime - TIMEOUT_TURN_LIMIT - elapsed.count();
#elif _WIN32
        std::clock_t end = std::clock();
        uint32_t elapsed = (uint32_t)((end - m_startTurn) / (CLOCKS_PER_SEC / 1000));

        if (elapsed > m_maxTime - TIMEOUT_TURN_LIMIT) {
            return 0;
        }
        return m_maxTime - TIMEOUT_TURN_LIMIT - elapsed;
#endif
    }

    inline void setMaxTime(uint32_t maxTime)
    {
        m_maxTime = std::max(100u, maxTime);
    }

private:
    std::vector<Move> m_moveOrder;
    TranspositionTable<std::uint32_t, int> m_tt;

    int m_width;
    int m_height;

    uint32_t m_maxMemory;

    // in milliseconds
    uint32_t m_maxTime;

#ifdef __linux__
    std::chrono::steady_clock::time_point m_startTurn;
#elif _WIN32
    std::clock_t m_startTurn;
#endif

    int m_depthLimit;
    int m_awayLimit;

    int m_nodeCount;
};
}

#endif /* PPAY_SOLVER_HPP */
