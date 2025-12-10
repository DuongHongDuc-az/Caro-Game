#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "AI.h"
#include "AudioManager.h"
#include <conio.h>
#include <windows.h>
#include <string>
#include <iostream>
static int cursorCol = 0;
static int cursorRow = 0;
static bool okToCheck = 0;

static void moveCursor(int direction);
static bool processInput();
static void handleTurn();
static void handleSave();
static void handleLoad(bool startFromMenu);
static void handleRename();
static void handleDelete();
static void endGame(int result, int player);
static void redrawGameScreen();
static void runGameLoop();


static void handleSettings() {
    bool exitSettings = false;
    AudioManager& audio = AudioManager::getInstance();

    while (!exitSettings) {
        int choice = (langChoice == 1) ? showSettingsMenu() : showMenuSettings();

        switch (choice) {
        case 1: 
            audio.toggleMute();
            break;
        case 2: 
            audio.decreaseVolume();
            break;
        case 3: 
            audio.increaseVolume();
            break;
        case 0: 
            exitSettings = true;
            break;
        default:
            break;
        }
    }
}
extern void randomizeSideImage();
void run() {
    fixConsoleWindow();
    langChoice = 1;

    int choice = showMainMenu();
    while (true) {
        switch (choice) {
        case 1: {
            int modeChoice = showModeMenu();
            switch (modeChoice) {
                case 1:
                    randomizeSideImage();
                    resetData();
                    runGameLoop();
                    break;
                case 2: {
                    int diffChoice = showModeMenu(1);
                    if (diffChoice < 4) DIFF = diffChoice;
                    else break;
                    randomizeSideImage();
                    resetData();
                    handleBotPlay();
                    break;
                }
                case 3:
                    break;
            }
            break;
        }
        case 2:
            handleLoad(true);
            break;
        case 3:
            showAbout();
            break;
        case 4:
            handleSettings();
            break;
        case 0:
            return;
        default:
            break;
        }

        if (langChoice == 1) choice = showMainMenu();
        else choice = hienMenuChinh();
    }
}

static void runGameLoop() {
    AudioManager::getInstance().stopBackgroundMusic();
    bool exitRequested = false;
    redrawGameScreen();
    while (!exitRequested) {
        exitRequested = processInput();
        int result = 2;
        if (okToCheck) {
            result = getGameState(board, _POINT{cursorRow, cursorCol, -turn});
			okToCheck = 0;
        }
        if (result != 2) {
            endGame(result,-turn);
            return;
        }
    }
}


static bool processInput() {
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            int c2 = _getch();
            if (c2 == 72) moveCursor(0);
            else if (c2 == 80) moveCursor(1);
            else if (c2 == 75) moveCursor(2);
            else if (c2 == 77) moveCursor(3);
            return false;
        }

        ch = toupper(ch);
        switch (ch) {
        case 'W': moveCursor(0); break;
        case 'S': moveCursor(1); break;
        case 'A': moveCursor(2); break;
        case 'D': moveCursor(3); break;
        case 13: handleTurn(); break;
        case 'L': handleSave(); break;
        case 'T': handleLoad(false); break;
        case 'R': handleRename(); break;
        case 'X': handleDelete(); break;
        case 27:
            return true;
        default:
            break;
        }
    }
    return false;
}

static void moveCursor(int direction) {
    if (direction == 0 && cursorRow > 0) cursorRow--;
    else if (direction == 1 && cursorRow < BOARD_SIZE - 1) cursorRow++;
    else if (direction == 2 && cursorCol > 0) cursorCol--;
    else if (direction == 3 && cursorCol < BOARD_SIZE - 1) cursorCol++;

    int screenX = LEFT + cursorCol * 4 + 2;
    int screenY = TOP + cursorRow * 2 + 1;
    GotoXY(screenX, screenY);
}

static void handleTurn() {
    if (takeTurn(cursorCol, cursorRow)) {
        AudioManager::getInstance().playSound(SoundEffect::Move);
        int previous_turn = (turn == 1) ? -1 : 1;
        updateCellAtBoardIndex(cursorCol, cursorRow, previous_turn);
		okToCheck = 1;
        showPlayerInfo();
        int screenX = LEFT + cursorCol * 4 + 2;
        int screenY = TOP + cursorRow * 2 + 1;
        GotoXY(screenX, screenY);
    }
}

static void handleSave() {
    int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    int msg_y = TOP + 12 + (timeFl.size() > 0 ? timeFl.size() + 1 + 2 : 2);
    std::string filename;

    GotoXY(msg_x, msg_y);
    langChoice == 1 ? std::cout << "Enter the file name to save (Maxium 10 character): " : std::cout << "Nhập tên file muốn lưu tối đa 10 kí tự: ";
    std::cin >> filename;

    clearMessage(msg_x, msg_y, 60);


    int infoX = LEFT + BOARD_SIZE * 4 + 5;
    int infoY = TOP + 2;

    if (saveGame(filename)) {
        GotoXY(infoX, infoY + 9);
        displayTimeOfFile(infoX, infoY + 9);
        GotoXY(infoX, infoY + (timeFl.size() > 0 ? (timeFl.size() + 10) : 10));
        std::cout << "                 ";
        GotoXY(infoX, infoY + (timeFl.size() > 0 ? (timeFl.size() + 11) : 11));
        langChoice == 1 ? std::cout << "Esc: Exit to Menu" : std::cout << "Esc: Thoát Menu";
    }
    else {
        langChoice == 1 ? displayMessage("Save failed!", msg_x, msg_y) : displayMessage("Lưu thất bại!", msg_x, msg_y);
        Sleep(1000);
        clearMessage(msg_x, msg_y, 30);
    }
    int screenX = LEFT + cursorCol * 4 + 2;
    int screenY = TOP + cursorRow * 2 + 1;
    GotoXY(screenX, screenY);
}

static void handleLoad(bool startFromMenu) {
    int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    int msg_y = TOP + 12 + (timeFl.size() > 0 ? timeFl.size() + 1 + 2 : 2);
    if (startFromMenu) system("cls");

    std::string filename;

    GotoXY(msg_x, msg_y);
    langChoice == 1 ? std::cout << "Enter the file name to load (Maxium 10 character): " : std::cout << "Nhập tên file muốn tải (tối đa 10 kí tự): ";
    std::cin >> filename;

    clearMessage(msg_x, msg_y, 60);

    if (loadGame(filename)) {
        if (startFromMenu) {
            runGameLoop();
        }
        else {
            redrawGameScreen();
        }
    }
    else {
        langChoice == 1 ? displayMessage("Load failed!", msg_x, msg_y) : displayMessage("Tải thất bại!", msg_x, msg_y);
        Sleep(1000);
        clearMessage(msg_x, msg_y, 20);
        int screenX = LEFT + cursorCol * 4 + 2;
        int screenY = TOP + cursorRow * 2 + 1;
        GotoXY(screenX, screenY);
    }
}

static void handleRename() {
    int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    int msg_y = TOP + 12 + (timeFl.size() > 0 ? timeFl.size() + 1 + 2 : 2);

    std::string filenameOld, filenameNew;

    GotoXY(msg_x, msg_y);
    langChoice == 1 ? std::cout << "Enter the file name to rename (Maxium 10 character): " : std::cout << "Nhập tên file muốn đổi tên (tối đa 10 kí tự): ";
    std::cin >> filenameOld;
    GotoXY(msg_x, msg_y + 1);
    langChoice == 1 ? std::cout << "Enter the name to rename (Maxium 10 character): " : std::cout << "Nhập tên mới của file ( tối đa 10 kí tự): ";
    std::cin >> filenameNew;

    clearMessage(msg_x, msg_y, 60);
    clearMessage(msg_x, msg_y + 1, 60);

    int tmpSize = timeFl.size();

    if (renameGame(filenameOld, filenameNew)) {
        int infoX = LEFT + BOARD_SIZE * 4 + 5;
        int infoY = TOP + 2;

        for (int i = infoY + 9; i <= infoY + tmpSize + 2 + 9; ++i) {
            clearMessage(infoX, i, 59);
        }

        GotoXY(infoX, infoY + 9);
        displayTimeOfFile(infoX, infoY + 9);
        GotoXY(infoX, infoY + (timeFl.size() > 0 ? timeFl.size() + 11 : 11));
        langChoice == 1 ? std::cout << "Esc: Exit to Menu" : std::cout << "Esc: Thoát Menu";
    }
    else {
        langChoice == 1 ? displayMessage("Rename failed!", msg_x, msg_y) : displayMessage("Đổi tên thất bại", msg_x, msg_y);
        Sleep(1000);
        clearMessage(msg_x, msg_y + 1, 30);
    }
    int screenX = LEFT + cursorCol * 4 + 2;
    int screenY = TOP + cursorRow * 2 + 1;
    GotoXY(screenX, screenY);
}


static void handleDelete() {
    int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    int msg_y = TOP + 12 + (timeFl.size() > 0 ? timeFl.size() + 1 + 2 : 2);

    std::string filename;

    GotoXY(msg_x, msg_y);
    langChoice == 1 ? std::cout << "Enter the file name to delete (Maxium 10 character): " : std::cout << "Nhập tên file muốn xóa (tối đa 10 kí tự): ";
    std::cin >> filename;

    clearMessage(msg_x, msg_y, 60);

    int tmpSize = timeFl.size();

    if (deleteGame(filename)) {
        int infoX = LEFT + BOARD_SIZE * 4 + 5;
        int infoY = TOP + 2;

        for (int i = infoY + 9; i <= infoY + tmpSize + 2 + 9; ++i) {
            clearMessage(infoX, i, 59);
        }

        GotoXY(infoX, infoY + 9);
        displayTimeOfFile(infoX, infoY + 9);
        GotoXY(infoX, infoY + (timeFl.size() > 0 ? timeFl.size() + 11 : 11));
        langChoice == 1 ? std::cout << "Esc: Exit to Menu" : std::cout << "Esc: Thoát Menu";
    }
    else {
        langChoice == 1 ? displayMessage("Delete failed!", msg_x, msg_y) : displayMessage("Xóa thất bại", msg_x, msg_y);
        Sleep(1000);
        clearMessage(msg_x, msg_y + 1, 30);
    }
    int screenX = LEFT + cursorCol * 4 + 2;
    int screenY = TOP + cursorRow * 2 + 1;
    GotoXY(screenX, screenY);
}

static void endGame(int result, int player) {
    if (result == 0) {
        AudioManager::getInstance().playSound(SoundEffect::Draw);
    }
    else {
        AudioManager::getInstance().playSound(SoundEffect::Win);
    }
    showWinEffect(result, player);
    if (askContinue()) {
        resetData();
        runGameLoop();
    }
}

static void redrawGameScreen() {
    drawBoard();
    showPlayerInfo();
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board[r][c].c != 0) {
                updateCellAtBoardIndex(c, r, board[r][c].c);
            }
        }
    }
    int screenX = LEFT + cursorCol * 4 + 2;
    int screenY = TOP + cursorRow * 2 + 1;
    GotoXY(screenX, screenY);
}

static inline void makeMove(int x, int y) {
    AudioManager::getInstance().playSound(SoundEffect::Move);
    board[x][y].c = turn;
    turn = -turn;
    cursorRow = x;
    cursorCol = y;
    --remains;
    updateCellAtBoardIndex(cursorCol, cursorRow, -turn);
    showPlayerInfo();
    int screenX = LEFT + cursorCol * 4 + 2;
    int screenY = TOP + cursorRow * 2 + 1;
    GotoXY(screenX, screenY);
}

void handleBotPlay() {
    AudioManager::getInstance().stopBackgroundMusic();
    bool exitRequested = false;
    redrawGameScreen();
    while (!exitRequested) {
        exitRequested = processInput();
        int result = 2, ongoing = 1;
        if (okToCheck) {
            result = getGameState(board, _POINT{cursorRow, cursorCol, -turn});
            ongoing = 0;
            okToCheck = 0;
        }
        if (result != 2) {
            endGame(result,-turn);
            return;
        }
        if (!ongoing) {
            pii botMove = getBestMove(board);
            makeMove(botMove.ff, botMove.ss);
            result = getGameState(board, _POINT{cursorRow, cursorCol, -turn});
        }
        if (result != 2) {
            endGame(result,-turn);
            return;
        }
    }
}
