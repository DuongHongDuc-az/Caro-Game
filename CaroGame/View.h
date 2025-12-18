#ifndef VIEW_H
#define VIEW_H

#include "Model.h"
#include "Interface.h"
#include <string>
#include <windows.h>
#define LEFT 3
#define TOP 1

extern int loadFromMenu;
extern int langChoice;
extern int sMM;
extern int cs;
extern float startX;
extern float startY;
extern int drawRec;
extern int delFile;
extern int res;
extern int askCon;

void fixConsoleWindow();
void GotoXY(int x, int y);
void setColor(int color);
void drawBoard();
void updateCellAtScreen(int screenX, int screenY, int player, int b);
void updateCellAtBoardIndex(int col, int row, int player);
void handleShowListOfFile();
void showPlayerInfo();
void displayMessage(const std::string &message, int x, int y);
void clearMessage(int x, int y, int length);
void showWinEffect(int result, int player);
void showMainMenu();
void showModeMenu(int type = 0);
void hienMenuChinh();
void askContinue();
std::string getFileNameFromUser(const std::string &prompt, int x, int y);
void showAbout();
int handleVol(const sf::Event& event);
void showSettingsMenu();
int showMenuSettings();
void showPlayerMenu();
void setConsoleFont();
void waitForMouseClick();
void showSplashScreen();
void drawMenu(int x, int y, int width, int height);
void printCentered(std::string text, int y, int color);
void drawRightSideImage();
void randomizeSideImage();
void drawSettingsArt();
void displayListOfFile();
void showButtonLoad(int m);
void showInputText(int slr, const sf::Event& event);
void handleMainMenu(const sf::Event& event);

#endif