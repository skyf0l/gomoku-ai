#include <climits>
#include <iostream>

#include "ppay/position.hpp"
#include "ppay/solver.hpp"
#include "ppay/transposition_table.hpp"

namespace gmk::ppay {

Solver::Solver(int width, int height, uint32_t max_memory, uint32_t maxTime)
    : m_tt(500000)
    , m_width(width)
    , m_height(height)
    , m_maxMemory(max_memory)
    , m_maxTime(maxTime)
    , m_depthLimit(2)
    , m_awayLimit(3)
{
}

Solver::~Solver()
{
}

void Solver::generateMovesOrder(const Position &pos)
{
    m_moveOrder.clear();

    // from center to corners
    for (int y = 0; y < m_height; ++y) {
        int _y = m_height / 2 + (1 - 2 * (y % 2)) * (y + 1) / 2;
        for (int x = 0; x < m_width; ++x) {
            int _x = m_width / 2 + (1 - 2 * (x % 2)) * (x + 1) / 2;
            if (pos.canPlay(_x, _y)) {
                m_moveOrder.push_back(Move(_x, _y));
            }
        }
    }
}

int Solver::minimax(const Position &pos, int deep, int alpha, int beta, bool maximizingPlayer)
{
    // if depth limit is reached, return the heuristic value
    if (deep == 0)
        return pos.heuristic();

    // max time is reached, return the heuristic value
    if (getRemainingTime() == 0)
        return pos.heuristic();

    // if the position is in the transposition table, return the value
    int score;
    if (m_tt.get(pos.hash(), score))
        return score;

    // if there is more than 6 moves on board, check if there is a winning move
    if (pos.getNbMoves() > 6) {
        Move loosingMove = Move(-1, -1);
        int loosingMoveCount = 0;

        for (const auto &move : m_moveOrder) {
            int x = move.first;
            int y = move.second;

            // play only moves that are not too far away
            if (isTooFar(pos, x, y))
                continue;

            if (pos.canPlay(x, y)) {
                // if we have a winning move, play it
                if (pos.isWinningMove(x, y)) {
                    score = maximizingPlayer ? INT_MAX - 1 : INT_MIN + 1;
                    // store the value in the transposition table
                    m_tt.set(pos.hash(), score);
                    return score;
                }

                // if there is a losing move, save it
                if (pos.isWinningMove(x, y, false)) {
                    loosingMove = Move(x, y);
                    loosingMoveCount++;
                }
            }
        }

        // if no winning move are found, we should block loosing move
        if (loosingMoveCount == 1) {
            Position newPos(pos);
            newPos.play(loosingMove.first, loosingMove.second);

            // calculate the score
            return minimax(newPos, deep - 1, alpha, beta, !maximizingPlayer);
        }
        // if there is more than one loosing move, it's impossible to counter both, it's a instant loose
        if (loosingMoveCount >= 2) {
            score = maximizingPlayer ? INT_MIN + 1 : INT_MAX - 1;
            // store the value in the transposition table
            m_tt.set(pos.hash(), score);
            return score;
        }
    }

    // if we are in the maximizing player's turn, find the best move
    // else, find the worst move
    int bestScore = maximizingPlayer ? INT_MIN + 1 : INT_MAX - 1;

    // calculate the score for each move
    for (const auto &move : m_moveOrder) {
        int x = move.first;
        int y = move.second;

        // play only moves that are not too far away
        if (isTooFar(pos, x, y))
            continue;

        // ignore moves that are not playable
        if (pos.canPlay(x, y)) {
            // play move in new board
            Position newPos(pos);
            newPos.play(x, y);

            // calculate the score
            score = minimax(newPos, deep - 1, alpha, beta, !maximizingPlayer);

            // update the best score
            if (maximizingPlayer) {
                bestScore = std::max(bestScore, score);
                alpha = std::max(alpha, score);
            } else {
                bestScore = std::min(bestScore, score);
                beta = std::min(beta, score);
            }
            // if the beta cut-off is reached, return the best score
            if (beta <= alpha)
                break;
        }
    }

    // store the best score in the transposition table
    m_tt.set(pos.hash(), bestScore);
    return bestScore;
}

Move Solver::findBestMove(const Position &pos)
{
    // first move is always at center
    if (pos.getNbMoves() == 0)
        return std::make_pair(m_width / 2, m_height / 2);

    // start chronometer    
#ifdef __linux__
    m_startTurn = std::chrono::steady_clock::now();
#elif _WIN32
    m_startTurn = std::clock();
#endif

    // reset transposition table and move order
    m_nodeCount = 0;
    m_tt.clear();
    generateMovesOrder(pos);

    // if there is more than 6 moves on board, check if there is a winning move
    if (pos.getNbMoves() > 6) {
        Move loosingMove = Move(-1, -1);
        for (const auto &move : m_moveOrder) {
            int x = move.first;
            int y = move.second;

            // play only moves that are not too far away
            if (isTooFar(pos, x, y))
                continue;

            if (pos.canPlay(x, y)) {
                // if we have a winning move, play it
                if (pos.isWinningMove(x, y))
                    return Move(x, y);

                // if there is a losing move, save it
                if (pos.isWinningMove(x, y, false))
                    loosingMove = Move(x, y);
            }
        }
        // if no winning move are found, we should block loosing move
        if (loosingMove.first != -1)
            return loosingMove;
    }
    // else, play the best move
    int bestScore = INT_MIN;
    Move bestMove = Move(0, 0);

    for (const auto &move : m_moveOrder) {
        int x = move.first;
        int y = move.second;

        // play only moves that are not too far away
        if (isTooFar(pos, x, y))
            continue;

        // ignore moves that are not playable
        if (pos.canPlay(x, y)) {
            // play move in new board
            Position nextPos(pos);
            nextPos.play(x, y);

            // calculate score
            int score = minimax(nextPos, m_depthLimit, INT_MIN, INT_MAX, false);

            // update best move
            if (score > bestScore) {
                bestScore = score;
                bestMove = Move(x, y);
            }
        }
    }

    return bestMove;
}
}
