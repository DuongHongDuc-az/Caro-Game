#include "AI.h"
#include <ctime>

using namespace std;

inline int change(int c) {
    if (c == -1) return 2;
    return c;
}

mt19937 Hashing::mt = mt19937(01234567);

ULL Hashing::computeHash(BOARD& board) {
    ULL h = 0;
    fto(i, 0, BOARD_SIZE - 1) {
        fto(j, 0, BOARD_SIZE - 1) {
            if (board[i][j].c != 0) {
                h ^= ZobristTable[i][j][change(board[i][j].c)];
            }
        }
    }
    return h;
}

int iteratePoints(BOARD& board, pii& move, int dX, int dY, int player, int isAttack) {
    int res = 1, cnt = 1, idx = 1, x = move.ff, y = move.ss;
    while (idx < 5) {
        x += dX;
        y += dY;
        if (Evaluation::isValidCell(x, y)) {
            if (board[x][y].c == 0 && isAttack)
                res++;
            else if (board[x][y].c == player && isAttack) {
                res += cnt * 2;
                cnt++;
            }
            else if (board[x][y].c != player && board[x][y].c != 0 && !isAttack) {
                res += cnt * 2;
                cnt++;
            }
            else break;
        }
        else break;
        idx++;
    }
    return res;
}

int getNodePoint(BOARD& board, pii move, int player) {
    int res = 0;
    fto(i, -1, 1) {
        fto(j, -1, 1) {
            if (i != 0 || j != 0) {
                res += iteratePoints(board, move, player, i, j, 1) + iteratePoints(board, move, player, i, j, 0);
            }
        }
    }
    return res;
}

MoveQueue getMoveList(BOARD& board, _POINT& tlB, _POINT& brB, bool isMaximizingPlayer) {
    MoveQueue res;
    int player = isMaximizingPlayer ? BOT_1 : BOT_2;

    fto(i, tlB.x, brB.x) {
        fto(j, tlB.y, brB.y) {
            PsAction::pseuMove(board, i, j, player);
            res.push(mp(mp(i, j), getNodePoint(board, mp(i, j), player)));
            PsAction::undoMove(board, i, j);
        }
    }
    return res;
}

int eval(BOARD& board, bool isMaximizingPlayer) {
    int playerVal = isMaximizingPlayer ? BOT_1 : BOT_2;
    return isMaximizingPlayer ? boardScoreCal(board, playerVal) : -boardScoreCal(board, playerVal);
}

int minimax(BOARD& board, int depth, pii lastMove, int alpha, int beta, bool isMaximizingPlayer, _POINT tlB, _POINT brB, Hashing& hash) {
    //Who has just taken the move
    int player = (!isMaximizingPlayer) ? BOT_1 : BOT_2;
    int gameState = getGameState(board, _POINT{lastMove.ff, lastMove.ss, player});
    if (gameState != 0) {
        return (player == BOT_2) ? -MAX_SCORE * gameState : MAX_SCORE * gameState;
    }

    if (depth == 0) {
        return eval(board, !isMaximizingPlayer);
    }
    _POINT newtlB, newbrB;
    newtlB.x = max(0, tlB.x - 1);
    newtlB.y = max(0, tlB.y - 1);
    newbrB.x = min(BOARD_SIZE - 1, brB.x + 1);
    newbrB.y = min(BOARD_SIZE - 1, brB.y + 1);

    MoveQueue moveQueue = getMoveList(board, newtlB, newbrB, isMaximizingPlayer);

    int bestVal = isMaximizingPlayer ? -oo : oo;
    while (!moveQueue.empty()) {
        pii move = moveQueue.top().ff;
        PsAction::pseuMove(board, move.ff, move.ss, isMaximizingPlayer ? BOT_1 : BOT_2);
        int curVal;
        ULL hashKey = hash.computeHash(board);
        if (hash.checkTable[hashKey]) {
            curVal = hash.evalTable[hashKey];
        }
        else {
            auto tmp = PsAction::updateBound(newtlB, newbrB, move.ff, move.ss);
            curVal = minimax(board, depth - 1, move, alpha, beta, !isMaximizingPlayer, tmp.ff, tmp.ss, hash);
            hash.checkTable[hashKey] = 1;
            hash.evalTable[hashKey] = curVal;
        }
        optimize(bestVal, curVal, !isMaximizingPlayer);
        optimize(alpha, curVal, !isMaximizingPlayer);
        PsAction::undoMove(board, move.ff, move.ss);
        if (beta < alpha) break;
        moveQueue.pop();
    }
    return bestVal;
}

pii getBestMove(BOARD& board) {
    _POINT newtlB, newbrB;
    newtlB.x = max(0, tlB.x - 1);
    newtlB.y = max(0, tlB.y - 1);
    newbrB.x = min(BOARD_SIZE - 1, brB.x + 1);
    newbrB.y = min(BOARD_SIZE - 1, brB.y + 1);
    int bestVal = -oo, alpha = -oo, beta = oo;
    pii res = { 0,0 };
    MoveQueue moveQueue = getMoveList(board, newtlB, newbrB, 1);
    pii bestMove = moveQueue.top().ff;
    Hashing hash;
    srand(time(NULL));
    while (!moveQueue.empty()) {
        MOVE node = moveQueue.top();
        pii move = node.ff;
        PsAction::pseuMove(board, move.ff, move.ss, BOT_1);
        if (getGameState(board, _POINT{ move.ff, move.ss, BOT_1 }) == 1) {
            PsAction::undoMove(board, move.ff, move.ss);
            return move;
        }
        int curVal;
        ULL hashKey = hash.computeHash(board);
        if (hash.checkTable[hashKey]) {
            curVal = hash.evalTable[hashKey];
        }
        else {
            curVal = minimax(board, DIFF - 1, move, alpha, beta, 0, tlB, brB, hash);
            hash.checkTable[hashKey] = 1;
            hash.evalTable[hashKey] = curVal;
        }
        if (optimize(bestVal, curVal)) {
            res = move;
        }
        else if (curVal == bestVal) {
            if (rand() % 2) res = move;
        }
        PsAction::undoMove(board, move.ff, move.ss);
        optimize(alpha, curVal);
        if (beta < alpha) break;
        moveQueue.pop();
    }
    return res;
}