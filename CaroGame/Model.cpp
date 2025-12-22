#include "Model.h"
#include "AI.h"
#include "Evaluation.h"
#include "Interface.h"
#include "Controller.h"
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <filesystem>

using namespace std;

BOARD board(BOARD_SIZE, vector<_POINT>(BOARD_SIZE));
BOARD boardMini(BOARD_SIZE, vector<_POINT>(BOARD_SIZE));
int turn;
int turnMini;
int remains;
int remainsMini;
int numFileDeleted = 0;
Player player1;
Player player2;
std::vector<std::pair<std::string, std::pair<std::string, std::string>>> timeFl;
std::vector<std::string> nameOfFile;
_POINT tlB(9,9), brB(0,0);

static void saveTimeOfFile(std::string fileName);
static int cs = (BOARD_SIZE) * 60;
static float startX = (WINDOW_W - cs) / 2.0f;
static float startY = (WINDOW_H - cs) / 2.0f;
static bool changeData(const std::string& filenameOld, const std::string& filenameNew);

void resetData() {
    turn = 1;
    cursorCol = 0;
    cursorRow = 0;
    _POINT rtlB(9,9), rbrB(0,0);
    swap(rtlB, tlB);
    swap(rbrB, brB);
    remains = BOARD_SIZE * BOARD_SIZE;
    player1 = { L"Player X", 0, 0 };
    player2 = { L"Player O", 0, 0 };

    winText.setString("");
    continueText.setString("");
    cursor.setPosition(sf::Vector2f({ startX + 18, startY + 6 }));

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j].x = j;
            board[i][j].y = i;
            board[i][j].c = 0;
        }
    }

    std::ifstream f(L"timeFile.txt");

    int sizeTimeFile = 0;

    f >> sizeTimeFile;

    timeFl.resize(sizeTimeFile);

    for (size_t i = 0; i < sizeTimeFile; ++i) {
        std::string s, d, t;
        int m = -1;

        f >> s >> d >> t;
        timeFl[i] = { s, {d, t} };
    }

    f.close();
}

static inline void upBound(_POINT& tlB, _POINT& brB, int x, int y) {
    optimize(tlB.x, x, 1);
    optimize(tlB.y, y, 1);
    optimize(brB.x, x);
    optimize(brB.y, y);
}

bool takeTurn(int pX, int pY) {
    if (pX < 0 || pX >= BOARD_SIZE || pY < 0 || pY >= BOARD_SIZE) return false;
    if (board[pY][pX].c != 0) return false;

    board[pY][pX].c = turn;
    remains--;

    if (turn == 1) ++player1.moves;
    else ++player2.moves;

    turn = -turn;
    return true;
}

static pii winPosition, winDirection;

pair<pii, pii> getWinLine(BOARD& board) {
    function<void(pii&, pii)> add = [&](pii& a, pii b) {
        a.ff += b.ff;
        a.ss += b.ss;
        };
    pair<pii, pii> res = mp(mp(winPosition.ff, winPosition.ss), mp(winPosition.ff, winPosition.ss));
    bool isContinue1 = 1, isContinue2 = 1;
    pii pos1 = mp(winPosition.ff + winDirection.ff, winPosition.ss + winDirection.ss), pos2 = mp(winPosition.ff - winDirection.ff, winPosition.ss - winDirection.ss);
    while (isContinue1 || isContinue2) {
        if (isContinue1) {
            if (Evaluation::isValidCell(pos1.ff, pos1.ss) && board[pos1.ff][pos1.ss].c == board[winPosition.ff][winPosition.ss].c) {
                res.ff = pos1;
                add(pos1, mp(winDirection.ff, winDirection.ss));
            }
            else isContinue1 = 0;
        }
        if (isContinue2) {
            if (Evaluation::isValidCell(pos2.ff, pos2.ss) && board[pos2.ff][pos2.ss].c == board[winPosition.ff][winPosition.ss].c) {
                res.ss = pos2;
                add(pos2, mp(-winDirection.ff, -winDirection.ss));
            }
            else isContinue2 = 0;
        }
    }

    return res;
}

int getGameState(BOARD& board, _POINT lastMove) {
    function<int(_POINT&, int, int)> countConsecutive = [&](_POINT& move, int dX, int dY) {
        int res = 0;
        int x = move.x + dX, y = move.y + dY, z = move.c;
        while (Evaluation::isValidCell(x, y) && board[x][y].c == z && res < 5) {
            res++;
            x += dX;
            y += dY;
        }
        return res;
    };
    if (remains == 0) return 0;
    for (auto dir : Evaluation::direct) {
        int res = countConsecutive(lastMove, dir.ff, dir.ss) + countConsecutive(lastMove, -dir.ff, -dir.ss) + 1;
        if (res >= 5) {
            winPosition = mp(lastMove.x, lastMove.y);
            winDirection = dir;
            return 1;
        }
    }
    return 2;
}

void saveTimeOfFile(std::string fileName) {
    std::time_t timeNum = std::time(nullptr);
    std::tm timeReal{};

    localtime_s(&timeReal, &timeNum);

    char dateShow[100], timeShow[100];
    std::strftime(dateShow, sizeof(dateShow), "%d/%m/%Y", &timeReal);
    std::strftime(timeShow, sizeof(timeShow), " %H:%M:%S", &timeReal);

    std::ofstream f("timeFile.txt");

    for (int i = 0; i < timeFl.size(); ++i) {
        if (timeFl[i].first == fileName) {
            timeFl.erase(timeFl.begin() + i);
            break;
        }
    }
    timeFl.push_back({ fileName, {dateShow, timeShow } });

    f << timeFl.size() << "\n";
    for (int i = 0; i < timeFl.size(); ++i) f << timeFl[i].first << " " << timeFl[i].second.first << " " << timeFl[i].second.second << "\n";

    f.close();
}

bool saveGame(const std::string& filename) {
    std::ofstream f(filename);
    if (!f.is_open()) return false;
    saveTimeOfFile(filename);
    f << turn << "\n" << remains << "\n";
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            f << board[i][j].c << " ";
        }
        f << "\n";
    }
    f << player1.moves << "\n" << player1.wins << "\n";
    f << player2.moves << "\n" << player2.wins << "\n";
    f.close();
    return true;
}

bool loadGame(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) return false;
    f >> turn;
    f >> remains;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            f >> board[i][j].c;
        }
    }
    f.ignore();
    if (!(f >> player1.moves)) player1.moves = 0;
    if (!(f >> player1.wins)) player1.wins = 0;
    f.ignore();
    if (!(f >> player2.moves)) player2.moves = 0;
    if (!(f >> player2.wins)) player2.wins = 0;
    f.close();
    return true;
}

static bool changeData(const std::string& filenameOld, const std::string& filenameNew) {
    std::ifstream fold(filenameOld);
    std::ofstream fnew(filenameNew);

    if (!fold.is_open()) return false;

    int turnFileOld, remainsFileOld, moves, wins;
    int a[BOARD_SIZE][BOARD_SIZE];

    if (!fold.is_open()) return false;
    fold >> turnFileOld; fnew << turnFileOld << "\n";
    fold >> remainsFileOld; fnew << remainsFileOld << "\n";
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            fold >> a[i][j];
            fnew << a[i][j] << " ";
        }
        fnew << "\n";
    }
    fold.ignore();
    std::string temp;
    std::getline(fold, temp); fnew << temp << "\n";
    fold >> moves; fnew << moves << "\n";
    fold >> wins; fnew << wins << "\n";

    fold.ignore();
    std::getline(fold, temp); fnew << temp << "\n";
    fold >> moves; fnew << moves << "\n";
    fold >> wins; fnew << wins << "\n";

    fold.close();
    remove(filenameOld.c_str());
    fnew.close();

    return true;
}

bool renameGame(const std::string& filenameOld, const std::string& filenameNew) {
    std::ofstream f("timeFile.txt");

    int tmpSize = timeFl.size();

    int flag = 0;

    for (int i = 0; i < timeFl.size(); ++i) {
        if (timeFl[i].first == filenameOld) {
            if (!changeData(filenameOld, filenameNew)) break;

            timeFl[i].first = filenameNew;
            flag = 1;
            break;
        }
    }

    if (flag != 1) {
        f.close();
        return false;
    }

    f << timeFl.size() << "\n";
    for (int i = 0; i < timeFl.size(); ++i) f << timeFl[i].first << " " << timeFl[i].second.first << " " << timeFl[i].second.second << "\n";

    f.close();

    return true;
}

bool deleteGame(const std::string& filename) {
    std::ofstream f("timeFile.txt");

    int tmpSize = timeFl.size();

    for (int i = 0; i < timeFl.size(); ++i) {
        if (timeFl[i].first == filename) {
            remove(filename.c_str());
            timeFl.erase(timeFl.begin() + i);            
            break;
        }
    }

    if (tmpSize == timeFl.size()) return false;

    f << timeFl.size() << "\n";
    for (int i = 0; i < timeFl.size(); ++i) f << timeFl[i].first << " " << timeFl[i].second.first << " " << timeFl[i].second.second << "\n";

    f.close();

    return true;
}

//-------------------------------------------------------------------------------------------------
_POINT getPointAt(int x, int y) {
    return board[y][x];
}
