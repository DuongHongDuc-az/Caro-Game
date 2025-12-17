#include "Model.h"
#include "AI.h"
#include "Evaluation.h"
#include "giaoDien.h"
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <filesystem>

using namespace std;

BOARD board(BOARD_SIZE, vector<_POINT>(BOARD_SIZE));
int turn;
int remains;
Player player1;
Player player2;
std::vector<std::pair<std::string, std::pair<std::string, std::string>>> timeFl;
_POINT tlB(11,11), brB(0,0);
std::vector<std::wstring> nameOfFile;

static bool changeData(const std::wstring& filenameOld, const std::wstring& filenameNew);
static void saveTimeOfFile(std::wstring fileName);

void resetData() {
    turn = 1;
    _POINT rtlB(11,11), rbrB(0,0);
    swap(rtlB, tlB);
    swap(rbrB, brB);
    remains = BOARD_SIZE * BOARD_SIZE;
    player1 = { L"Player X", 0, 0 };
    player2 = { L"Player O", 0, 0 };

    winText.setString("");
    continueText.setString("");

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j].x = j;
            board[i][j].y = i;
            board[i][j].c = 0;
        }
    }
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

int getGameState(BOARD& board, _POINT lastMove) {
    //Return values: 0 - draw, 1 - Win, 2 - ongoing
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
        if (res >= 5)
            return 1;
    }
    return 2;
}

static void saveTimeOfFile(std::string fileName) {
    
    std::time_t timeNum = std::time(nullptr);
    std::tm timeReal{};

    localtime_s(&timeReal, &timeNum);

    char dateShow[100], timeShow[100];
    //wchar_t dateShow[100], timeShow[100];
    std::strftime(dateShow, sizeof(dateShow), "%d/%m/%Y", &timeReal);
    std::strftime(timeShow, sizeof(timeShow), " %H:%M:%S", &timeReal);
    //std::wcsftime(dateShow, sizeof(dateShow), L"%d/%m/%Y", &timeReal);
    //std::wcsftime(timeShow, sizeof(timeShow), L"%H:%M:%S", &timeReal);
    
    std::ofstream f("timeFile.txt");

    for (int i = 0; i < timeFl.size(); ++i) {
        if (timeFl[i].first == fileName) {
            timeFl.erase(timeFl.begin() + i);
            break;
        }
    }
    timeFl.push_back({ fileName, {dateShow, timeShow } });

    f << timeFl.size() << "\n";
    for (size_t i = 0; i < timeFl.size(); ++i) {
        //std::wostringstream woss;
        //woss << timeFl[i].first;

        //f << woss.str() << " " << timeFl[i].second.first << " " << timeFl[i].second.second << "\n";
        f << timeFl[i].first << " " << timeFl[i].second.first << " " << timeFl[i].second.second << "\n";
    }

    f.close();
}

bool saveGame(const std::wstring& filename) {
    std::wofstream nOF(L"name_of_file.txt");
    if (!nOF.is_open()) return false;

    nameOfFile.push_back(filename);
    nOF << nameOfFile.size() << "\n";
    for (size_t i = 0; i < nameOfFile.size(); ++i) {
        std::wostringstream woss;

        woss << (int)i << L" " << nameOfFile[i];
        nOF << woss.str() << L"\n";
    }

    nOF.close();

    char c = (int)(nameOfFile.size() - 1) + '0';
    std::string fileName{ c };
    //std::cout << "File name: " << fileName << "\n";
    std::ofstream f(fileName);
    if (!f.is_open()) return false;

    saveTimeOfFile(fileName);

    f << turn << "\n" << remains << "\n";
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            f << board[i][j].c << " ";
        }
        f << "\n";
    }
    //f << "Player X" << "\n" << player1.moves << "\n" << player1.wins << "\n";
    //f << "Player O" << "\n" << player2.moves << "\n" << player2.wins << "\n";
    f << player1.moves << "\n" << player1.wins << "\n";
    f << player2.moves << "\n" << player2.wins << "\n";
    f.close();
    return true;
}

bool loadGame(const std::wstring& filename) {
    std::wifstream nOF(L"name_of_file.txt");
    if (!nOF.is_open()) return false;

    int sizeNameOfFile = 0;
    char p = '0';

    nOF >> sizeNameOfFile;
    for (size_t i = 0; i < sizeNameOfFile; ++i) {
        std::wstring ws;
        int m = -1;

        nOF >> m >> ws;
        nameOfFile.push_back(ws);

        if (ws == filename) p = m + '0';
    }

    nOF.close();

    std::string s{ p };

    std::ifstream f(s);
    if (!f.is_open()) return false;
    f >> turn;
    f >> remains;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            f >> board[i][j].c;
        }
    }
    f.ignore();
    //std::string temp;
    //if (std::getline(f, temp)) player1.name = L"Player X"; 

    //std::cout << temp << "\n";
    if (!(f >> player1.moves)) player1.moves = 0;
    if (!(f >> player1.wins)) player1.wins = 0;
    f.ignore();
    //if (std::getline(f, temp)) player2.name = L"Player O";
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

// static bool changeData(const std::wstring& filenameOld, const std::wstring& filenameNew) {
    //std::wifstream fold(filenameOld);
    //std::wofstream fnew(filenameNew);

    //if (!fold.is_open()) return false;

    //int turnFileOld, remainsFileOld, moves, wins;
    //int a[BOARD_SIZE][BOARD_SIZE];

    //if (!fold.is_open()) return false;
    //fold >> turnFileOld; fnew << turnFileOld << "\n";
    //fold >> remainsFileOld; fnew << remainsFileOld << "\n";
    //for (int i = 0; i < BOARD_SIZE; ++i) {
    //    for (int j = 0; j < BOARD_SIZE; ++j) {
    //        fold >> a[i][j];
    //        fnew << a[i][j] << " ";
    //    }
    //    fnew << "\n";
    //}
    //fold.ignore();
    //std::string temp;
    //std::getline(fold, temp); fnew << temp << "\n";
    //fold >> moves; fnew << moves << "\n";
    //fold >> wins; fnew << wins << "\n";

    //fold.ignore();
    //std::getline(fold, temp); fnew << temp << "\n";
    //fold >> moves; fnew << moves << "\n";
    //fold >> wins; fnew << wins << "\n";

    //fold.close();
    ////remove(filenameOld.c_str());
    //_wremove(filenameOld.c_str());
    //fnew.close();
// }

// bool renameGame(const std::wstring& filenameOld, const std::wstring& filenameNew) {
    //std::ofstream f("timeFile.txt");

    //int tmpSize = timeFl.size();

    //int flag = 0;

    //for (int i = 0; i < timeFl.size(); ++i) {
    //    if (timeFl[i].first == filenameOld) {
    //        if (!changeData(filenameOld, filenameNew)) break;

    //        timeFl[i].first = filenameNew;
    //        flag = 1;
    //        break;
    //    }
    //}

    //if (flag != 1) {
    //    f.close();
    //    return false;
    //}

    //f << timeFl.size() << "\n";
    //for (int i = 0; i < timeFl.size(); ++i) f << timeFl[i].first << " " << timeFl[i].second.first << " " << timeFl[i].second.second << "\n";

    //f.close();

    // return true;
// }

bool deleteGame(const std::wstring& filename) {
    //std::ofstream f("timeFile.txt");

    //int tmpSize = timeFl.size();

    //for (int i = 0; i < timeFl.size(); ++i) {
    //    if (timeFl[i].first == filename) {
    //        timeFl.erase(timeFl.begin() + i);
    //        _wremove(filename.c_str());
    //        break;
    //    }
    //}

    //if (tmpSize == timeFl.size()) return false;

    //f << timeFl.size() << "\n";
    //for (int i = 0; i < timeFl.size(); ++i) f << timeFl[i].first << " " << timeFl[i].second.first << " " << timeFl[i].second.second << "\n";

    //f.close();

    return true;
}

//-------------------------------------------------------------------------------------------------
_POINT getPointAt(int x, int y) {
    return board[y][x];
}
