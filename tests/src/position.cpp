#include <gtest/gtest.h>

#include "ppay/position.hpp"

namespace gmk::ppay {

void randomFill(Position &pos)
{
    for (int i = 0; i < pos.getNbCells(); i++)
        if (pos.canPlay(i % pos.getWidth(), i / pos.getWidth()))
            pos.play(rand() % pos.getWidth(), rand() % pos.getHeight());
}

TEST(Position, EmptyBoard)
{
    Position pos(5, 5);

    for (int y = 0; y < pos.getHeight(); y++) {
        for (int x = 0; x < pos.getWidth(); x++) {
            EXPECT_EQ(pos.getState(x, y), 0);
        }
    }
}

TEST(Position, Play)
{
    Position pos(5, 5);

    EXPECT_EQ(pos.isMyTurn(), true);
    pos.play(0, 0);
    EXPECT_EQ(pos.isMyTurn(), false);
    pos.play(0, 1);
    EXPECT_EQ(pos.isMyTurn(), true);

    EXPECT_EQ(pos.getState(0, 0), 1);
    EXPECT_EQ(pos.getState(0, 1), 2);
}

TEST(Position, FullGame)
{
    Position pos(5, 5);

    bool isMe = true;
    for (int y = 0; y < pos.getHeight(); y++) {
        for (int x = 0; x < pos.getWidth(); x++) {
            EXPECT_TRUE(pos.canPlay(x, y));
            pos.play(x, y);
            EXPECT_FALSE(pos.canPlay(x, y));
            EXPECT_EQ(pos.isMyTurn(), !isMe);
            EXPECT_EQ(pos.getState(x, y), isMe ? 1 : 2);
            isMe = !isMe;
        }
    }
}

TEST(Position, Hash)
{
    srand(time(nullptr));

    Position pos(20, 20);
    randomFill(pos);

    EXPECT_EQ(pos.hash(), pos.hash());
}

TEST(Position, HashOfTwoPrositions)
{
    srand(time(nullptr));

    Position pos1(20, 20);
    Position pos2(20, 20);

    for (int i = 0; i < pos1.getNbCells(); i++) {
        int x = rand() % pos1.getWidth();
        int y = rand() % pos1.getHeight();
        if (pos1.canPlay(x, y)) {
            pos1.play(x, y);
            pos2.play(x, y);
        }
    }

    EXPECT_EQ(pos1.hash(), pos2.hash());
}

TEST(Position, Equality)
{
    srand(time(nullptr));

    Position pos1(20, 20);
    Position pos2(20, 20);
    Position pos3(20, 20);

    for (int i = 0; i < pos1.getNbCells(); i++) {
        int x = rand() % pos1.getWidth();
        int y = rand() % pos1.getHeight();
        if (pos1.canPlay(x, y)) {
            pos1.play(x, y);
            pos2.play(x, y);
        }
        x = rand() % pos1.getWidth();
        y = rand() % pos1.getHeight();
        if (pos3.canPlay(x, y)) {
            pos3.play(x, y);
        }
    }

    EXPECT_EQ(pos1, pos2);
    EXPECT_EQ(pos1.hash(), pos2.hash());
    EXPECT_NE(pos1, pos3);
    EXPECT_NE(pos1.hash(), pos3.hash());
    EXPECT_NE(pos2, pos3);
    EXPECT_NE(pos2.hash(), pos3.hash());
}

TEST(Position, Copy)
{
    srand(time(nullptr));

    Position pos(20, 20);
    randomFill(pos);

    Position copy1(pos);
    EXPECT_EQ(pos, copy1);
    EXPECT_EQ(pos.hash(), copy1.hash());

    Position copy2 = pos;
    EXPECT_EQ(pos, copy2);
    EXPECT_EQ(pos.hash(), copy2.hash());

    Position copy3(5, 5);
    copy3 = pos;
    EXPECT_EQ(pos, copy3);
    EXPECT_EQ(pos.hash(), copy3.hash());

    EXPECT_EQ(copy1, copy2);
    EXPECT_EQ(copy1, copy3);
    EXPECT_EQ(copy2, copy3);
    EXPECT_EQ(pos.hash(), copy1.hash());
    EXPECT_EQ(pos.hash(), copy2.hash());
    EXPECT_EQ(pos.hash(), copy3.hash());
}

TEST(Position, VerticalFlip)
{
    /**
     * Example of a position:
     *
     * 0 1 2
     * 3 4 5
     * 6 7 8
     *
     * Vertical flip:
     *
     * 6 7 8
     * 3 4 5
     * 0 1 2
     */

    srand(time(nullptr));

    Position pos(5, 5);
    Position sym(5, 5);

    for (int i = 0; i < pos.getNbCells(); i++) {
        int x = rand() % pos.getWidth();
        int y = rand() % pos.getHeight();
        if (pos.canPlay(x, y)) {
            pos.play(x, y);
            EXPECT_TRUE(sym.canPlay(x, sym.getHeight() - y - 1));
            sym.play(x, sym.getHeight() - y - 1);
        }
    }

    EXPECT_NE(pos, sym);
    EXPECT_EQ(pos, sym.verticalFlip());
    EXPECT_EQ(pos.hash(), sym.hash());
}

TEST(Position, HorizontalFlip)
{
    /**
     * Example of a position:
     *
     * 0 1 2
     * 3 4 5
     * 6 7 8
     *
     * Horizontal flip:
     *
     * 2 1 0
     * 5 4 3
     * 8 7 6
     */

    srand(time(nullptr));

    Position pos(5, 5);
    Position sym(5, 5);

    for (int i = 0; i < pos.getNbCells(); i++) {
        int x = rand() % pos.getWidth();
        int y = rand() % pos.getHeight();
        if (pos.canPlay(x, y)) {
            pos.play(x, y);
            EXPECT_TRUE(sym.canPlay(sym.getWidth() - x - 1, y));
            sym.play(sym.getWidth() - x - 1, y);
        }
    }

    EXPECT_NE(pos, sym);
    EXPECT_EQ(pos, sym.horizontalFlip());
    EXPECT_EQ(pos.hash(), sym.hash());
}

TEST(Position, DiagonalFlip)
{
    /**
     * Example of a position:
     *
     * 0 1 2
     * 3 4 5
     * 6 7 8
     *
     * Diagonal flip:
     *
     * 0 3 6
     * 1 4 7
     * 2 5 8
     */
    srand(time(nullptr));

    Position pos(5, 5);
    Position sym(5, 5);

    for (int i = 0; i < pos.getNbCells(); i++) {
        int x = rand() % pos.getWidth();
        int y = rand() % pos.getHeight();
        if (pos.canPlay(x, y)) {
            pos.play(x, y);
            EXPECT_TRUE(sym.canPlay(y, x));
            sym.play(y, x);
        }
    }

    EXPECT_NE(pos, sym);
    EXPECT_EQ(pos, sym.diagonalFlip());
    EXPECT_EQ(pos.hash(), sym.hash());
}

TEST(Position, AntiDiagonalFlip)
{
    /**
     * Example of a position:
     *
     * 0 1 2
     * 3 4 5
     * 6 7 8
     *
     * Anti-diagonal flip:
     *
     * 8 5 2
     * 7 4 1
     * 6 3 0
     */
    srand(time(nullptr));

    Position pos(5, 5);
    Position sym(5, 5);

    for (int i = 0; i < pos.getNbCells(); i++) {
        int x = rand() % pos.getWidth();
        int y = rand() % pos.getHeight();
        if (pos.canPlay(x, y)) {
            pos.play(x, y);
            EXPECT_TRUE(sym.canPlay(sym.getHeight() - y - 1, sym.getWidth() - x - 1));
            sym.play(sym.getHeight() - y - 1, sym.getWidth() - x - 1);
        }
    }

    EXPECT_NE(pos, sym);
    EXPECT_EQ(pos, sym.antiDiagonalFlip());
    EXPECT_EQ(pos.hash(), sym.hash());
}

TEST(Position, HashFlip)
{
    srand(time(nullptr));

    Position pos(20, 20);
    randomFill(pos);

    EXPECT_EQ(pos.hash(), pos.verticalFlip().hash());
    EXPECT_EQ(pos.hash(), pos.horizontalFlip().hash());
    EXPECT_EQ(pos.hash(), pos.verticalFlip().horizontalFlip().hash());
    EXPECT_EQ(pos.hash(), pos.diagonalFlip().hash());
    EXPECT_EQ(pos.hash(), pos.antiDiagonalFlip().hash());
    EXPECT_EQ(pos.hash(), pos.diagonalFlip().verticalFlip().hash());
    EXPECT_EQ(pos.hash(), pos.antiDiagonalFlip().horizontalFlip().hash());
}

static inline int countWinningMoves(const Position &pos)
{
    int winCount = 0;
    for (int y = 0; y < pos.getHeight(); y++)
        for (int x = 0; x < pos.getWidth(); x++)
            if (pos.canPlay(x, y))
                if (pos.isWinningMove(x, y))
                    winCount++;
    return winCount;
}
TEST(Position, WinningMoveVertical)
{
    Position pos(20, 20);

    // me
    pos.play(2, 2);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(2, 3);

    // me
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(3, 2);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(3, 3);

    // me
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(4, 2);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(4, 3);

    // me
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(5, 2);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(5, 3);

    // me
    EXPECT_EQ(countWinningMoves(pos), 2);
    pos.play(6, 2);

    // you
    EXPECT_EQ(countWinningMoves(pos), 2);
    pos.play(6, 3);
}

TEST(Position, WinningMoveHorizontal)
{
    Position pos(20, 20);

    // me
    pos.play(2, 2);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(3, 2);

    // me
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(2, 3);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(3, 3);

    // me
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(2, 4);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(3, 4);

    // me
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(2, 5);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(3, 5);

    // me
    EXPECT_EQ(countWinningMoves(pos), 2);
    pos.play(2, 6);

    // you
    EXPECT_EQ(countWinningMoves(pos), 2);
    pos.play(3, 6);
}

TEST(Position, WinningMoveDiagonal)
{
    Position pos(20, 20);

    // me
    pos.play(2, 2);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(3, 2);

    // me
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(3, 3);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(4, 3);

    // me
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(4, 4);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(5, 4);

    // me
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(5, 5);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(6, 5);

    // me
    EXPECT_EQ(countWinningMoves(pos), 2);
    pos.play(6, 6);

    // you
    EXPECT_EQ(countWinningMoves(pos), 2);
    pos.play(7, 6);
}

TEST(Position, WinningMoveAntiDiagonal)
{
    Position pos(20, 20);

    // me
    pos.play(6, 2);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(7, 2);

    // me
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(5, 3);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(6, 3);

    // me
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(4, 4);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(5, 4);

    // me
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(3, 5);

    // you
    EXPECT_EQ(countWinningMoves(pos), 0);
    pos.play(4, 5);

    // me
    EXPECT_EQ(countWinningMoves(pos), 2);
    pos.play(2, 6);

    // you
    EXPECT_EQ(countWinningMoves(pos), 2);
    pos.play(3, 6);
}
}