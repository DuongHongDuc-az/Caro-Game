#include "Evaluation.h"

using namespace std;

int boardScoreCal(BOARD& board, int player) {
    vector<vector<int>> checkBoard(BOARD_SIZE, vector<int>(BOARD_SIZE, 210));
    int res = 0;
    fto(i, 0, BOARD_SIZE - 1) {
        fto(j, 0, BOARD_SIZE - 1) {
            if (board[i][j].c != 0) {
                int tmpVal = 0;
                if (checkBoard[i][j] % 2 == 0) {
                    _POINT xxx = { i, j, board[i][j].c };
                    int getVal = getComboEval(board, checkBoard, xxx, 0);
                    if (board[i][j].c != player && (getVal == Evaluation::score[3] * 10 || getVal == Evaluation::score[3] / 2))
                        getVal = Evaluation::score[3] * 50;
                    tmpVal += getVal;
                }
                if (checkBoard[i][j] % 3 == 0) {
                    _POINT xxx = { i, j, board[i][j].c };
                    int getVal = getComboEval(board, checkBoard, xxx, 1);
                    if (board[i][j].c != player && (getVal == Evaluation::score[3] * 10 || getVal == Evaluation::score[3] / 2))
                        getVal = Evaluation::score[3] * 50;
                    tmpVal += getVal;
                }
                if (checkBoard[i][j] % 5 == 0) {
                    _POINT xxx = { i, j, board[i][j].c };
                    int getVal = getComboEval(board, checkBoard, xxx, 2);
                    if (board[i][j].c != player && (getVal == Evaluation::score[3] * 10 || getVal == Evaluation::score[3] / 2))
                        getVal = Evaluation::score[3] * 50;
                    tmpVal += getVal;
                }
                if (checkBoard[i][j] % 7 == 0) {
                    _POINT xxx = { i, j, board[i][j].c };
                    int getVal = getComboEval(board, checkBoard, xxx, 3);
                    if (board[i][j].c != player && (getVal == Evaluation::score[3] * 10 || getVal == Evaluation::score[3] / 2))
                        getVal = Evaluation::score[3] * 50;
                    tmpVal += getVal;
                }
                if (board[i][j].c == player)
                    res += tmpVal;
                else
                    res -= tmpVal;
            }
        }
    }
    return res;
}

inline int getComboEval(BOARD& board, MATRIX& checkBoard, _POINT& p, int type) {
    int res = 0;
    _POINT temp = p;
    while (Evaluation::isValidCell(temp.x + Evaluation::direct[type].ff, temp.y + Evaluation::direct[type].ss)) {
        temp.move(Evaluation::direct[type]);
        if (board[temp.x][temp.y].c == board[p.x][p.y].c) {
            checkBoard[temp.x][temp.y] /= Evaluation::divisor[type];
        }
        else {
            temp.move({ -Evaluation::direct[type].ff, -Evaluation::direct[type].ss });
            break;
        }
    }

    _POINT temp2 = p;
    res = Evaluation::score[max(temp.x - p.x, abs(temp.y - p.y))];
    bool block1 = 0, block2 = 0;
    temp2.move({ -Evaluation::direct[type].ff, -Evaluation::direct[type].ss });
    temp.move(Evaluation::direct[type]);

    if (!Evaluation::isValidCell(temp2.x, temp2.y) || (board[temp2.x][temp2.y].c != 0 && board[temp2.x][temp2.y].c != board[p.x][p.y].c))
        block1 = 1;
    if (!Evaluation::isValidCell(temp.x, temp.y) || (board[temp.x][temp.y].c != 0 && board[temp.x][temp.y].c != board[p.x][p.y].c))
        block2 = 1;
    if (block1 && block2) res = 0;
    if (block1 || block2) res /= 2;
    if (res == Evaluation::score[3]) res = Evaluation::score[3] * 10;
    return res;
}