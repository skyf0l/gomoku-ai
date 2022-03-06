/**
 * @file position.hpp
 * @brief Position computation
 */

#ifndef PPAY_POSITION_HPP
#define PPAY_POSITION_HPP

#include <algorithm>
#include <iostream>
#include <sstream>

namespace gmk::ppay {

#define LOSE_RATIO 2

static const int heuristicResults[] = {
    /* ..... */ 0,
    /* 1.... */ 1,
    /* 2.... */ LOSE_RATIO * -1,
    /* .1... */ 1,
    /* 11... */ 10,
    /* 21... */ 0,
    /* .2... */ LOSE_RATIO * -1,
    /* 12... */ 0,
    /* 22... */ LOSE_RATIO * -10,
    /* ..1.. */ 1,
    /* 1.1.. */ 10,
    /* 2.1.. */ 0,
    /* .11.. */ 10,
    /* 111.. */ 100,
    /* 211.. */ 0,
    /* .21.. */ 0,
    /* 121.. */ 0,
    /* 221.. */ 0,
    /* ..2.. */ LOSE_RATIO * -1,
    /* 1.2.. */ 0,
    /* 2.2.. */ LOSE_RATIO * -10,
    /* .12.. */ 0,
    /* 112.. */ 0,
    /* 212.. */ 0,
    /* .22.. */ LOSE_RATIO * -10,
    /* 122.. */ 0,
    /* 222.. */ LOSE_RATIO * -100,
    /* ...1. */ 1,
    /* 1..1. */ 10,
    /* 2..1. */ 0,
    /* .1.1. */ 10,
    /* 11.1. */ 100,
    /* 21.1. */ 0,
    /* .2.1. */ 0,
    /* 12.1. */ 0,
    /* 22.1. */ 0,
    /* ..11. */ 10,
    /* 1.11. */ 100,
    /* 2.11. */ 0,
    /* .111. */ 10000,
    /* 1111. */ 1000,
    /* 2111. */ 0,
    /* .211. */ 0,
    /* 1211. */ 0,
    /* 2211. */ 0,
    /* ..21. */ 0,
    /* 1.21. */ 0,
    /* 2.21. */ 0,
    /* .121. */ 0,
    /* 1121. */ 0,
    /* 2121. */ 0,
    /* .221. */ 0,
    /* 1221. */ 0,
    /* 2221. */ 0,
    /* ...2. */ LOSE_RATIO * -1,
    /* 1..2. */ 0,
    /* 2..2. */ LOSE_RATIO * -10,
    /* .1.2. */ 0,
    /* 11.2. */ 0,
    /* 21.2. */ 0,
    /* .2.2. */ LOSE_RATIO * -10,
    /* 12.2. */ 0,
    /* 22.2. */ LOSE_RATIO * -100,
    /* ..12. */ 0,
    /* 1.12. */ 0,
    /* 2.12. */ 0,
    /* .112. */ 0,
    /* 1112. */ 0,
    /* 2112. */ 0,
    /* .212. */ 0,
    /* 1212. */ 0,
    /* 2212. */ 0,
    /* ..22. */ LOSE_RATIO * -10,
    /* 1.22. */ 0,
    /* 2.22. */ LOSE_RATIO * -100,
    /* .122. */ 0,
    /* 1122. */ 0,
    /* 2122. */ 0,
    /* .222. */ LOSE_RATIO * -10000,
    /* 1222. */ 0,
    /* 2222. */ LOSE_RATIO * -1000,
    /* ....1 */ 1,
    /* 1...1 */ 10,
    /* 2...1 */ 0,
    /* .1..1 */ 10,
    /* 11..1 */ 100,
    /* 21..1 */ 0,
    /* .2..1 */ 0,
    /* 12..1 */ 0,
    /* 22..1 */ 0,
    /* ..1.1 */ 10,
    /* 1.1.1 */ 100,
    /* 2.1.1 */ 0,
    /* .11.1 */ 100,
    /* 111.1 */ 1000,
    /* 211.1 */ 0,
    /* .21.1 */ 0,
    /* 121.1 */ 0,
    /* 221.1 */ 0,
    /* ..2.1 */ 0,
    /* 1.2.1 */ 0,
    /* 2.2.1 */ 0,
    /* .12.1 */ 0,
    /* 112.1 */ 0,
    /* 212.1 */ 0,
    /* .22.1 */ 0,
    /* 122.1 */ 0,
    /* 222.1 */ 0,
    /* ...11 */ 10,
    /* 1..11 */ 100,
    /* 2..11 */ 0,
    /* .1.11 */ 100,
    /* 11.11 */ 1000,
    /* 21.11 */ 0,
    /* .2.11 */ 0,
    /* 12.11 */ 0,
    /* 22.11 */ 0,
    /* ..111 */ 100,
    /* 1.111 */ 1000,
    /* 2.111 */ 0,
    /* .1111 */ 1000,
    /* 11111 */ 1000000,
    /* 21111 */ 0,
    /* .2111 */ 0,
    /* 12111 */ 0,
    /* 22111 */ 0,
    /* ..211 */ 0,
    /* 1.211 */ 0,
    /* 2.211 */ 0,
    /* .1211 */ 0,
    /* 11211 */ 0,
    /* 21211 */ 0,
    /* .2211 */ 0,
    /* 12211 */ 0,
    /* 22211 */ 0,
    /* ...21 */ 0,
    /* 1..21 */ 0,
    /* 2..21 */ 0,
    /* .1.21 */ 0,
    /* 11.21 */ 0,
    /* 21.21 */ 0,
    /* .2.21 */ 0,
    /* 12.21 */ 0,
    /* 22.21 */ 0,
    /* ..121 */ 0,
    /* 1.121 */ 0,
    /* 2.121 */ 0,
    /* .1121 */ 0,
    /* 11121 */ 0,
    /* 21121 */ 0,
    /* .2121 */ 0,
    /* 12121 */ 0,
    /* 22121 */ 0,
    /* ..221 */ 0,
    /* 1.221 */ 0,
    /* 2.221 */ 0,
    /* .1221 */ 0,
    /* 11221 */ 0,
    /* 21221 */ 0,
    /* .2221 */ 0,
    /* 12221 */ 0,
    /* 22221 */ 0,
    /* ....2 */ LOSE_RATIO * -1,
    /* 1...2 */ 0,
    /* 2...2 */ LOSE_RATIO * -10,
    /* .1..2 */ 0,
    /* 11..2 */ 0,
    /* 21..2 */ 0,
    /* .2..2 */ LOSE_RATIO * -10,
    /* 12..2 */ 0,
    /* 22..2 */ LOSE_RATIO * -100,
    /* ..1.2 */ 0,
    /* 1.1.2 */ 0,
    /* 2.1.2 */ 0,
    /* .11.2 */ 0,
    /* 111.2 */ 0,
    /* 211.2 */ 0,
    /* .21.2 */ 0,
    /* 121.2 */ 0,
    /* 221.2 */ 0,
    /* ..2.2 */ LOSE_RATIO * -10,
    /* 1.2.2 */ 0,
    /* 2.2.2 */ LOSE_RATIO * -100,
    /* .12.2 */ 0,
    /* 112.2 */ 0,
    /* 212.2 */ 0,
    /* .22.2 */ LOSE_RATIO * -100,
    /* 122.2 */ 0,
    /* 222.2 */ LOSE_RATIO * -1000,
    /* ...12 */ 0,
    /* 1..12 */ 0,
    /* 2..12 */ 0,
    /* .1.12 */ 0,
    /* 11.12 */ 0,
    /* 21.12 */ 0,
    /* .2.12 */ 0,
    /* 12.12 */ 0,
    /* 22.12 */ 0,
    /* ..112 */ 0,
    /* 1.112 */ 0,
    /* 2.112 */ 0,
    /* .1112 */ 0,
    /* 11112 */ 0,
    /* 21112 */ 0,
    /* .2112 */ 0,
    /* 12112 */ 0,
    /* 22112 */ 0,
    /* ..212 */ 0,
    /* 1.212 */ 0,
    /* 2.212 */ 0,
    /* .1212 */ 0,
    /* 11212 */ 0,
    /* 21212 */ 0,
    /* .2212 */ 0,
    /* 12212 */ 0,
    /* 22212 */ 0,
    /* ...22 */ LOSE_RATIO * -10,
    /* 1..22 */ 0,
    /* 2..22 */ LOSE_RATIO * -100,
    /* .1.22 */ 0,
    /* 11.22 */ 0,
    /* 21.22 */ 0,
    /* .2.22 */ LOSE_RATIO * -100,
    /* 12.22 */ 0,
    /* 22.22 */ LOSE_RATIO * -1000,
    /* ..122 */ 0,
    /* 1.122 */ 0,
    /* 2.122 */ 0,
    /* .1122 */ 0,
    /* 11122 */ 0,
    /* 21122 */ 0,
    /* .2122 */ 0,
    /* 12122 */ 0,
    /* 22122 */ 0,
    /* ..222 */ LOSE_RATIO * -100,
    /* 1.222 */ 0,
    /* 2.222 */ LOSE_RATIO * -1000,
    /* .1222 */ 0,
    /* 11222 */ 0,
    /* 21222 */ 0,
    /* .2222 */ LOSE_RATIO * -1000,
    /* 12222 */ 0,
    /* 22222 */ LOSE_RATIO * -1000000,
};

class Position {
public:
    /**
     * Default constructor, build an empty position.
     */
    Position(int width, int height)
        : m_width { width }
        , m_height { height }
        , m_nbCells { width * height }
        , m_minX { width - 1 }
        , m_minY { height - 1 }
        , m_maxX { 0 }
        , m_maxY { 0 }
        , m_nbMoves { 0 }
        , m_isMyTurn { true }
    {
        // init int **m_board with 0
        m_board = new int[m_nbCells];
        std::fill(m_board, m_board + m_nbCells, 0);
    }

    /**
     * Destructor, delete the board.
     */
    ~Position()
    {
        delete[] m_board;
    }

    /**
     * Copy constructor.
     */
    Position(const Position &other)
        : m_width { other.m_width }
        , m_height { other.m_height }
        , m_nbCells { other.m_nbCells }
        , m_minX { other.m_minX }
        , m_minY { other.m_minY }
        , m_maxX { other.m_maxX }
        , m_maxY { other.m_maxY }
        , m_nbMoves { other.m_nbMoves }
        , m_isMyTurn { other.m_isMyTurn }

    {
        // init int **m_board with 0
        m_board = new int[m_nbCells];
        std::copy(other.m_board, other.m_board + m_nbCells, m_board);
    }

    /**
     * Indicates whether a column is playable.
     *
     * @param x: 0-based index of column to play.
     * @param y: 0-based index of row to play.
     * @return true if the move is playable, false if the move is already full.
     */
    inline bool canPlay(int x, int y) const
    {
        return !m_board[x + y * m_width];
    }

    /**
     * Plays a playable move.
     * This function should not be called on a non-playable move or a move making a win.
     *
     * @param col: 0-based index of a playable column.
     */
    inline void play(int x, int y)
    {
        m_board[x + y * m_width] = m_isMyTurn ? 1 : 2;
        m_nbMoves++;
        m_isMyTurn = !m_isMyTurn;

        m_minX = std::min(m_minX, x);
        m_minY = std::min(m_minY, y);
        m_maxX = std::max(m_maxX, x);
        m_maxY = std::max(m_maxY, y);
    }

    /**
     * Plays a playable move.
     * This function should not be called on a non-playable move or a move making a win.
     *
     * @param col: 0-based index of a playable column.
     */
    inline void play(int x, int y, int isMe)
    {
        m_board[x + y * m_width] = isMe ? 1 : 2;
        m_nbMoves++;
        m_isMyTurn = !isMe;

        m_minX = std::min(m_minX, x);
        m_minY = std::min(m_minY, y);
        m_maxX = std::max(m_maxX, x);
        m_maxY = std::max(m_maxY, y);
    }

    /**
     * Clear one move.
     * This function should not be called on a empty move.
     *
     * @param x: 0-based index of column to clear.
     * @param y: 0-based index of row to clear.
     */
    inline void clear(int x, int y)
    {
        m_board[x + y * m_width] = 0;
        m_nbMoves--;
        m_isMyTurn = !m_isMyTurn;
    }

    /**
     * Indicates whether the current player wins by playing a given move.
     * This function should never be called on a non-playable move.
     *
     * @param x: 0-based index of a playable column.
     * @param y: 0-based index of a playable row.
     * @return true if current player makes an alignment by playing the corresponding column col.
     */

    bool isWinningMove(int x, int y) const
    {
        return isWinningMove(x, y, m_isMyTurn);
    }

    /**
     * Indicates if a specific player wins by playing a given move.
     * This function should never be called on a non-playable move.
     *
     * @param x: 0-based index of a playable column.
     * @param y: 0-based index of a playable row.
     * @param isMyTurn:
     * @return true if current player makes an alignment by playing the corresponding column col.
     */
    bool isWinningMove(int x, int y, bool isMyTurn) const
    {
        // win if 5 in a row / column / diagonal
        int currentPlayer = isMyTurn ? 1 : 2;
        int count;

        // check horizontal
        for (int x_ = std::max(0, x - 4); x_ < std::min(m_width - 4, x + 1); x_++) {
            count = 0;
            for (int i = 0; i < 5; i++)
                if (x_ + i == x || m_board[x_ + i + y * m_width] == currentPlayer)
                    count++;

            if (count == 5)
                return true;
        }

        // check vertical
        for (int y_ = std::max(0, y - 4); y_ < std::min(m_height - 4, y + 1); y_++) {
            count = 0;
            for (int i = 0; i < 5; i++)
                if (y_ + i == y || m_board[x + (y_ + i) * m_width] == currentPlayer)
                    count++;

            if (count == 5)
                return true;

            for (int x_ = std::max(0, x - 4); x_ < std::min(m_width - 4, x + 1); x_++) {
                // check diagonal
                count = 0;
                for (int i = 0; i < 5; i++)
                    if (x_ + i == x && y_ + i == y || m_board[x_ + i + (y_ + i) * m_width] == currentPlayer)
                        count++;

                if (count == 5)
                    return true;

                // check anti-diagonal
                count = 0;
                for (int i = 0; i < 5; i++)
                    if (x_ + i == x && y_ + 4 - i == y || m_board[x_ + i + (y_ + 4 - i) * m_width] == currentPlayer)
                        count++;

                if (count == 5)
                    return true;
            }
        }

        return false;
    }

    /**
     * Calculates the heuristic score of the current position.
     *
     * @return the heuristic score of the current position.
     */
    int heuristic() const
    {
        int score = 0;

        int c2 = 0;
        int c3 = 0;
        int c4 = 0;
        int c5 = 0;

        int *board = m_board;
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                if (x < m_height - 4) {
                    c2 = board[1];
                    c3 = board[2];
                    c4 = board[3];
                    c5 = board[4];
                    score += heuristicResults[*board + c2 * 3 + c3 * 9 + c4 * 27 + c5 * 81];
                }

                if (y < m_width - 4) {
                    c2 = board[m_width];
                    c3 = board[2 * m_width];
                    c4 = board[3 * m_width];
                    c5 = board[4 * m_width];
                    score += heuristicResults[*board + c2 * 3 + c3 * 9 + c4 * 27 + c5 * 81];

                    if (x < m_height - 4) {
                        c2 = board[1 + m_width];
                        c3 = board[2 + 2 * m_width];
                        c4 = board[3 + 3 * m_width];
                        c5 = board[4 + 4 * m_width];
                        score += heuristicResults[*board + c2 * 3 + c3 * 9 + c4 * 27 + c5 * 81];
                    }

                    if (x > 3) {
                        c2 = board[-1 + m_width];
                        c3 = board[-2 + (2) * m_width];
                        c4 = board[-3 + (3) * m_width];
                        c5 = board[-4 + (4) * m_width];
                        score += heuristicResults[*board + c2 * 3 + c3 * 9 + c4 * 27 + c5 * 81];
                    }
                }
                ++board;
            }
        }

        return score;
    }

    /**
     * Indicates the state of a given move.
     *
     * @param x: 0-based index of a column.
     * @param y: 0-based index of a row.
     * @return 0 if the move is empty, 1 if the move is played by the current player, 2 if the move is played by the
     * other player.
     */
    inline int getState(int x, int y) const
    {
        return m_board[x + y * m_width];
    }

    /**
     * = operator.
     */
    Position &operator=(const Position &other)
    {
        if (this != &other) {
            m_width = other.m_width;
            m_height = other.m_height;
            m_nbCells = other.m_nbCells;
            m_minX = other.m_minX;
            m_maxX = other.m_maxX;
            m_minY = other.m_minY;
            m_maxY = other.m_maxY;
            m_nbMoves = other.m_nbMoves;
            m_isMyTurn = other.m_isMyTurn;
            m_hash = other.m_hash;

            delete[] m_board;
            m_board = new int[m_nbCells];
            std::copy(other.m_board, other.m_board + m_nbCells, m_board);
        }
        return *this;
    }

    /**
     * == operator.
     */
    bool operator==(const Position &other) const
    {
        if (m_width != other.m_width || m_height != other.m_height || m_nbCells != other.m_nbCells || m_nbMoves != other.m_nbMoves
            || m_isMyTurn != other.m_isMyTurn) {
            return false;
        }
        for (int i = 0; i < m_nbCells; i++) {
            if (m_board[i] != other.m_board[i]) {
                return false;
            }
        }
        return true;
    }

    /**
     * != operator.
     */
    bool operator!=(const Position &other) const
    {
        return !(*this == other);
    }

    /**
     * Vertical flip
     *
     * Example of a position:
     *
     * 0 1 2
     * 3 4 5
     * 6 7 8
     *
     * Result:
     *
     * 6 7 8
     * 3 4 5
     * 0 1 2
     */
    Position verticalFlip() const
    {
        Position new_position(*this);
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                new_position.m_board[x + (m_height - y - 1) * m_width] = m_board[x + y * m_width];
            }
        }
        return new_position;
    }

    /**
     * Horizontal flip
     *
     * Example of a position:
     *
     * 0 1 2
     * 3 4 5
     * 6 7 8
     *
     * Result:
     *
     * 2 1 0
     * 5 4 3
     * 8 7 6
     */
    Position horizontalFlip() const
    {
        Position new_position(*this);
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                new_position.m_board[(m_width - x - 1) + y * m_width] = m_board[x + y * m_width];
            }
        }
        return new_position;
    }

    /**
     * Diagonal flip
     *
     * Example of a position:
     *
     * 0 1 2
     * 3 4 5
     * 6 7 8
     *
     * Result:
     *
     * 0 3 6
     * 1 4 7
     * 2 5 8
     */
    Position diagonalFlip() const
    {
        Position new_position(*this);
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                new_position.m_board[y + x * m_width] = m_board[x + y * m_width];
            }
        }
        return new_position;
    }

    /**
     * Anti-diagonal flip
     *
     * Example of a position:
     *
     * 0 1 2
     * 3 4 5
     * 6 7 8
     *
     * Result:
     *
     * 8 5 2
     * 7 4 1
     * 6 3 0
     */
    Position antiDiagonalFlip() const
    {
        Position new_position(*this);
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                new_position.m_board[(m_height - y - 1) + (m_width - x - 1) * m_width] = m_board[x + y * m_width];
            }
        }
        return new_position;
    }

    /**
     * Comparison for map key.
     * Two maps symmetric (horizontal, vertical, diagonal, anti-diagonal and rotated) are equal because they are the
     * same absolute position.
     */
    uint64_t hash() const
    {
        std::uint32_t hash = simple_hash();

        Position vertical(*this);
        Position horizontal(*this);
        Position diagonal(*this);
        Position antiDiagonal(*this);

        int *board = m_board;
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                vertical.m_board[x + (m_height - y - 1) * m_width] = *board;
                horizontal.m_board[(m_width - x - 1) + y * m_width] = *board;
                diagonal.m_board[y + x * m_width] = *board;
                antiDiagonal.m_board[(m_height - y - 1) + (m_width - x - 1) * m_width] = *board;
                ++board;
            }
        }

        // hash ^= verticalFlip().simple_hash();
        hash ^= vertical.simple_hash();
        // hash ^= horizontalFlip().simple_hash();
        hash ^= horizontal.simple_hash();

        // hash ^= diagonalFlip().simple_hash();
        hash ^= diagonal.simple_hash();
        // hash ^= antiDiagonalFlip().simple_hash();
        hash ^= antiDiagonal.simple_hash();

        board = vertical.m_board;
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                horizontal.m_board[(m_width - x - 1) + y * m_width] = *board;
                diagonal.m_board[y + x * m_width] = *board;
                antiDiagonal.m_board[(m_height - y - 1) + (m_width - x - 1) * m_width] = *board;
                ++board;
            }
        }

        // hash ^= verticalFlip().horizontalFlip().simple_hash();
        hash ^= horizontal.simple_hash();
        // hash ^= verticalFlip().diagonalFlip().simple_hash();
        hash ^= diagonal.simple_hash();
        // hash ^= verticalFlip().antiDiagonalFlip().simple_hash();
        hash ^= antiDiagonal.simple_hash();

        return hash;
    }

    inline uint64_t simple_hash() const
    {
        uint64_t hash = 0;

        int nbCells = m_nbCells;
        int *board = m_board;
        while (nbCells) {
            hash ^= hash * 0xc0fe + *board;
            ++board;
            --nbCells;
        }
        return hash;
    }

    /**
     * To string
     *
     * @return string representation of the position
     */
    std::string toString() const
    {
        std::stringstream ss;
        for (int y = 0; y < m_height; y++) {
            if (y > 0)
                ss << std::endl;
            for (int x = 0; x < m_width; x++)
                ss << m_board[x + y * m_width];
        }
        return ss.str();
    }

    ////////////////////////////////////////////////////////////////////////////////

    inline int getWidth() const
    {
        return m_width;
    }

    inline int getHeight() const
    {
        return m_height;
    }

    inline int getNbCells() const
    {
        return m_nbCells;
    }

    inline int getNbMoves() const
    {
        return m_nbMoves;
    }

    inline int getMinX() const
    {
        return m_minX;
    }

    inline int getMinY() const
    {
        return m_minY;
    }

    inline int getMaxX() const
    {
        return m_maxX;
    }

    inline int getMaxY() const
    {
        return m_maxY;
    }

    inline bool isMyTurn() const
    {
        return m_isMyTurn;
    }

    inline void setIsMyTurn(bool isMyTurn)
    {
        m_isMyTurn = isMyTurn;
    }

private:
    // width of the board
    int m_width;
    // height of the board
    int m_height;
    // total number of cells
    int m_nbCells;
    // board (0 = empty, 1 = me, 2 = opponent)
    int *m_board;

    // min x of the board
    int m_minX;
    // min y of the board
    int m_minY;
    // max x of the board
    int m_maxX;
    // max y of the board
    int m_maxY;

    // number of moves played from the beginning of the game
    int m_nbMoves;
    // current player (true = me, false = opponent)
    bool m_isMyTurn;

    // hash optimization
    uint64_t m_hash;
};
}

#endif /* PPAY_POSITION_HPP */