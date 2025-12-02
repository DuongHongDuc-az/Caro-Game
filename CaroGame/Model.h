#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

#define BOARD_SIZE 12

struct _POINT {
	int x, y;
	int c;
};

struct Player {
	std::string name;
	int moves;
	int wins;
};

extern _POINT board[BOARD_SIZE][BOARD_SIZE];
extern int turn;
extern int remains;
extern Player player1, player2;
extern std::vector<std::pair<std::string, std::pair<std::string, std::string>>> timeFl;

void resetData();
bool takeTurn(int pX, int pY);
int checkWin();
std::string getCurrentTime();
bool saveGame(const std::string& filename);
bool loadGame(const std::string& filename);
bool renameGame(const std::string& filenameOld, const std::string& filenameNew);
bool deleteGame(const std::string& filename);

_POINT getPointAt(int x, int y);

#endif