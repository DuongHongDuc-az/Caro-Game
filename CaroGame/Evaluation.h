#ifndef EVALUATION_H
#define EVALUATION_H

#include "Model.h"

using namespace std;

namespace Evaluation {
    const pair<int,int> direct[4] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
    const int divisor[4] = { 2, 3, 5, 7 };
    const int score[4] = { 1, 10, 50, 200 };
    inline bool isValidCell(int x, int y) {
        return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE);
    }
}

inline int getComboEval(BOARD& board, MATRIX& checkBoard, _POINT& p, int type);
int boardScoreCal(BOARD& board, int player);

#endif