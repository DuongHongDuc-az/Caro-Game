#ifndef VIEW_H
#define VIEW_H

#include "Model.h"
#include <string>
#include<windows.h>
#define LEFT 3
#define TOP 1

extern int langChoice;
void fixConsoleWindow();
void GotoXY(int x, int y);
void setColor(int color);
void drawBoard();
void updateCellAtScreen(int screenX, int screenY, int player);
void updateCellAtBoardIndex(int col, int row, int player);
void displayTimeOfFile(int infoX, int infoY);
void showPlayerInfo();
void displayMessage(const std::string& message, int x, int y);
void clearMessage(int x, int y, int length);
void showWinEffect(int player);
int showMainMenu();
int hienMenuChinh();
bool askContinue();
std::string getFileNameFromUser(const std::string& prompt, int x, int y);
void showAbout();
void thongTin();
int showSettingsMenu();
int showMenuSettings();
void setConsoleFont();
void waitForMouseClick();
void showSplashScreen();
void drawMenu(int x, int y, int width, int height);
void printCentered(std::string text, int y, int color);
void drawRightSideImage();
void randomizeSideImage();
void drawSettingsArt();
#endif