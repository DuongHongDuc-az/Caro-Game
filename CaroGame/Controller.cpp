#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "AI.h"
#include "AudioManager.h"
#include "giaoDien.h"
// #include <conio.h>
// #include <windows.h>
#include <string>
#include <iostream>

int lKey = 0;
int tKey = 0;

static int pressL = 0;
static int pressT = 0;
static int cursorCol = 0;
static int cursorRow = 0;
static bool okToCheck = 0;
static int statusOfGame = 0;
static std::wstring inputString;

static void moveCursor(int direction);
static bool processInput(const sf::Event &event);
static void handleTurn();
void handleSave(const sf::Event &event);
void handleLoad(bool startFromMenu, const sf::Event &event);
static void handleRename();
static void handleDelete();
static void endGame(int result, int player, int playWBot = 0, const sf::Event &event);
static void redrawGameScreen();
static void runGameLoop(const sf::Event &event);

bool isKeyDown(sf::Keyboard::Key key)
{
    return sf::Keyboard::isKeyPressed(key);
}

static void handleSettings()
{
    bool exitSettings = false;
    AudioManager &audio = AudioManager::getInstance();

    // while (!exitSettings) {
    // int choice = (langChoice == 1) ? showSettingsMenu() : showMenuSettings();
    int choice = handleVol();

    std::cout << choice << "\n";

    switch (choice)
    {
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
        currentMenu = 1;
        sMM = 100;
        break;
    default:
        currentMenu = 5;
        break;
    }
}
extern void randomizeSideImage();

void run(const sf::Event &event)
{
    // fixConsoleWindow();

    int choice = sMM;
    // while (true) {
    switch (choice)
    {
    case 1:
    {
        currentMenu = 20;
        int done = 0;
        while (!done)
        {
            int modeChoice = showModeMenu();
            switch (modeChoice)
            {
            case 1:
                randomizeSideImage();
                resetData();
                runGameLoop();
                break;
            case 2:
            {
                int diffChoice = showModeMenu(1);
                if (diffChoice < 4)
                    DIFF = diffChoice;
                else
                    break;
                randomizeSideImage();
                resetData();
                handleBotPlay();
                done = 1;
                break;
            }
            case 3:
                done = 1;
                break;
            }
        }
        break;
    }
    case 2:
        currentMenu = 3;
        // handleLoad(true);
        break;
    case 21:
        currentMenu = 21;
        break;
    case 22:
        currentMenu = 22;
        ++statusOfGame;
        // randomizeSideImage();

        if (statusOfGame == 1)
        {
            resetData();
        }
        else
            statusOfGame = 2;

        runGameLoop(event);
        break;
    case 3:
        currentMenu = 4;
        // showAbout();
        break;
    case 4:
        currentMenu = 5;
        handleSettings();
        break;
    case 0:
        currentMenu = -1;
        colorBackGround = sf::Color::Yellow;
        return;
    default:
        break;
    }

    // if (langChoice == 1) {
    //     showMainMenu();
    //     choice = sMM;
    // }
    // else {
    //     hienMenuChinh();
    //     choice = sMM;
    // }
    //}
}

static void runGameLoop(const sf::Event &event)
{
    AudioManager::getInstance().stopBackgroundMusic();
    bool exitRequested = false;
    redrawGameScreen();

    // while (!exitRequested) {
    // exitRequested =
    processInput();
    int result = 2;
    if (okToCheck)
    {
        result = getGameState(board, _POINT{cursorRow, cursorCol, -turn});
        okToCheck = 0;
    }
    if (result != 2)
    {
        endGame(result, -turn);
        return;
    }
    //}
}

// static bool processInput(const sf::Event& event) {
// if (_kbhit()) {
//     int ch = _getch();
//     if (ch == 0 || ch == 224) {
//         int c2 = _getch();
//         if (c2 == 72) moveCursor(0);
//         else if (c2 == 80) moveCursor(1);
//         else if (c2 == 75) moveCursor(2);
//         else if (c2 == 77) moveCursor(3);
//         return false;
//     }
//
// if (currentMenu == 22) {
//     if (isKeyDown(Key::Up) && lKey != 1 && tKey != 1) moveCursor(0);
//     if (isKeyDown(Key::Down) && lKey != 1 && tKey != 1) moveCursor(1);
//     if (isKeyDown(Key::Left) && lKey != 1 && tKey != 1) moveCursor(2);
//     if (isKeyDown(Key::Right) && lKey != 1 && tKey != 1) moveCursor(3);

//    //    ch = toupper(ch);
//    //    switch (ch) {
//    //    case 'W': moveCursor(0); break;
//    //    case 'S': moveCursor(1); break;
//    //    case 'A': moveCursor(2); break;
//    //    case 'D': moveCursor(3); break;
//    //    case 13: handleTurn(); break;
//    //    case 'L': handleSave(); break;
//    //    case 'T': handleLoad(false); break;
//    //    case 'R': handleRename(); break;
//    //    case 'X': handleDelete(); break;
//    //    case 27:
//    //        return true;
//    //    default:
//    //        break;
//    //    }
//    //}

//    if (isKeyDown(Key::W) && lKey != 1 && tKey != 1) moveCursor(0);
//    if (isKeyDown(Key::S) && lKey != 1 && tKey != 1) moveCursor(1);
//    if (isKeyDown(Key::A) && lKey != 1 && tKey != 1) moveCursor(2);
//    if (isKeyDown(Key::D) && lKey != 1 && tKey != 1) moveCursor(3);
//    if (isKeyDown(Key::Enter) && lKey != 1 && tKey != 1) handleTurn();
//    //if (isKeyDown(Key::L)) handleSave(event);
//    if (isKeyDown(Key::L)) lKey = 1;
//    if (isKeyDown(Key::T)) tKey = 1;
//    //if (isKeyDown(Key::R)) handleRename();
//    //if (isKeyDown(Key::X)) handleDelete();
//    if (isKeyDown(Key::Escape)) return true;
//}

/*   return false;
}*/

static bool processInput(const sf::Event &event)
{
    if (currentMenu == 22)
    {
        if (isKeyDown(Key::Up) && lKey != 1 && tKey != 1)
            moveCursor(0);
        if (isKeyDown(Key::Down) && lKey != 1 && tKey != 1)
            moveCursor(1);
        if (isKeyDown(Key::Left) && lKey != 1 && tKey != 1)
            moveCursor(2);
        if (isKeyDown(Key::Right) && lKey != 1 && tKey != 1)
            moveCursor(3);

        if (isKeyDown(Key::W) && lKey != 1 && tKey != 1)
            moveCursor(0);
        if (isKeyDown(Key::S) && lKey != 1 && tKey != 1)
            moveCursor(1);
        if (isKeyDown(Key::A) && lKey != 1 && tKey != 1)
            moveCursor(2);
        if (isKeyDown(Key::D) && lKey != 1 && tKey != 1)
            moveCursor(3);

        if (isKeyDown(Key::Enter) && lKey != 1 && tKey != 1)
            handleTurn();

        if (isKeyDown(Key::L))
            lKey = 1;
        if (isKeyDown(Key::T))
            tKey = 1;

        if (isKeyDown(Key::Escape))
            return true;
    }
    return false;
}

static void moveCursor(int direction)
{
    if (direction == 0 && cursorRow > 0)
        cursorRow--;
    else if (direction == 1 && cursorRow < BOARD_SIZE - 1)
        cursorRow++;
    else if (direction == 2 && cursorCol > 0)
        cursorCol--;
    else if (direction == 3 && cursorCol < BOARD_SIZE - 1)
        cursorCol++;

    // int screenX = LEFT + cursorCol * 4 + 2;
    // int screenY = TOP + cursorRow * 2 + 1;
    // GotoXY(screenX, screenY);

    int screenX = startX + (cursorCol + 1) * 22 + 48 * (cursorCol);
    int screenY = startY + (cursorRow + 1) * 11 + 59 * (cursorRow);

    // std::cout << "Con tro: " << screenX << " " << screenY << "\n";

    cursor.setString(turn == 1 ? "X" : "O");
    cursor.setFillColor(sf::Color(173, 173, 173));
    cursor.setPosition(sf::Vector2f({(float)screenX, (float)screenY}));
}

static inline void upBound(int x, int y)
{
    optimize(tlB.x, x, 1);
    optimize(tlB.y, y, 1);
    optimize(brB.x, x);
    optimize(brB.y, y);
}

static void handleTurn()
{
    if (takeTurn(cursorCol, cursorRow))
    {
        upBound(cursorRow, cursorCol);
        AudioManager::getInstance().playSound(SoundEffect::Move);
        int previous_turn = (turn == 1) ? -1 : 1;
        updateCellAtBoardIndex(cursorCol, cursorRow, previous_turn);
        okToCheck = 1;
        showPlayerInfo();
        int screenX = LEFT + cursorCol * 4 + 2;
        int screenY = TOP + cursorRow * 2 + 1;
        GotoXY(screenX, screenY);
        // int previous_turn = (turn == 1) ? -1 : 1;
        // updateCellAtBoardIndex(cursorCol, cursorRow, previous_turn);
        // showPlayerInfo();
        // int screenX = LEFT + cursorCol * 4 + 2;
        // int screenY = TOP + cursorRow * 2 + 1;
        // GotoXY(screenX, screenY);
    }
}

void handleSave(const sf::Event &event)
{
    // int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    // int msg_y = TOP + 12 + (timeFl.size() > 0 ? timeFl.size() + 1 + 2 : 2);
    // int msg_y = TOP + 12;
    int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    int msg_y = TOP + 200;

    // GotoXY(msg_x, msg_y);
    // langChoice == 1 ? std::cout << "Enter the file name to save (Maxium 10 character): " : std::cout << "Nhập tên file muốn lưu tối đa 10 kí tự: ";
    // std::cin >> filename;

    saveText.setFillColor(sf::Color::Black);
    saveText.setString(langChoice == 1 ? "Enter the file name to save (Maxium 10 character): " : "Nhập tên file muốn lưu tối đa 10 kí tự: ");
    saveText.setPosition(sf::Vector2f({(float)msg_x, (float)msg_y}));
    // clearMessage(msg_x, msg_y, 60);

    inputText.setFillColor(sf::Color::Blue);
    inputText.setPosition(sf::Vector2f({(float)msg_x, (float)TOP + 250}));
    int res = -1;

    if (const sf::Event::TextEntered *textEventData = event.getIf<sf::Event::TextEntered>())
    {
        uint32_t unicode = textEventData->unicode;

        if (unicode == 8)
        {
            if (!inputString.empty())
                inputString.pop_back();
        }
        else if (unicode == 13)
        {
            res = saveGame(inputString);
            lKey = 0;
            pressL = 0;
        }
        else if (unicode >= 32 && unicode != 127)
        {
            ++pressL;
            if (pressL != 1 && inputString.length() < 11)
            {
                inputString += static_cast<wchar_t>(unicode);
                pressL = 2;
            }
        }
        inputText.setString(inputString);
    }

    // int infoX = LEFT + BOARD_SIZE * 4 + 5;
    // int infoY = TOP + 2;

    if (res == 1)
    {
        // GotoXY(infoX, infoY + 9);
        // displayTimeOfFile(infoX, infoY + 9);
        // GotoXY(infoX, infoY + (timeFl.size() > 0 ? (timeFl.size() + 10) : 10));
        // std::cout << "                 ";
        // GotoXY(infoX, infoY + (timeFl.size() > 0 ? (timeFl.size() + 11) : 11));
        // langChoice == 1 ? std::cout << "Esc: Exit to Menu" : std::cout << "Esc: Thoát Menu";

        nameFile.setFillColor(sf::Color::Green);
        nameFile.setPosition(sf::Vector2f({(float)msg_x, (float)msg_y}));
        for (size_t i = 0; i < nameOfFile.size(); ++i)
            nameFile.setString(nameOfFile[i]);

        inputText.setString("");
        saveText.setString("");
        inputString = L"";
    }
    else if (res == 0)
    {
        // langChoice == 1 ? displayMessage("Save failed!", msg_x, msg_y) : displayMessage("Lưu thất bại!", msg_x, msg_y);
        // Sleep(1000);
        // clearMessage(msg_x, msg_y, 30);

        inputText.setString("");
        saveText.setString("");
        inputString = L"";
    }
    // int screenX = LEFT + cursorCol * 4 + 2;
    // int screenY = TOP + cursorRow * 2 + 1;
    // GotoXY(screenX, screenY);
}

void handleLoad(bool startFromMenu, const sf::Event &event)
{
    int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    int msg_y = TOP + 200;
    // if (startFromMenu) system("cls");

    // GotoXY(msg_x, msg_y);
    // langChoice == 1 ? std::cout << "Enter the file name to load (Maxium 10 character): " : std::cout << "Nhập tên file muốn tải (tối đa 10 kí tự): ";
    // std::cin >> filename;

    // clearMessage(msg_x, msg_y, 60);

    // if (loadGame(filename)) {
    //     if (startFromMenu) {
    //         runGameLoop();
    //     }
    //     else {
    //         redrawGameScreen();
    //     }
    // }
    // else {
    //     langChoice == 1 ? displayMessage("Load failed!", msg_x, msg_y) : displayMessage("Tải thất bại!", msg_x, msg_y);
    //     Sleep(1000);
    //     clearMessage(msg_x, msg_y, 20);
    //     int screenX = LEFT + cursorCol * 4 + 2;
    //     int screenY = TOP + cursorRow * 2 + 1;
    //     GotoXY(screenX, screenY);
    // }

    loadText.setFillColor(sf::Color::Black);
    loadText.setString(langChoice == 1 ? "Enter the file name to load (Maxium 10 character): " : "Nhập tên file muốn tải tối đa 10 kí tự: ");
    loadText.setPosition(sf::Vector2f({(float)msg_x, (float)msg_y}));
    // clearMessage(msg_x, msg_y, 60);

    inputText.setFillColor(sf::Color::Blue);
    inputText.setPosition(sf::Vector2f({(float)msg_x, (float)TOP + 250}));
    int res = -1;

    if (const sf::Event::TextEntered *textEventData = event.getIf<sf::Event::TextEntered>())
    {
        uint32_t unicode = textEventData->unicode;

        if (unicode == 8)
        {
            if (!inputString.empty())
                inputString.pop_back();
        }
        else if (unicode == 13)
        {
            res = loadGame(inputString);
            inputString = L"";
            tKey = 0;
            pressT = 0;
        }
        else if (unicode >= 32 && unicode != 127)
        {
            ++pressT;
            if (pressT != 1 && inputString.length() < 11)
            {
                inputString += static_cast<wchar_t>(unicode);
                pressT = 2;
            }
        }
        inputText.setString(inputString);
    }

    if (res)
    {
        if (startFromMenu)
        {
            inputText.setString("");
            loadText.setString("");
            inputString = L"";

            runGameLoop(event);
        }
        else
        {
            inputText.setString("");
            loadText.setString("");
            inputString = L"";

            redrawGameScreen();
        }
    }
}

static void handleRename()
{
    // int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    // int msg_y = TOP + 12 + (timeFl.size() > 0 ? timeFl.size() + 1 + 2 : 2);

    // std::string filenameOld, filenameNew;

    // GotoXY(msg_x, msg_y);
    // langChoice == 1 ? std::cout << "Enter the file name to rename (Maxium 10 character): " : std::cout << "Nhập tên file muốn đổi tên (tối đa 10 kí tự): ";
    // std::cin >> filenameOld;
    // GotoXY(msg_x, msg_y + 1);
    // langChoice == 1 ? std::cout << "Enter the name to rename (Maxium 10 character): " : std::cout << "Nhập tên mới của file ( tối đa 10 kí tự): ";
    // std::cin >> filenameNew;

    // clearMessage(msg_x, msg_y, 60);
    // clearMessage(msg_x, msg_y + 1, 60);

    // int tmpSize = timeFl.size();

    // if (renameGame(filenameOld, filenameNew)) {
    //     int infoX = LEFT + BOARD_SIZE * 4 + 5;
    //     int infoY = TOP + 2;

    //    for (int i = infoY + 9; i <= infoY + tmpSize + 2 + 9; ++i) {
    //        clearMessage(infoX, i, 59);
    //    }

    //    GotoXY(infoX, infoY + 9);
    //    displayTimeOfFile(infoX, infoY + 9);
    //    GotoXY(infoX, infoY + (timeFl.size() > 0 ? timeFl.size() + 11 : 11));
    //    langChoice == 1 ? std::cout << "Esc: Exit to Menu" : std::cout << "Esc: Thoát Menu";
    //}
    // else {
    //    langChoice == 1 ? displayMessage("Rename failed!", msg_x, msg_y) : displayMessage("Đổi tên thất bại", msg_x, msg_y);
    //    Sleep(1000);
    //    clearMessage(msg_x, msg_y + 1, 30);
    //}
    // int screenX = LEFT + cursorCol * 4 + 2;
    // int screenY = TOP + cursorRow * 2 + 1;
    // GotoXY(screenX, screenY);
}

static void handleDelete()
{
    // int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    // int msg_y = TOP + 12 + (timeFl.size() > 0 ? timeFl.size() + 1 + 2 : 2);

    // std::wstring filename;

    // GotoXY(msg_x, msg_y);
    // langChoice == 1 ? std::cout << "Enter the file name to delete (Maxium 10 character): " : std::cout << "Nhập tên file muốn xóa (tối đa 10 kí tự): ";
    // std::cin >> filename;

    // clearMessage(msg_x, msg_y, 60);

    // int tmpSize = timeFl.size();

    // if (deleteGame(filename)) {
    //     int infoX = LEFT + BOARD_SIZE * 4 + 5;
    //     int infoY = TOP + 2;

    //    for (int i = infoY + 9; i <= infoY + tmpSize + 2 + 9; ++i) {
    //        clearMessage(infoX, i, 59);
    //    }

    //    GotoXY(infoX, infoY + 9);
    //    displayTimeOfFile(infoX, infoY + 9);
    //    GotoXY(infoX, infoY + (timeFl.size() > 0 ? timeFl.size() + 11 : 11));
    //    langChoice == 1 ? std::cout << "Esc: Exit to Menu" : std::cout << "Esc: Thoát Menu";
    //}
    // else {
    //    langChoice == 1 ? displayMessage("Delete failed!", msg_x, msg_y) : displayMessage("Xóa thất bại", msg_x, msg_y);
    //    Sleep(1000);
    //    clearMessage(msg_x, msg_y + 1, 30);
    //}
    // int screenX = LEFT + cursorCol * 4 + 2;
    // int screenY = TOP + cursorRow * 2 + 1;
    // GotoXY(screenX, screenY);
}

static void endGame(int result, int player, int playWBot, const sf::Event &event)
{

    if (result == 0)
    {
        AudioManager::getInstance().playSound(SoundEffect::Draw);
    }
    else
    {
        AudioManager::getInstance().playSound(SoundEffect::Win);
    }
    showWinEffect(result, player);
    if (askContinue())
    {
        resetData();
        if (!playWBot)
            runGameLoop();
        else
            handleBotPlay();
    }
}

static void redrawGameScreen()
{
    // drawBoard();
    // showPlayerInfo();
    for (int r = 0; r < BOARD_SIZE; ++r)
    {
        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            if (board[r][c].c != 0)
            {
                updateCellAtBoardIndex(c, r, board[r][c].c);
            }
        }
    }
    int screenX = LEFT + cursorCol * 4 + 2;
    int screenY = TOP + cursorRow * 2 + 1;
    // GotoXY(screenX, screenY);
}

static inline void makeMove(int x, int y)
{
    AudioManager::getInstance().playSound(SoundEffect::Move);
    board[x][y].c = turn;
    turn = -turn;
    cursorRow = x;
    cursorCol = y;
    --remains;
    upBound(cursorRow, cursorCol);
    player2.moves++;
    updateCellAtBoardIndex(cursorCol, cursorRow, -turn);
    showPlayerInfo();
}

void handleBotPlay()
{
    AudioManager::getInstance().stopBackgroundMusic();
    bool exitRequested = false;
    redrawGameScreen();
    while (!exitRequested)
    {
        exitRequested = processInput();
        int result = 2, ongoing = 1;
        if (okToCheck)
        {
            result = getGameState(board, _POINT{cursorRow, cursorCol, -turn});
            ongoing = 0;
            okToCheck = 0;
        }
        if (result != 2)
        {
            endGame(result, -turn, 1);
            return;
        }
        if (!ongoing)
        {
            int tmpX = cursorRow, tmpY = cursorCol;
            pii botMove = getBestMove(board);
            makeMove(botMove.ff, botMove.ss);
            result = getGameState(board, _POINT{cursorRow, cursorCol, -turn});
            cursorRow = tmpX;
            cursorCol = tmpY;
            int screenX = LEFT + cursorCol * 4 + 2;
            int screenY = TOP + cursorRow * 2 + 1;
            GotoXY(screenX, screenY);
        }
        if (result != 2)
        {
            endGame(result, -turn, 1);
            return;
        }
    }
}
