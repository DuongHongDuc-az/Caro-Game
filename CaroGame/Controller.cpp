#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "AI.h"
#include "AudioManager.h"
#include "Interface.h"
// #include <conio.h>
// #include <windows.h>
#include <string>
#include <iostream>

int dKey = 0;
int rKey = 0;
int lKey = 0;
int tKey = 0;
int diffChoice = 0;
int cursorCol = 0;
int cursorRow = 0;

static int bot = -1;
static int pressL = 0;
static int pressT = 0;
static bool okToCheck = 0;
static int statusOfGame = 0;
static std::string inputString;

static void moveCursor(int direction);
static bool processInput(const sf::Event &event, int preMenu);
static void handleTurn();
void handleSave(const sf::Event &event);
void handleLoad(bool startFromMenu, const sf::Event &event);
void handleRename(const sf::Event& event);
void handleDelete();
static void endGame(const sf::Event& event, int result, int player, int playWBot = 0);
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

    //std::cout << choice << "\n";

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
    switch (choice) {
    case 1: {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyReleased>()) {
            if (keyPressed->code == sf::Keyboard::Key::Space) currentMenu = 20;
            if (keyPressed->code == sf::Keyboard::Key::Escape) currentMenu = 1;
        }
        
        //int done = 0;
        //while (!done)
        //{
        //    int modeChoice = showModeMenu();
        //    switch (modeChoice)
        //    {
        //    case 1:
        //        randomizeSideImage();
        //        resetData();
        //        runGameLoop(event);
        //        break;
        //    case 2:
        //    {
                //int diffChoice = showModeMenu(1);
        //        if (diffChoice < 4)
        //            DIFF = diffChoice;
        //        else
        //            break;
        //        randomizeSideImage();
        //        resetData();
        //        handleBotPlay(event);
        //        done = 1;
        //        break;
        //    }
        //    case 3:
        //        done = 1;
        //        break;
        //    }
        //}
        break;
    }
    case 2:
    {
        currentMenu = 3;
        // handleLoad(true);
        break;
    }
    case 21:
    {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyReleased>()) {
            if (currentMenu == 20 && keyPressed->code == sf::Keyboard::Key::Space) {
                currentMenu = 21;
                //sMM = 211;
            }
        }

        //int diffChoice = 0;

        if (0 < diffChoice && diffChoice < 4) DIFF = diffChoice;
        else break;
        //        randomizeSideImage();
        if (const auto* keyPressed = event.getIf<sf::Event::KeyReleased>()) {
            if (currentMenu == 21 && keyPressed->code == sf::Keyboard::Key::Space) {
                currentMenu = 22;
                sMM = 211;
            }
        }
        //resetData();
        //handleBotPlay(event);

        //done = 1;
//        break;
//    }
//    case 3:
//        done = 1;
        break;
    }
    case 211: {
        ++statusOfGame;

        if (statusOfGame == 1)
        {
            resetData();
        }
        else
            statusOfGame = 2;
        handleBotPlay(event); 

        break;
    }
    case 22:
    {
        if (loadFromMenu == 1) {
            if (const auto* keyPressed = event.getIf<sf::Event::KeyReleased>()) {
                if (keyPressed->code == sf::Keyboard::Key::L) {
                    currentMenu = 22;
                }
            }
        }
        else currentMenu = 22;

        ++statusOfGame;
        // randomizeSideImage();
        if (statusOfGame == 1 && loadFromMenu != 1)
        {
            resetData();
        }
        else
            statusOfGame = 2;

        runGameLoop(event);
        break;
    }
    case 3:
    {
        currentMenu = 4;
        // showAbout();
        break;
    }
    case 32:
    {
        currentMenu = 22;
        runGameLoop(event);
        break;
    }
    case 4:
    {
        currentMenu = 5;
        handleSettings();
        break;
    }
    case 0:
    {
        currentMenu = -1;
        return;
    }
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
     
    processInput(event, 20);
    int result = 2;
    if (okToCheck)
    {
        result = getGameState(board, _POINT{cursorRow, cursorCol, -turn});
        okToCheck = 0;
    }
    if (result != 2)
    {
        endGame(event, result, -turn);
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

static bool processInput(const sf::Event &event, int preMenu)
{
    if (currentMenu == 22)
    {
        //std::cout << lKey << " " << tKey << "\n";
        if (isKeyDown(Key::Up) && lKey == 0 && tKey == 0)
            moveCursor(0);
        if (isKeyDown(Key::Down) && lKey == 0 && tKey == 0)
            moveCursor(1);
        if (isKeyDown(Key::Left) && lKey == 0 && tKey == 0)
            moveCursor(2);
        if (isKeyDown(Key::Right) && lKey == 0 && tKey == 0)
            moveCursor(3);

        if (isKeyDown(Key::W) && lKey == 0 && tKey == 0)
            moveCursor(0);
        if (isKeyDown(Key::S) && lKey == 0 && tKey == 0)
            moveCursor(1);
        if (isKeyDown(Key::A) && lKey == 0 && tKey == 0)
            moveCursor(2);
        if (isKeyDown(Key::D) && lKey == 0 && tKey == 0)
            moveCursor(3);

        if (isKeyDown(Key::Enter) && lKey == 0 && tKey == 0)
            handleTurn();

        if (isKeyDown(Key::L) && tKey == 0) lKey = 1;
            //if (const auto* keyPressed = event.getIf<sf::Event::KeyReleased>()) {
            //    if (lKey == 0 && keyPressed->code == sf::Keyboard::Key::L) lKey = 1;
            //}
        if (isKeyDown(Key::T) && lKey == 0)
            tKey = 1;

        //if (isKeyDown(Key::Escape)) {
            if (const auto* keyPressed = event.getIf<sf::Event::KeyReleased>()) {
                {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) {
                        statusOfGame = 0;
                        currentMenu = preMenu;
                        sMM = preMenu;
                        return true;
                    }
                }
                
            }            
            
        //}
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
    cursor.setFillColor(sf::Color::Black);
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
    showInputText(1, event);
    drawRec = 1;
    // int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    // int msg_y = TOP + 12 + (timeFl.size() > 0 ? timeFl.size() + 1 + 2 : 2);
    // int msg_y = TOP + 12;
    //int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    //int msg_y = TOP + 200;

    // GotoXY(msg_x, msg_y);
    // langChoice == 1 ? std::cout << "Enter the file name to save (Maxium 10 character): " : std::cout << "Nhập tên file muốn lưu tối đa 10 kí tự: ";
    // std::cin >> filename;

    //saveText.setFillColor(sf::Color::White);
    //saveText.setString(langChoice == 1 ? "Enter the file name to save (Maxium 10 character): " : "Nhập tên file muốn lưu tối đa 10 kí tự: ");
    //saveText.setPosition(sf::Vector2f({(float)msg_x, (float)msg_y}));
    // clearMessage(msg_x, msg_y, 60);

    //inputText.setFillColor(sf::Color::Blue);
    //inputText.setPosition(sf::Vector2f({(float)msg_x, (float)TOP + 250}));
    //if (const sf::Event::TextEntered *textEventData = event.getIf<sf::Event::TextEntered>())
    //{
    //    uint32_t unicode = textEventData->unicode;

    //    if (unicode == 8)
    //    {
    //        if (!inputString.empty())
    //            inputString.pop_back();
    //    }
    //    else if (unicode == 13)
    //    {
    //        res = saveGame(inputString);
    //        lKey = 0;
    //        pressL = 0;
    //    }
    //    else if (unicode >= 32 && unicode != 127)
    //    {
    //        ++pressL;
    //        if (pressL != 1 && inputString.length() < 11)
    //        {
    //            inputString += static_cast<char>(unicode);
    //            pressL = 2;
    //        }
    //    }
    //    inputText.setString(inputString);
    //}

    // int infoX = LEFT + BOARD_SIZE * 4 + 5;
    // int infoY = TOP + 2;

    //if (res == 1)
    //{
        // GotoXY(infoX, infoY + 9);
        // displayTimeOfFile(infoX, infoY + 9);
        // GotoXY(infoX, infoY + (timeFl.size() > 0 ? (timeFl.size() + 10) : 10));
        // std::cout << "                 ";
        // GotoXY(infoX, infoY + (timeFl.size() > 0 ? (timeFl.size() + 11) : 11));
        // langChoice == 1 ? std::cout << "Esc: Exit to Menu" : std::cout << "Esc: Thoát Menu";
        //for (size_t i = 0; i < nameOfFile.size(); ++i)
        //    nameFile.setString(nameOfFile[i]);

        //inputText.setString("");
        //saveText.setString("");
        //inputString = "";
    //}
    //else if (res == 0)
    //{
        // langChoice == 1 ? displayMessage("Save failed!", msg_x, msg_y) : displayMessage("Lưu thất bại!", msg_x, msg_y);
        // Sleep(1000);
        // clearMessage(msg_x, msg_y, 30);

    //    inputText.setString("");
    //    saveText.setString("");
    //    inputString = "";
    //}
    // int screenX = LEFT + cursorCol * 4 + 2;
    // int screenY = TOP + cursorRow * 2 + 1;
    // GotoXY(screenX, screenY);
}

int handleLoadMiniBoard(size_t num) {
    char p = '0';

    if (nameOfFile.size() < 1) {
        std::ifstream nOF(L"name_of_file.txt");
        if (!nOF.is_open()) return 0;

        int sizeNameOfFile = 0;

        nOF >> sizeNameOfFile;
        for (int i = 0; i < sizeNameOfFile; ++i) {
            std::string ws;
            int m = -1;

            nOF >> m >> ws;
            nameOfFile.push_back(ws);

            if (ws == nameOfFile[num]) p = m + '0';
        }

        nOF.close();
    }
    else {
        for (int i = 0; i < nameOfFile.size(); ++i) {
            if (nameOfFile[i] == nameOfFile[num]) {
                p = i + '0';
                break;
            }
        }
    }

    std::string s{ p };

    std::ifstream f(s);
    if (!f.is_open()) return 0;

    f >> turnMini;
    f >> remainsMini;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            f >> boardMini[i][j].c;
        }
    }

    f.close();
    
    return 1;
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
    //wstring titleText = (langChoice == 1 ? L"LOAD GAME" : L"TẢI TRÒ CHƠI");

    //sf::FloatRect bounds = titleLoad.getLocalBounds();

    //titleLoad.setOrigin(bounds.getCenter());
    //titleLoad.setPosition(sf::Vector2f({ WINDOW_W / 2, OPAREC_Y - titleLoad.getCharacterSize() / 2 }));
    //titleLoad.setString(titleText);
    //titleLoad.setFillColor(sf::Color::Red);

    //loadText.setFillColor(sf::Color::White);
    //if (!startFromMenu) loadText.setString(langChoice == 1 ? "Enter the file name to load (Maxium 10 character): " : "Nhập tên file muốn tải tối đa 10 kí tự: ");
    //loadText.setPosition(sf::Vector2f({(float)msg_x, (float)msg_y}));
    //// clearMessage(msg_x, msg_y, 60);

    //inputText.setFillColor(sf::Color::Blue);
    //inputText.setPosition(sf::Vector2f({(float)msg_x, (float)TOP + 250}));
    //int res = -1;

    //if (const sf::Event::TextEntered *textEventData = event.getIf<sf::Event::TextEntered>())
    //{
    //    uint32_t unicode = textEventData->unicode;

    //    if (unicode == 8)
    //    {
    //        if (!inputString.empty())
    //            inputString.pop_back();
    //    }
    //    else if (unicode == 13 || (unicode == 76 && startFromMenu == 1))
    //    {
    //        if (unicode == 13) {
    //            res = loadGame(inputString);
    //            inputString = "";
    //            tKey = 0;
    //            pressT = 0;
    //        }
    //        else {

    //        }
    //    }
    //    else if (unicode >= 32 && unicode != 127)
    //    {
    //        ++pressT;
    //        if ((pressT != 1 || startFromMenu == 1)  && inputString.length() < 11)
    //        {
    //            inputString += static_cast<char>(unicode);
    //            pressT = 2;
    //        }
    //    }
    //    inputText.setString(inputString);
    //}

    //if (res == 1) {
    //    if (startFromMenu)
    //    {
    //        inputText.setString("");
    //        loadText.setString("");
    //        inputString = "";
    //        cursor.setString(turn == 1 ? "X" : "O");
    //        //Can chinh them khi playing with bot
    //        sMM = 32;

    //        //runGameLoop(event);
    //    }
    //    else
    //    {
    //        inputText.setString("");
    //        loadText.setString("");
    //        inputString = "";

    //        redrawGameScreen();
    //    }
    //}
}

void handleRename(const sf::Event& event)
{
    showInputText(3, event);
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

void handleDelete()
{
    deleteGame(delFile);
    dKey = 0;
    //colorBackGround = sf::Color::Cyan;
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

static void endGame(const sf::Event& event, int result, int player, int playWBot)
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
    bot = playWBot;
    //if (askCon == 1)
    //{
    //    resetData();
    //    if (!playWBot)
    //        runGameLoop(event);
    //    else
    //        handleBotPlay(event);
    //}
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

void handleBotPlay(const sf::Event& event)
{
    //AudioManager::getInstance().stopBackgroundMusic();
    bool exitRequested = false;
    //colorBackGround = sf::Color::Green;
    //redrawGameScreen();
    //while (!exitRequested)
    //{
        //exitRequested = 
        processInput(event, 21);
        int result = 2, ongoing = 1;
        if (okToCheck)
        {
            result = getGameState(board, _POINT{cursorRow, cursorCol, -turn});
            ongoing = 0;
            okToCheck = 0;
        }
        if (result != 2)
        {
            endGame(event, result, -turn, 1);
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
            endGame(event, result, -turn, 1);
            return;
        }
    //}
}

void ansContinue(const sf::Event &event) {
    if (askCon == 1)
    {
        res = -1;
        askCon = 0;
        //colorBackGround = sf::Color::Magenta;
        resetData();
        if (!bot)
            runGameLoop(event);
        else
            handleBotPlay(event);
    }
}
