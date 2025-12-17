#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <utility>
#include <functional>

#include <fstream>

using std::pair;
using std::vector;

#define BOARD_SIZE 12
#define MAXX 0xffffffffffffffffULL
#define BOT_1 -1
#define BOT_2 1
#define ff first
#define ss second
#define mp make_pair
#define fto(i, j, k) for (int i = j; i <= k; ++i)

typedef vector<vector<int>> MATRIX;

struct _POINT
{
	int x, y;
	int c;
	_POINT()
	{
		x = y = c = 0;
	}
	_POINT(int X, int Y, int Z = 0)
	{
		x = X;
		y = Y;
		c = Z;
	}
	void move(pair<int, int> dir)
	{
		x += dir.first;
		y += dir.second;
	}
};

typedef vector<vector<_POINT>> BOARD;

struct Player
{
	std::wstring name;
	int moves;
	int wins;
};

extern BOARD board;
extern int turn;
extern int remains;
extern Player player1, player2;
extern std::vector<std::pair<std::string, std::pair<std::string, std::string>>> timeFl;
extern _POINT tlB, brB;
extern std::vector<std::wstring> nameOfFile;

void resetData();
bool takeTurn(int pX, int pY);
int getGameState(BOARD &board, _POINT lastMove);
std::string getCurrentTime();
bool saveGame(const std::wstring &filename);
bool loadGame(const std::wstring &filename);
bool renameGame(const std::wstring &filenameOld, const std::wstring &filenameNew);
bool deleteGame(const std::wstring &filename);

_POINT getPointAt(int x, int y);

#endif