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
extern int selectedFile;

int checkInputString(std::string s);
void drawBoard();
void updateCellAtScreen(int screenX, int screenY, int player, int b);
void handleShowListOfFile();
void showPlayerInfo();
void showWinEffect(int result, int player);
void showMainMenu();
void showModeMenu(int type = 0);
void hienMenuChinh();
void askContinue();
void showAbout();
int handleVol(const sf::Event& event);
void showSettingsMenu();
int showMenuSettings();
void showPlayerMenu();
void displayListOfFile();
void showButtonLoad(int m);
void showInputText(int slr, const sf::Event& event);
void handleMainMenu(const sf::Event& event);

#endif