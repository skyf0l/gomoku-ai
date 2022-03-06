#include "random_brain.hpp"

namespace gmk::randbrain {

#define ABOUT_NAME "RandomBrain"
#define ABOUT_VERSION "1.0"
#define ABOUT_AUTHOR "Skyf0l"
#define ABOUT_COUNTRY "FR"
#define ABOUT_WWW "https://github.com/Skyf0l"
#define ABOUT_DESCRIPTION "Random move brain"

static const std::string ABOUT = "name=\"" ABOUT_NAME "\" version=\"" ABOUT_VERSION "\" author=\"" ABOUT_AUTHOR "\" country=\"" ABOUT_COUNTRY
                                 "\" www=\"" ABOUT_WWW "\" description=\"" ABOUT_DESCRIPTION "\"";

RandomBrain::RandomBrain()
    : BrainCore(ABOUT)
{
    srand(time(nullptr));
}

RandomBrain::~RandomBrain()
{
    m_board.clear();
}

bool RandomBrain::isFree(std::uint32_t x, std::uint32_t y)
{
    if (x >= m_board.size() || y >= m_board[x].size())
        return false;
    return m_board[x][y] == EMPTY;
}

bool RandomBrain::resetBoard()
{
    if (m_config.board_width < 5 || m_config.board_height < 5) {
        sendError("board size is 5x5");
        return false;
    }
    m_board.resize(m_config.board_height);
    for (std::uint32_t i = 0; i < m_config.board_height; ++i) {
        m_board[i].resize(m_config.board_width);
        for (std::uint32_t j = 0; j < m_config.board_width; ++j) {
            m_board[i][j] = EMPTY;
        }
    }
    return true;
}

// create the board and call sendOK() or sendError("msg"), return false if error
bool RandomBrain::brainInit()
{
    if (resetBoard()) {
        sendOK();
        return true;
    }
    return false;
}

// delete old board, create new board, call pipeOut("OK"), return false if error
bool RandomBrain::brainRestart()
{
    if (resetBoard()) {
        sendOK();
        return true;
    }
    return false;
}

// callback for info update
void RandomBrain::brainInfo(const InfoType &info)
{
}

// choose your move and call doMyMove(x,y), 0 <= x < width, 0 <= y < height
void RandomBrain::brainTurn()
{
    std::uint32_t x = rand() % m_config.board_width;
    std::uint32_t y = rand() % m_config.board_height;
    while (!isFree(x, y)) {
        x = rand() % m_config.board_width;
        y = rand() % m_config.board_height;
    }
    doMyMove(x, y);
}

// put your move to the board, return true if success
bool RandomBrain::brainMyMove(std::uint32_t x, std::uint32_t y)
{
    if (isFree(x, y)) {
        m_board[x][y] = ME;
        return true;
    }
    sendError("Invalid move");
    return false;
}

// put opponent's move to the board, return true if success
bool RandomBrain::brainOpponentMove(std::uint32_t x, std::uint32_t y)
{
    if (isFree(x, y)) {
        m_board[x][y] = OPPONENT;
        return true;
    }
    sendError("Invalid opponent move");
    return false;
}

// square [x,y] belongs to a winning line (when info_continuous is 1), return true if success
bool RandomBrain::brainBlock(std::uint32_t x, std::uint32_t y)
{
    if (isFree(x, y)) {
        m_board[x][y] = BLOCKED;
        return true;
    }
    sendError("Invalid block");
    return false;
}

// clear one square and call call sendOK() or sendError("msg")
void RandomBrain::brainTakeback(std::uint32_t x, std::uint32_t y)
{
    if (x < m_board.size() && y < m_board[x].size() && !isFree(x, y)) {
        m_board[x][y] = EMPTY;
        sendOK();
    } else {
        sendError("Invalid takeback");
    }
}

// delete temporary files, free resources
void RandomBrain::brainEnd()
{
}
}