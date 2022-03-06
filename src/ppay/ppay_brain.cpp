#include "ppay/ppay_brain.hpp"

#include <climits>

namespace gmk::ppay {

#define ABOUT_NAME "PPay"
#define ABOUT_VERSION "1.0"
#define ABOUT_AUTHOR "Skyf0l"
#define ABOUT_COUNTRY "FR"
#define ABOUT_WWW "https://github.com/Skyf0l"
#define ABOUT_DESCRIPTION "Pretty Perfect AI"

static const std::string ABOUT = "name=\"" ABOUT_NAME "\" version=\"" ABOUT_VERSION "\" author=\"" ABOUT_AUTHOR "\" country=\"" ABOUT_COUNTRY
                                 "\" www=\"" ABOUT_WWW "\" description=\"" ABOUT_DESCRIPTION "\"";

PPayBrain::PPayBrain()
    : BrainCore(ABOUT)
{
    m_currentPos = nullptr;
    m_solver = nullptr;
}

PPayBrain::~PPayBrain()
{
    if (m_currentPos) {
        delete m_currentPos;
        m_currentPos = nullptr;
    }
    if (m_solver) {
        delete m_solver;
        m_solver = nullptr;
    }
}

bool PPayBrain::resetBoard()
{
    if (m_config.board_width < 5 || m_config.board_height < 5) {
        sendError("board size is 5x5");
        return false;
    }
    if (m_currentPos) {
        delete m_currentPos;
    }
    if (m_solver) {
        delete m_solver;
    }
    m_currentPos = new Position(m_config.board_width, m_config.board_height);
    m_solver = new Solver(m_config.board_width, m_config.board_height, m_config.max_memory, m_config.timeout_turn);
    return true;
}

// create the board and call sendOK() or sendError("msg"), return false if error
bool PPayBrain::brainInit()
{
    if (resetBoard()) {
        sendOK();
        return true;
    }
    return false;
}

// delete old board, create new board and call sendOK(), return false if error
bool PPayBrain::brainRestart()
{
    if (resetBoard()) {
        sendOK();
        return true;
    }
    return false;
}

// callback for info update
void PPayBrain::brainInfo(const InfoType &info)
{
    switch (info) {
    case InfoType::timeout_turn:
        m_solver->setMaxTime(m_config.timeout_turn);
        break;
    default:
        break;
    }
}

// choose your move and call doMyMove(x,y), 0 <= x < width, 0 <= y < height
void PPayBrain::brainTurn()
{
    if (!m_currentPos) {
        sendError("No game in progress");
        return;
    }

    m_currentPos->setIsMyTurn(true);
    Move bestMove = m_solver->findBestMove(*m_currentPos);
    doMyMove(bestMove.first, bestMove.second);
}

bool PPayBrain::isFree(std::uint32_t x, std::uint32_t y)
{
    return x < m_config.board_width && y < m_config.board_height && m_currentPos->canPlay(x, y);
}

// put your move to the board, return true if success
bool PPayBrain::brainMyMove(std::uint32_t x, std::uint32_t y)
{
    if (!m_currentPos) {
        sendError("No game in progress");
        return false;
    }
    if (isFree(x, y)) {
        m_currentPos->play(x, y, true);
        return true;
    }
    sendError("Invalid move");
    return false;
}

// put opponent's move to the board, return true if success
bool PPayBrain::brainOpponentMove(std::uint32_t x, std::uint32_t y)
{
    if (!m_currentPos) {
        sendError("No game in progress");
        return false;
    }
    if (isFree(x, y)) {
        m_currentPos->play(x, y, false);
        return true;
    }
    sendError("Invalid opponent move");
    return false;
}

// square [x,y] belongs to a winning line (when info_continuous is 1), return true if success
bool PPayBrain::brainBlock(std::uint32_t x, std::uint32_t y)
{
    sendError("Not implemented");
    return false;
}

// clear one square and call call sendOK() or sendError("msg")
void PPayBrain::brainTakeback(std::uint32_t x, std::uint32_t y)
{
    if (!m_currentPos) {
        sendError("No game in progress");
        return;
    }
    if (x < m_config.board_width && y < m_config.board_height && !isFree(x, y)) {
        m_currentPos->clear(x, y);
        sendOK();
    } else {
        sendError("Invalid takeback");
    }
}

// delete temporary files, free resources
void PPayBrain::brainEnd()
{
}
}