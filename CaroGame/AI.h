#ifndef AI_H
#define AI_H

#include "Model.h"
#include "Evaluation.h"
#include <random>
#include <queue>
#include <algorithm>

using namespace std;

#define MAX_SCORE 10000
#define oo static_cast<int>(1e9+7)

//Ez: 1, Med: 2, Hard: 3 
extern int DIFF;

typedef unsigned long long ULL;
typedef pair<int, int> pii;
typedef pair<pii, int> MOVE;
struct COMP {
    bool operator()(MOVE a, MOVE b) {
        return a.ss < b.ss;
    }
};
typedef priority_queue<MOVE, vector<MOVE>, COMP> MoveQueue;
typedef unordered_map<ULL, int> evalTable;
typedef unordered_map<ULL, bool> checkTable;

template<class A, class B> bool optimize(A& a, B b, bool isMin = 0) {
    return !isMin ? (a < b ? a = b, 1 : 0) : (a > b ? a = b, 1 : 0);
}

namespace PsAction {
    inline void pseuMove(BOARD& board, int x, int y, int c) {
        board[x][y].c = c;
        remains--;
    }
    inline void undoMove(BOARD& board, int x, int y) {
        board[x][y].c = 0;
        remains++;
    }
    inline pair<_POINT, _POINT> updateBound(_POINT tlB, _POINT brB, int x, int y) {
        optimize(tlB.x, x, 1);
        optimize(tlB.y, y, 1);
        optimize(brB.x, x);
        optimize(brB.y, y);
        return mp(tlB, brB);
    }
}

struct Hashing {
    static mt19937 mt;
    ULL ZobristTable[BOARD_SIZE][BOARD_SIZE][3];
    evalTable evalTable;
    checkTable checkTable;

    inline constexpr void initTable() {
        fto(i, 0, BOARD_SIZE - 1) {
            fto(j, 0, BOARD_SIZE - 1) {
                fto(k, 0, 2) {
                    ZobristTable[i][j][k] = randomInt();
                }
            }
        }
    }

    Hashing() {
        initTable();
    }

    inline ULL randomInt() {
        std::uniform_int_distribution<ULL> dist(0, MAXX);
        return dist(mt);
    }

    ULL computeHash(BOARD& board);
};

int iteratePoints(BOARD& board, pii& move, int dX, int dY, int player, int isAttack);
int getNodePoint(BOARD& board, pii move, int player);
MoveQueue getMoveList(BOARD& board, _POINT& tlB, _POINT& brB, bool isMaximizingPlayer);
int eval(BOARD& board, bool isMaximizingPlayer);
int minimax(BOARD& board, int depth, pii lastMove, int alpha, int beta, bool isMaximizingPlayer, _POINT tlB, _POINT brB, Hashing& hash);
pii getBestMove(BOARD& board);

#endif