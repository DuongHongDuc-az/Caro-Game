#include "View.h"
#include "AudioManager.h"
#include "Controller.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cwchar>
using namespace std;

int res = -1;
int delFile = -1;
int drawRec = 0;
int oldFile = -1;
int loadFromMenu = 0;
int langChoice = 1;
int sMM = 100;
int cs = (BOARD_SIZE) * 70;
int askCon = 0;
float startX = (1500 - cs) / 2.0f;
float startY = (800 - cs) / 2.0f;

static int numFileShow = 5;
static int startShowFile = 0;
static int endShowFile = startShowFile + numFileShow;;
static int pressR = 0;
static int selectedItem = 0;
static int selectedFile = 0;
static int selectedDiff = 0;
static int selectedMode = 0;
static int selectedButton = -1;
static sf::Clock keyPressTimer;
//static sf::String inputString;
static std::string inputString;
static const float KEY_DELAY_SECONDS = 0.2f;

int handleVol(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyReleased>()) {
        if (keyPressed->code == sf::Keyboard::Key::Num1) {
            return 1;
        }
    }
    if (isKeyDown(Key::Num2))
        return 2;
    if (isKeyDown(Key::Num3))
        return 3;
    if (isKeyDown(Key::Num0))
        return 0;
    return -1;
}

void showSettingsMenu()
{
    // system("cls");
    // setColor(240);
    // drawSettingsArt();
    AudioManager &audio = AudioManager::getInstance();
    // int leftX = 18;
    // int topY = 12;
    // GotoXY(leftX, topY);
    // setColor(240 + 9);
    // cout << "\n\n\t\t =============== AUDIO SETTINGS ===============";
    // setColor(240);
    // GotoXY(leftX, topY + 3);
    // cout << "\n\t\t\t 1. Toggle Mute (Currently: "
    //     << (audio.isMuted() ? "MUTED" : "ON") << ")";
    // GotoXY(leftX, topY + 5);
    // cout << "\n\t\t\t 2. Decrease Volume";
    // GotoXY(leftX, topY + 6);
    // cout << "\n\t\t\t 3. Increase Volume";
    // GotoXY(leftX, topY + 8);
    // cout << "\n\n\t\t\t    Current Volume: " << audio.getVolumeLevel() << "%";
    // GotoXY(leftX, topY + 11);
    // cout << "\n\n\t\t\t 0. Back to Main Menu";
    // setColor(240 + 9);
    // cout << "\n\n\t\t ==============================================";
    // setColor(240);
    // GotoXY(leftX, topY + 14);
    // cout << "\n\n\t\t\t Enter your choice: ";

    vector<wstring> outSettingsMenu;
    wstring outSettingsMenuTitle = (langChoice == 1 ? L"SETTINGS" : L"CÀI ĐẶT");
    std::wostringstream woss;

    int soundLevel = (int)audio.getVolumeLevel();
    isSoundOn = audio.isMuted();
    isMoving = true;

    woss << soundLevel << L"%";

    if (langChoice == 1)
    {
        //const wchar_t *tggMute = audio.isMuted() ? L"MUTED" : L"ON";

        outSettingsMenu = {
            L"1. Toggle Mute",
            L"2. Decrease Volume",
            L"3. Increase Volume",
            L" ",
            L"ESC. Back to Main Menu"};
    }
    else
    {
        outSettingsMenu = {
            L"1. Bật / Tắt Âm ",
            L"2. Giảm âm lượng",
            L"3. Tăng âm lượng",
            L" ",
            L"ESC: Quay lại Menu Chính"};
    }

    sf::Text title(font, outSettingsMenuTitle, 80);
    sf::Text textSound(font, woss.str(), 80);
    sf::FloatRect bounds = title.getLocalBounds();

    title.setOrigin(bounds.getCenter());
    title.setPosition(sf::Vector2f({WINDOW_W / 2, OPAREC_Y - title.getCharacterSize() / 2}));
    title.setFillColor(sf::Color(35, 71, 139));
    textSound.setOrigin(bounds.getCenter());
    textSound.setPosition(sf::Vector2f({ (WINDOW_W - 800) / 2 + 950, OPAREC_Y - 60 + 75 * (float)(3 + 2) }));
    textSound.setFillColor(sf::Color::White);

    window.draw(textSound);
    window.draw(title);

    for (size_t i = 0; i < outSettingsMenu.size(); ++i)
    {
        sf::Text text(font, outSettingsMenu[i], 35);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f({WINDOW_W - OPAREC_X - OPAREC_W / 2 + 50, OPAREC_Y - 75 + 75 * (float)(i + 2)}));

        window.draw(text);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
    {
        currentMenu = 1;
        sMM = 100;
    }

    window.draw(toggleTrack);
    window.draw(toggleThumb);
    soundBar(soundLevel);
}
int showMenuSettings()
{
    // system("cls");
    // setColor(240);
    // drawSettingsArt();
    // AudioManager& audio = AudioManager::getInstance();
    // int leftX = 18;
    // int topY = 12;
    // GotoXY(leftX, topY);
    // setColor(240 + 9);
    // cout << "\n\n\t\t =============== CÀI ĐẶT ÂM THANH ===============";
    // setColor(240);
    // GotoXY(leftX, topY + 3);
    // cout << "\n\t\t\t 1. Bật/Tắt Âm (Hiện tại: "
    //     << (audio.isMuted() ? "TẮT" : "BÂT") << ")";
    // GotoXY(leftX, topY + 5);
    // cout << "\n\t\t\t 2. Giảm Âm Lượng";
    // GotoXY(leftX, topY + 6);
    // cout << "\n\t\t\t 3. Tăng Âm Lượng";
    // GotoXY(leftX, topY + 8);
    // cout << "\n\n\t\t\t    Âm Lượng Hiện Tại: " << audio.getVolumeLevel() << "%";
    // GotoXY(leftX, topY + 11);
    // cout << "\n\n\t\t\t 0. Quay lại Menu Chinh";
    // setColor(240 + 9);
    // cout << "\n\n\t\t ================================================";
    // setColor(240);
    // GotoXY(leftX, topY + 14);
    // cout << "\n\n\t\t\t Nhập lựa chọn cơ bản: ";

    int choice;
    cin >> choice;
    return choice;
}

void fixConsoleWindow()
{
    HWND consoleWindow = GetConsoleWindow();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    wcscpy_s(fontInfo.FaceName, L"Consolas");
    fontInfo.dwFontSize.X = 0;
    fontInfo.dwFontSize.Y = 16;
    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    COORD bufferSize = {120, 65};
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    SMALL_RECT windowSize = {0, 0, 119, 60};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
    SetWindowLong(consoleWindow, GWL_STYLE, style);
    HMENU hmenu = GetSystemMenu(consoleWindow, FALSE);
    EnableMenuItem(hmenu, SC_CLOSE, MF_ENABLED);
    RECT rectClient, rectWindow;
    GetClientRect(consoleWindow, &rectClient);
    GetWindowRect(consoleWindow, &rectWindow);
    int width = 1200;
    int height = 700;
    int posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    int posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
    MoveWindow(consoleWindow, posX, posY, width, height, TRUE);
}

void GotoXY(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void updateCellAtScreen(int screenX, int screenY, int player, int b)
{
    // void updateCellAtScreen() {
    // GotoXY(screenX, screenY);

    // if (player == 1) {
    // setColor(240+12);
    // cout << "X";

    // sf::Text X(font, "X", 35);

    // X.setPosition(sf::Vector2f({ (float)screenX, (float)screenY }));
    // X.setFillColor(sf::Color::Red);

    // window.draw(X);
    //}
    // else if (player == -1) {
    // setColor(240+10);
    // cout << "O";

    // sf::Text O(font, "O", 35);

    // O.setPosition(sf::Vector2f({ (float)screenX, (float)screenY }));
    // O.setFillColor(sf::Color::Green);

    // window.draw(O);
    //}
    // else {
    //    int col = (screenX - LEFT) / 4;
    //    int row = (screenY - TOP) / 2;

    // if ((row + col) % 2 == 0)
    // setColor(240+7);
    // else
    // setColor(240+8);
    // cout << " ";
    //}

    // setColor(240);

    for (int r = 0; r < BOARD_SIZE; ++r)
    {
        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            int piece = (b == 1 ? board[r][c].c : boardMini[r][c].c);
            float posX = 0, posY = 0;
            sf::Text currentPiece(font, "", 35);

            if (b == 1) {
                posX = startX + (c + 1) * 22 + 48 * (c);
                posY = startY + (r + 1) * 11 + 59 * (r);

                currentPiece.setCharacterSize(35);
                currentPiece.setFillColor(piece == 1 ? sf::Color(210, 4, 45) : sf::Color(0, 128, 0));
            }
            else {
                //int posX = 
                posX = WINDOW_W / 2 + 100 + (c + 1) * 17 + 38 * (c);
                
                if (currentMenu == 3) posY = WINDOW_H / 2 - 225 + (r + 1) * 7 + 49 * (r);
                else posY = WINDOW_H / 2 - 275 + (r + 1) * 7 + 49 * (r);
                //posY = WINDOW_H / 2 - 225;

                currentPiece.setCharacterSize(27.5);
                currentPiece.setFillColor(piece == 1 ? sf::Color::Red : sf::Color::Green);
            }            

            if (piece != 0)
            {
                currentPiece.setString(piece == 1 ? "X" : "O");
                currentPiece.setPosition(sf::Vector2f({(float)posX, (float)posY}));
                
                window.draw(currentPiece);
            }
        }
    }
}

void updateCellAtBoardIndex(int col, int row, int player)
{
    // int screenX = LEFT + col * 4 + 2;
    // int screenY = TOP + row * 2 + 1;
    int screenX = startX + col * 22 + 48 * (col - 1);
    int screenY = startY + row * 11 + 59 * (row - 1);
    // updateCellAtScreen(screenX, screenY, player);
    // updateCellAtScreen();
}

void drawBoard()
{
    // system("cls");
    // drawRightSideImage();
    // setColor(249);
    // sf::Text text(font, "", 20);
    // for (int i = 0; i <= BOARD_SIZE; ++i) {
    //     for (int j = 0; j <= BOARD_SIZE; ++j) {
    // GotoXY(LEFT + 4 * j, TOP + 2 * i);
    // if (i == 0 && j == 0) cout << "╔";
    // else if (i == 0 && j == BOARD_SIZE) cout << "╗";
    // else if (i == BOARD_SIZE && j == 0) cout << "╚";
    // else if (i == BOARD_SIZE && j == BOARD_SIZE) cout << "╝";
    // else if (i == 0) std::cout << "╦";
    // else if (i == BOARD_SIZE) cout << "╩";
    // else if (j == 0) std::cout << "╠";
    // else if (j == BOARD_SIZE) cout << "╣";
    // else cout << "╬";

    // if (j < BOARD_SIZE) cout << "═══";

    // if (i < BOARD_SIZE) {
    //     GotoXY(LEFT + 4 * j, TOP + 2 * i + 1);
    //     cout << "║";
    // }
    //}

    std::vector<sf::RectangleShape> drawBoard = createThickGrid(cs, cs, 70, sf::Color(35, 71, 139), 5, startX, startY);

    for (const auto &line : drawBoard)
    {
        window.draw(line);
    }

    // for (int r = 0; r < BOARD_SIZE; ++r) {
    //     for (int c = 0; c < BOARD_SIZE; ++c) {
    //         updateCellAtBoardIndex(c, r, 0);
    //     }
    // }
    // GotoXY(0, 0);
    // setColor(240);
}
void handleShowListOfFile()
{
    // setColor(240);
    std::string fileName;
    std::string timeSaved, dateSaved;

     std::ifstream f("timeFile.txt");
     std::ifstream nOF("name_of_file.txt");

     int numOfFile = 0;

     f >> numOfFile;
     nOF >> numOfFile;

     if (numOfFile < 1) return;

     timeFl.resize(numOfFile);
     nameOfFile.resize(numOfFile);

     for (int i = 0; i < nameOfFile.size(); ++i) {
         int m;
         nOF >> m >> fileName;
         f >> m >> dateSaved >> timeSaved;

         char p = m + '0';
         string s{ p };

         timeFl[i].first = s;
         timeFl[i].second.first = dateSaved;
         timeFl[i].second.second = timeSaved;
         nameOfFile[i] = fileName;
     }

     nOF.close();
     f.close();
}

void displayListOfFile() {
    handleShowListOfFile();

    if (timeFl.size() >= 1 && nameOfFile.size() >= 1) {
        sf::Text fileText(font, "", 30);
        float col1_X = WINDOW_W - OPAREC_X - OPAREC_W / 2 - 75;
        float col2_X = col1_X + 300;
        float col3_X = col2_X + 200;

        if (endShowFile > timeFl.size()) endShowFile = timeFl.size();

        for (int i = startShowFile; i < endShowFile; ++i) {
            float posY;

            if (endShowFile <= numFileShow) {
                if (currentMenu == 3) posY = OPAREC_Y - 50 + 65 * (float)(i + 2);
                else posY = OPAREC_Y - 100 + 65 * (float)(i + 2);
            }
            else {
                float tmp = i - ((endShowFile - 1) - (numFileShow - 1));
                if (currentMenu == 3) posY = OPAREC_Y - 50 + 65 * (float)(tmp + 2);
                else posY = OPAREC_Y - 100 + 65 * (float)(tmp + 2);
            }

            //if (selectedFile == (int)i) {
            if (selectedFile == i) {
                if (handleLoadMiniBoard(i)) {
                    updateCellAtScreen(1, 1, 1, 0);
                }
                else {
                    sf::Text text(font, langChoice == 1 ? L"Cann't load file!" : L"Không thể tải file!", 30);

                    text.setFillColor(sf::Color::Red);

                    window.draw(text);
                }
            }
            else {
                fileText.setFillColor(sf::Color::White);
            }

            //window.draw(fileText);

            sf::Color textColor = (selectedFile == (int)i) ? sf::Color::Yellow : sf::Color::White;
            fileText.setFillColor(textColor);

            fileText.setString(nameOfFile[i]);
            fileText.setPosition({ col1_X, posY });
            window.draw(fileText);

            fileText.setString(timeFl[i].second.first);
            fileText.setPosition({ col2_X, posY });
            window.draw(fileText);

            fileText.setString(timeFl[i].second.second);
            fileText.setPosition({ col3_X, posY });
            window.draw(fileText);
        }

        int itemChanged = 0;

        if (keyPressTimer.getElapsedTime().asSeconds() >= KEY_DELAY_SECONDS)
        {
            if (rKey != 1 && (isKeyDown(Key::W) || isKeyDown(Key::Up)))
            {
                --selectedFile;
                itemChanged = 1;
            }
            else if (rKey != 1 && (isKeyDown(Key::S) || isKeyDown(Key::Down)))
            {
                ++selectedFile;
                itemChanged = 1;
            }
        }

        if (currentMenu == 3) {
            if (dKey != 1 && rKey != 1 && lKey != 1 && isKeyDown(Key::L)) {
                //sMM = 22;
                loadFromMenu = 1;
                loadGame(nameOfFile[selectedFile]);
            }
            else if (dKey != 1 && rKey != 1 && lKey != 1 && isKeyDown(Key::R)) {
                rKey = 1;
                oldFile = selectedFile;
                drawRec = 1;
            }
            else if (dKey != 1 && rKey != 1 && lKey != 1 && isKeyDown(Key::D)) {
                dKey = 1;
                delFile = selectedFile;
            }
            else if (isKeyDown(Key::Escape)) {
                sMM = 1;
                currentMenu = 1;
            }
        }

        if (currentMenu == 22) {

            if (dKey != 1 && rKey != 1 && lKey == 1 && isKeyDown(Key::Space)) {
                loadGame(nameOfFile[selectedFile]);
                lKey = -1;
            }
            else if (dKey != 1 && rKey != 1 && lKey != 1 && isKeyDown(Key::R)) {
                rKey = 1;
                oldFile = selectedFile;
                drawRec = 1;
            }
            else if (dKey != 1 && rKey != 1 && lKey != 1 && isKeyDown(Key::D)) {
                dKey = 1;
                delFile = selectedFile;
            }
            else if (isKeyDown(Key::X)) {
                lKey = 0;
            }
        }

        if (itemChanged)
            keyPressTimer.restart();

        if (selectedFile < 0)
            selectedFile = timeFl.size() - 1;
        if (selectedFile >= timeFl.size())
            selectedFile = 0;

        if (selectedFile >= endShowFile) {
            ++startShowFile;
            ++endShowFile;
        }
        if (selectedFile < startShowFile) {
            --startShowFile;
            --endShowFile;
        }

        std::vector<sf::RectangleShape> drawBoard = createThickGrid(550, 550, 55, sf::Color::White, 2, WINDOW_W / 2 + 100, currentMenu == 3 ? WINDOW_H / 2 - 225 : WINDOW_H / 2 - 275);

        for (const auto& line : drawBoard) window.draw(line);
    }
 else {
     sf::Text text(font, "No file has been saved", 30);
     sf::FloatRect bounds = text.getLocalBounds();

     text.setOrigin(bounds.getCenter());
     text.setFillColor(sf::Color::White);
     text.setPosition(sf::Vector2f({WINDOW_W / 2, WINDOW_H / 2}));

     window.draw(text);
    }
}

void showPlayerInfo()
{
    // setColor(240);
    //int infoX = LEFT + BOARD_SIZE * 4 + 5;
    //int infoY = TOP + 2;
    // GotoXY(infoX, infoY);
    // cout << "====================";
    // GotoXY(infoX, infoY + 1);
    // if (langChoice == 1) cout << (turn == 1 ? ">> " : "   ") << player1.name << " (X)  Moves: " << player1.moves;
    // else cout << (turn == 1 ? ">> " : "   ") << player1.name << " (X)  Di chuyển: " << player1.moves;
    // GotoXY(infoX, infoY + 2);
    // if (langChoice == 1) cout << (turn == -1 ? ">> " : "   ") << player2.name << " (O)  Moves: " << player2.moves;
    // else cout << (turn == -1 ? ">> " : "   ") << player2.name << " (O)  Di chuyển: " << player2.moves;
    // setColor(240);
    // GotoXY(infoX, infoY + 3);
    // cout << "====================";
    // GotoXY(infoX, infoY + 5);
    // if (langChoice == 1) cout << "L: Save Game";
    // else cout << "L: Lưu Game";
    // GotoXY(infoX, infoY + 6);
    // if (langChoice == 1) cout << "T: Load Game";
    // else cout << "T: Tải Game";
    // GotoXY(infoX, infoY + 7);
    // if (langChoice == 1) cout << "R: Rename Saved File";
    // else cout << "R: Đổi tên file";
    // GotoXY(infoX, infoY + 8);
    // if (langChoice == 1) cout << "X: Delete Saved File";
    // else cout << "X: Xóa file";
    // GotoXY(infoX, infoY + 9);
    // displayTimeOfFile(infoX, infoY + 9);
    // GotoXY(infoX, infoY + (timeFl.size() > 0 ? timeFl.size() + 11 : 11));
    // if (langChoice == 1) cout << "Esc: Exit to Menu";
    // else cout << "Esc: Thoát Menu";
    // setColor(240);

    vector<wstring> infoPlayer;
    vector<wstring> button;
    std::wostringstream woss1, woss2;

    if (langChoice == 1)
    {
        woss1 << "(X)  Moves: " << player1.moves;
        woss2 << "(O)  Moves: " << player2.moves;

        infoPlayer = {
            woss1.str(),
            woss2.str(),
        };

        button = {
            L"T: Save Game",
            L"L: Load Game",
            L"Esc: Back"
        };
    }
    else
    {
        woss1 << L"(X)  Di chuyển: " << player1.moves;
        woss2 << L"(O)  Di chuyển: " << player2.moves;

        infoPlayer = {
            woss1.str(),
            woss2.str()
        };

        button = {
            L"T: Lưu Game",
            L"L: Tải Game",
            L"Esc: Quay lại"
        };
    }

    sf::FloatRect bounds = nameFile.getLocalBounds();

    nameFile.setPosition(sf::Vector2f({ (float)WINDOW_W - bounds.getCenter().x*2 , (float)WINDOW_H - nameFile.getCharacterSize() - 25}));
    nameFile.setFillColor(sf::Color::Black);


    for (size_t i = 0; i < infoPlayer.size(); ++i) {
        sf::Text infoText(font, infoPlayer[i], 30);

        infoText.setFillColor(i == 0 ? sf::Color::Red : sf::Color(44, 101, 29));

        sf::FloatRect bounds = infoText.getLocalBounds();
        infoText.setOrigin(bounds.getCenter());

        infoText.setPosition(sf::Vector2f({ (float)WINDOW_W / 2 - cs / 2 - 200, (float)WINDOW_H / 2 - cs / 2 + 100 * (i+1)}));
        window.draw(infoText);
    }

    for (size_t i = 0; i < button.size(); ++i) {
        sf::Text buttonText(font, button[i], 30);

        buttonText.setFillColor(sf::Color::Black);

        sf::FloatRect bounds = buttonText.getLocalBounds();
        buttonText.setOrigin(bounds.getCenter());

        buttonText.setPosition(sf::Vector2f({ (float)((WINDOW_W / 2 - cs / 2) + 900), (float)WINDOW_H / 2 - cs / 2 + 100 * (i + 1)}));
        window.draw(buttonText);
    }
}

void displayMessage(const std::string &message, int x, int y)
{
    setColor(240);
    GotoXY(x, y);
    cout << message;
}

void clearMessage(int x, int y, int length)
{
    setColor(240);
    GotoXY(x, y);
    for (int i = 0; i < length; ++i)
        std::cout << " ";
}

void showWinEffect(int result, int player)
{
    // int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    // int msg_y = TOP + 15;
    // string message;
    // int color = 240 + 14;
    //int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    //int msg_y = TOP + 162;

    // if (result == 0) {
    //     message = (langChoice == 1 ? "        DRAW!       " : "        Hòa!       ");
    //     color = 14;
    // }
    // else if (player == 1) {
    //     message = (langChoice == 1 ? "   PLAYER X WINS!   " : "   NGƯOI CHƠI X THẮNG!   ");
    //     color = 240 + 12;
    // }
    // else {
    //     message = (langChoice == 1 ? "   PLAYER O WINS!   " : "   NGƯỜI CHƠI 0 THẮNG!   ");
    //     color = 240 + 1;
    // }
    res = result;
    if (result == 0)
    {
        winText.setString(langChoice == 1 ? "DRAW!" : "Hòa!");
        winText.setFillColor(sf::Color::Blue);
        
        // color = 240 + 12;
    }
    else if (player == 1)
    {
        winText.setString(langChoice == 1 ? L"PLAYER X WINS!" : L"NGƯỜI CHƠI X THẮNG!");
        winText.setFillColor(sf::Color(210, 4, 45));
        // color = 240 + 1;
    }
    else
    {
        winText.setString(langChoice == 1 ? L"PLAYER O WINS!" : L"NGƯOI CHƠI O THẮNG!");
        winText.setFillColor(sf::Color(0, 128, 0));
        // color = 14;
    }

    sf::FloatRect bounds = winText.getLocalBounds();

    winText.setCharacterSize(100);
    winText.setOrigin(bounds.getCenter());
    winText.setPosition(sf::Vector2f({(float)WINDOW_W / 2 - (float)winText.getCharacterSize() / 2 - 200, (float)WINDOW_H / 2 - (float)winText.getCharacterSize() / 2 - 50}));

    // for (int i = 0; i < 3; ++i) {
    // setColor(color);
    // displayMessage(message, msg_x, msg_y);
    // Sleep(200);
    // clearMessage(msg_x, msg_y, (int)message.length());
    // Sleep(200);
    //}
    // setColor(color);
    // displayMessage(message, msg_x, msg_y);
    // setColor(240);
}

void handleMainMenu(const sf::Event& event) {
    if (currentMenu == 1 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
        sMM = selectedItem + 1;

        if (selectedItem == 5)
            sMM = 0;
    }

    if (const auto* keyPressed = event.getIf<sf::Event::KeyReleased>()) {
        if (currentMenu == 1 && selectedItem == 4 && keyPressed->code == sf::Keyboard::Key::Space) {
            langChoice = (langChoice == 1) ? 2 : 1;
        }
    }
}

void showMainMenu()
{
    // system("cls");
    // setColor(240);

    vector<wstring> menuItems;

    if (langChoice == 1)
    {
        menuItems = {L"New Game", L"Load Game", L"About", L"Settings", L"Language: English", L"Exit"};
    }
    else
    {
        menuItems = {L"Trò Chơi Mới", L"Tải Trò Chơi", L"Giới Thiệu", L"Cài Đặt", L"Ngôn ngữ: Tiếng Việt", L"Thoát "};
    }

    sf::Text menuText(font, "", 50);
    //sf::Text menuTitle(font, "CARO GAME", 80);
    //sf::FloatRect bounds = menuTitle.getLocalBounds();
    int totalItems = menuItems.size();
    const unsigned int textSize = 30;
    const float lineSpacing = 100.0f;
    const float totalHeight = totalItems * lineSpacing;
    const float startY = (WINDOW_H / 2.0f) - (totalHeight / 2.0f);

    //menuTitle.setOrigin(bounds.getCenter());
    //menuTitle.setFillColor(sf::Color::Black);
    //menuTitle.setPosition(sf::Vector2f({WINDOW_W / 2, WINDOW_H / 2 - 300}));

    //window.draw(menuTitle);
    
    // int consoleWidth = 120;
    // int boxWidth = 40;
    // int boxHeight = totalItems * 2 + 3;

    // int menuX = (consoleWidth - boxWidth) / 2;
    // int menuY = 12;
    // vector<string> logoLines = {
    //     "  __  __   ______   _   _   _    _  ",
    //     " |  \\/  | |  ____| | \\ | | | |  | | ",
    //     " | \\  / | | |__    |  \\| | | |  | | ",
    //     " | |\\/| | |  __|   | . ` | | |  | | ",
    //     " | |  | | | |____  | |\\  | | |__| | ",
    //     " |_|  |_| |______| |_| \\_|  \\____/  "
    // };
    // int logoWidth = 0;
    // for (const string& line : logoLines) {
    //     if (line.length() > logoWidth) logoWidth = line.length();
    // }
    // int logoX = (consoleWidth - logoWidth) / 2;
    // int logoY = 4;
    // for (const string& line : logoLines) {
    //
    // }

    // while (true) {
    //     drawMenu(menuX, menuY, boxWidth, boxHeight);

    for (size_t i = 0; i < totalItems; ++i)
    {
        // int itemY = menuY + 2 + i * 2;
        // int textX = menuX + (boxWidth - (int)menuItems[i].length()) / 2;

        if ((int)i == selectedItem)
        {
            // string label = " >> " + menuItems[i] + " << ";
            // int labelX = menuX + (boxWidth - (int)label.length()) / 2;

            // GotoXY(labelX, itemY);
            // setColor(240 + 12);
            // cout << label;
            menuText.setString(menuItems[i]);
            menuText.setFillColor(sf::Color::Yellow);
        }
        else
        {
            // GotoXY(textX, itemY);
            // setColor(240);
            // cout << menuItems[i];

            menuText.setFillColor(sf::Color::White);
            menuText.setString(menuItems[i]);
        }
        float currentY = startY + (i * lineSpacing);

        sf::FloatRect bounds = menuText.getLocalBounds();
        menuText.setOrigin(bounds.getCenter());

        //menuText.setPosition(sf::Vector2f({WINDOW_W / 2, 70 * (float)(i + 2) + 125}));
        menuText.setPosition(sf::Vector2f({ WINDOW_W / 2, currentY + 25 }));
        window.draw(menuText);
    }

    // int key = _getch();
    // if (key == 224) {
    //     key = _getch();
    //     if (key == 72) selectedItem--;
    //     if (key == 80) selectedItem++;
    // }
    // else if (key == 'w' || key == 'W') selectedItem--;
    // else if (key == 's' || key == 'S') selectedItem++;
    // else if (key == 13) {
    //     if (selectedItem == 4) {
    //         langChoice = (langChoice == 1) ? 2 : 1;
    //         return showMainMenu();
    //     }
    //     if (selectedItem == 5) return 0;
    //     return selectedItem + 1;
    // }

    int itemChanged = 0;

    if (keyPressTimer.getElapsedTime().asSeconds() >= KEY_DELAY_SECONDS)
    {
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            --selectedItem;
            itemChanged = 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            ++selectedItem;
            itemChanged = 1;
        }
    }

    if (itemChanged)
        keyPressTimer.restart();

    if (selectedItem < 0)
        selectedItem = totalItems - 1;
    if (selectedItem >= totalItems)
        selectedItem = 0;
    //}
}

void showModeMenu(int type)
{
    //system("cls");
    //setColor(240);

    //vector<string> menuItems;

    //int totalItems = menuItems.size();
    //int selectedItem = 0;
    //int consoleWidth = 120;
    //int boxWidth = 40;
    //int boxHeight = totalItems * 2 + 3;

    //int menuX = (consoleWidth - boxWidth) / 2;
    //int menuY = 12;
    //vector<string> logoLines = {
    //    "  __  __   ______   _   _   _    _  ",
    //    " |  \\/  | |  ____| | \\ | | | |  | | ",
    //    " | \\  / | | |__    |  \\| | | |  | | ",
    //    " | |\\/| | |  __|   | . ` | | |  | | ",
    //    " | |  | | | |____  | |\\  | | |__| | ",
    //    " |_|  |_| |______| |_| \\_|  \\____/  "};
    //int logoWidth = 0;
    //for (const string &line : logoLines)
    //{
    //    if (line.length() > logoWidth)
    //        logoWidth = line.length();
    //}
    //int logoX = (consoleWidth - logoWidth) / 2;
    //int logoY = 4;
    //for (const string &line : logoLines)
    //{
    //    GotoXY(logoX, logoY++);
    //    setColor(240 + 2);
    //    cout << line;
    //}
    //while (true)
    //{
    //    drawMenu(menuX, menuY, boxWidth, boxHeight);

    //    for (int i = 0; i < totalItems; i++)
    //    {
    //        int itemY = menuY + 2 + i * 2;
    //        int textX = menuX + (boxWidth - (int)menuItems[i].length()) / 2;

    //        if (i == selectedItem)
    //        {
    //            string label = " >> " + menuItems[i] + " << ";
    //            int labelX = menuX + (boxWidth - (int)label.length()) / 2;

    //            GotoXY(labelX, itemY);
    //            setColor(240 + 12);
    //            cout << label;
    //        }
    //        else
    //        {
    //            GotoXY(textX, itemY);
    //            setColor(240);
    //            cout << menuItems[i];
    //        }
    //    }

    //    int key = _getch();
    //    if (key == 224)
    //    {
    //        key = _getch();
    //        if (key == 72)
    //            selectedItem--;
    //        if (key == 80)
    //            selectedItem++;
    //    }
    //    else if (key == 'w' || key == 'W')
    //        selectedItem--;
    //    else if (key == 's' || key == 'S')
    //        selectedItem++;
    //    else if (key == 13)
    //    {
    //        return selectedItem + 1;
    //    }

    //    if (selectedItem < 0)
    //        selectedItem = totalItems - 1;
    //    if (selectedItem >= totalItems)
    //        selectedItem = 0;
    //}
    vector<wstring> menuItems;
    if (langChoice == 1) {
        menuItems = !type ? vector<wstring>{L"2 PLAYERS", L"PLAY WITH BOT", L"BACK"} : vector<wstring>{ L"EASY", L"MEDIUM", L"HARD"};
    } else {
        menuItems = !type ? vector<wstring>{L"2 NGƯỜI CHƠI", L"CHƠI VỚI MÁY", L"QUAY LẠI"} : vector<wstring>{ L"DỄ", L"TRUNG BÌNH", L"KHÓ"};
    }
    //colorBackGround = sf::Color::Cyan;
    
    int totalItems = menuItems.size();
    const unsigned int textSize = 50;
    sf::Text menuText(font, "", textSize);    
    const float lineSpacing = 100.0f;
    const float totalHeight = totalItems * lineSpacing;
    const float startY = (WINDOW_H / 2.0f) - (totalHeight / 2.0f);

    for (size_t i = 0; i < totalItems; ++i)
    {
        // int itemY = menuY + 2 + i * 2;
        // int textX = menuX + (boxWidth - (int)menuItems[i].length()) / 2;

        if ((int)i == selectedDiff) {
            // string label = " >> " + menuItems[i] + " << ";
            // int labelX = menuX + (boxWidth - (int)label.length()) / 2;

            // GotoXY(labelX, itemY);
            // setColor(240 + 12);
            // cout << label;
            menuText.setString(menuItems[i]);
            menuText.setFillColor(sf::Color::Yellow);
        }
        else
        {
            // GotoXY(textX, itemY);
            // setColor(240);
            // cout << menuItems[i];

            menuText.setFillColor(sf::Color::White);
            menuText.setString(menuItems[i]);
        }
        float currentY = startY + (i * lineSpacing);

        sf::FloatRect bounds = menuText.getLocalBounds();
        menuText.setOrigin(bounds.getCenter());
        menuText.setPosition(sf::Vector2f({ WINDOW_W / 2, currentY + 25}));

        window.draw(menuText);
    }

    int itemChanged = 0;

    if (keyPressTimer.getElapsedTime().asSeconds() >= KEY_DELAY_SECONDS)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            --selectedDiff;
            itemChanged = 1;            
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            ++selectedDiff;
            itemChanged = 1;
        }
    }

    if (currentMenu == 21) 
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) diffChoice = selectedItem + 1;
        //if (isKeyDown(Key::Escape)) {
        //    currentMenu = 20;
        //    //sMM = 20;
        //}
        //sMM = selectedItem + 1;
        //return selectedItem + 1;
        
    }

    if (itemChanged) keyPressTimer.restart();

    if (selectedDiff < 0)
        selectedDiff = totalItems - 1;
    if (selectedDiff >= totalItems)
        selectedDiff = 0;
}

void hienMenuChinh()
{
    showMainMenu();
}

void askContinue()
{
    // int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    // int msg_y = TOP + 17;
    // setColor(240+5);
    // langChoice ? displayMessage("Continue? (Y/N)", msg_x, msg_y) : displayMessage("Tiếp tục? (O/F)", msg_x, msg_y);
    // setColor(240);

    continueText.setString(langChoice ? "Continue? (Y/N)" : "Tiếp tục? (O/F)");
    continueText.setFillColor(sf::Color::Black);
    continueText.setPosition(sf::Vector2f({(float)25, (float)WINDOW_H - continueText.getCharacterSize() - 25}));

    // char c;
    // while (true) {
    //     c = toupper(_getch());
    // if (c == 'Y' || c == 'N') {
    //     clearMessage(msg_x, msg_y, 25);
    //     return (c == 'Y');
    // }
    // if (c == 'O' || c == 'F') {
    //     clearMessage(msg_x, msg_y, 25);
    //     return (c == 'O');
    // }
    //}

    if (isKeyDown(Key::Y) || isKeyDown(Key::O)) {
        //colorBackGround = sf::Color::Blue;
        askCon = 1;
    }

    if (isKeyDown(Key::N) || isKeyDown(Key::F)) {
        //colorBackGround = sf::Color::Blue;
        askCon = 0;
        sMM = 1;
        currentMenu = 1;
        continueText.setString("");
        resetData();
    }
}

string getFileNameFromUser(const string &prompt, int x, int y)
{
    string filename;
    displayMessage(prompt, x, y);
    cin >> filename;
    clearMessage(x, y, (int)prompt.length() + 30);
    return filename;
}

void showAbout()
{
    // system("cls");
    // setColor(240);
    vector<wstring> outAbout;
    wstring outAboutTitile = (langChoice == 1 ? L"ABOUT US" : L"THÔNG TIN TRÒ CHƠI");
    if (langChoice == 1)
    {
        // cout << "\n\n\t\t ================= ABOUT GAME =================";
        // cout << "\n\t\t\t  GAME: CARO CONSOLE EDITION";
        // cout << "\n\t\t\t  Version: 1.0";
        // cout << "\n\t\t\t  Author: Group 11";
        // cout << "\n\t\t\t  Language: C++ (Console)";
        // cout << "\n\t\t\t  TIP: Game has 12x12 checkers, bet 5 consecutive cards to win.";
        // cout << "\n\t\t\t  CONTROL KEYS:";
        // cout << "\n\t\t\t    - W/A/S/D or arrow: Move";
        // cout << "\n\t\t\t    - Enter: Select / Confirm";
        // cout << "\n\t\t\t    - L: Save game";
        // cout << "\n\t\t\t    - T: Load game";
        // cout << "\n\t\t\t    - ESC: Exit to Menu";
        // cout << "\n\n\t\t ==============================================";
        // cout << "\n\n\t\t  Press any key to go back...";

        outAbout = {
            L"GAME: CARO CONSOLE EDITION",
            L"Version: 1.0",
            L"Author: Group 11",
            L"Language: C++ (SFML)",
            L"TIP: Game has 10x10 checkers, bet 5 consecutive cards to win.",
            L"CONTROL KEYS:",
            L"- W/A/S/D or arrow: Move",
            L"- Space: Select",
            L"- Enter: Confirm",
            L"- T: Save game",
            L"- L: Load game",
            L"- X: Close messange",
            L"Press ESC to go back..."};
    }
    else
    {
        // cout << "\n\n\t\t ================= THÔNG TIN TRÒ CHƠI =================";
        // cout << "\n\t\t\t  TRÒ CHƠI: CARO - PHIÊN BẢN CONSOLE";
        // cout << "\n\t\t\t  Phiên bản: 1.0";
        // cout << "\n\t\t\t  Tác giả: Nhóm 11";
        // cout << "\n\t\t\t  Ngôn ngữ lập trình: C++ (Console)";
        // cout << "\n\t\t\t  MẸO: Bàn cờ 12x12, người chơi đặt 5 quân liên tiếp để thắng.";
        // cout << "\n\t\t\t  PHÍM ĐIỀU KHIỂN:";
        // cout << "\n\t\t\t    - W/A/S/D hoặc phím di chuyển: Di chuyển";
        // cout << "\n\t\t\t    - Enter: Đánh cờ / Xác nhận";
        // cout << "\n\t\t\t    - L: Lưu trò chơi";
        // cout << "\n\t\t\t    - T: Tải trò chơi";
        // cout << "\n\t\t\t    - ESC: Thoát về Menu chính";
        // cout << "\n\n\t\t =====================================================";
        // cout << "\n\n\t\t  Nhấn phím bất kì để thoát...";

        outAbout = {
            L"TRÒ CHƠI : CARO - PHIÊN BẢN CONSOLE",
            L"Phiên bản: 1.0",
            L"Tác giả : Nhóm 11",
            L"Ngôn ngữ lập trình : C++ (Console)",
            L"MẸO : Bàn cờ 10x10, người chơi đặt 5 quân liên tiếp để thắng.",
            L"PHÍM ĐIỀU KHIỂN : ",
            L"- W / A / S / D hoặc phím di chuyển : Di chuyển",
            L"- Phím cách: Xác nhận",
            L"- Enter: Đánh cờ",
            L"- T: Lưu game",
            L"- L: Tải game",
            L"- X: Tắt thông báo",
            L"Nhấn phím ESC để thoát..."};
    }

    sf::Text title(font, outAboutTitile, 80);

    sf::FloatRect bounds = title.getLocalBounds();

    title.setOrigin(bounds.getCenter());
    title.setPosition(sf::Vector2f({WINDOW_W / 2, OPAREC_Y - title.getCharacterSize() / 2}));
    title.setFillColor(sf::Color(35, 71, 139));

    window.draw(title);

    for (size_t i = 0; i < outAbout.size(); ++i)
    {
        sf::Text text(font, outAbout[i], 27);
        text.setFillColor(sf::Color::Black);
        text.setPosition(sf::Vector2f({WINDOW_W - OPAREC_X - OPAREC_W / 2 + 25, OPAREC_Y - 75 + 45 * (float)(i + 2)}));

        window.draw(text);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
    {
        currentMenu = 1;
        sMM = 100;
    }
}

void showPlayerMenu()
{
    vector<wstring> playerChoice;

    if (langChoice == 1)
    {
        playerChoice = {
            L"1 player",
            L"2 player"
        };
    }
    else
    {
        playerChoice = {
            L"1 người chơi",
            L"2 người chơi"
        };
    }

    const unsigned int textSize = 50;
    const float lineSpacing = 100.0f;
    const float totalHeight = playerChoice.size() * lineSpacing;
    const float startY = (WINDOW_H / 2.0f) - (totalHeight / 2.0f);

    sf::Text playerChoiceText(font, "", textSize);    

    for (size_t i = 0; i < playerChoice.size(); ++i)
    {
        if ((int)i == selectedMode)
        {
            playerChoiceText.setString(playerChoice[i]);
            playerChoiceText.setFillColor(sf::Color::Yellow);
        }
        else
        {
            playerChoiceText.setFillColor(sf::Color::White);
            playerChoiceText.setString(playerChoice[i]);
        }

        float currentY = startY + (i * lineSpacing);
        sf::FloatRect bounds = playerChoiceText.getLocalBounds();
        playerChoiceText.setOrigin(bounds.getCenter());
        playerChoiceText.setPosition(sf::Vector2f({ WINDOW_W / 2, currentY + 25}));

        window.draw(playerChoiceText);
    }

    int itemChanged = 0;

    if (keyPressTimer.getElapsedTime().asSeconds() >= KEY_DELAY_SECONDS)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            --selectedMode;
            itemChanged = 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            ++selectedMode;
            itemChanged = 1;
        }
    }

    if (currentMenu == 20 && isKeyDown(Key::Space))
    {
        if (selectedMode == 0)
        {
            sMM = 21;
        }
        else if (selectedMode == 1)
        {
            sMM = 22;
        }
    }

    if (currentMenu == 20 && isKeyDown(Key::Escape)) {
        sMM = 1;
    }

    if (itemChanged)
        keyPressTimer.restart();

    if (selectedMode < 0)
        selectedMode = playerChoice.size() - 1;
    if (selectedMode >= playerChoice.size())
        selectedMode = 0;
}

void setConsoleFont()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    fontInfo.FontFamily = FF_DONTCARE;
    fontInfo.FontWeight = FW_NORMAL;
    wcscpy_s(fontInfo.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}

void waitForMouseClick()
{
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, (mode & ~ENABLE_QUICK_EDIT_MODE) | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    FlushConsoleInputBuffer(hStdin);
    INPUT_RECORD irInBuf[128];
    DWORD cNumRead;
    while (true)
    {
        if (!ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead))
            continue;
        for (DWORD i = 0; i < cNumRead; i++)
        {
            if (irInBuf[i].EventType == MOUSE_EVENT)
            {
                if (irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
                {
                    SetConsoleMode(hStdin, mode);
                    return;
                }
            }
            if (irInBuf[i].EventType == KEY_EVENT)
            {
                if (irInBuf[i].Event.KeyEvent.bKeyDown)
                {
                    SetConsoleMode(hStdin, mode);
                    return;
                }
            }
        }
    }
}

void printCentered(string text, int y, int color)
{
    int consoleWidth = 150;
    int textLength = text.length();
    int x = (consoleWidth - textLength) / 2;
    if (x < 0)
        x = 0;

    GotoXY(x, y);
    setColor(color);
    cout << text;
}

void showSplashScreen()
{
    system("cls");
    setColor(240);
    string pixelLogo = R"(
 ██████╗ █████╗ ██████╗  ██████╗      ██████╗  █████╗ ███╗   ███╗███████╗
██╔════╝██╔══██╗██╔══██╗██╔═══██╗    ██╔════╝ ██╔══██╗████╗ ████║██╔════╝
██║     ███████║██████╔╝██║   ██║    ██║  ███╗███████║██╔████╔██║█████╗  
██║     ██╔══██║██╔══██╗██║   ██║    ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝  
╚██████╗██║  ██║██║  ██║╚██████╔╝    ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗
 ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝      ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝
  )";
    stringstream ss(pixelLogo);
    string line;
    int logoX = 5;
    int y = 5;
    while (getline(ss, line))
    {
        if (line.length() > 0 && line.find_first_not_of(" \t\r\n") != string::npos)
        {
            GotoXY(logoX, y++);
            setColor(240 + 9);
            cout << line;
        }
    }
    int promptY = 15;
    string msg1 = "Press any key to enter the game";
    GotoXY(28, promptY);
    setColor(240 + 12);
    cout << msg1;
    string myIntroArt = R"(
   ██░▀██████████████▀░██
　 █▌▒▒░████████████░▒▒▐█
　 █░▒▒▒░██████████░▒▒▒░█
　　▌░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░▐
　　░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░
　 ███▀▀▀██▄▒▒▒▒▒▒▒▄██▀▀▀██
　 ██░░░▐█░▀█▒▒▒▒▒█▀░█▌░░░█
　 ▐▌░░░▐▄▌░▐▌▒▒▒▐▌░▐▄▌░░▐▌
　　█░░░▐█▌░░▌▒▒▒▐░░▐█▌░░█
　　▒▀▄▄▄█▄▄▄▌░▄░▐▄▄▄█▄▄▀▒
　　░░░░░░░░░░└┴┘░░░░░░░░░
　　██▄▄░░░░░░░░░░░░░░▄▄██
　　████████▒▒▒▒▒▒████████
　　█▀░░███▒▒░░▒░░▒▀██████
　　█▒░███▒▒╖░░╥░░╓▒▐█████
　　█▒░▀▀▀░░║░░║░░║░░█████
　　██▄▄▄▄▀▀┴┴╚╧╧╝╧╧╝┴┴███
　　██████████████████████
    )";

    stringstream ssArt(myIntroArt);
    string lineArt;
    int x = 80;
    int artY = 5;

    setColor(240 + 5);

    while (getline(ssArt, lineArt))
    {
        if (lineArt.length() == 0 && artY == 5)
            continue;
        GotoXY(x, artY++);
        cout << lineArt;
    }

    waitForMouseClick();
    system("cls");
    setColor(240);
}
void drawMenu(int x, int y, int width, int height)
{
    setColor(240 + 8);
    for (int i = 0; i < height; i++)
    {
        GotoXY(x, y + i);
        for (int j = 0; j < width; j++)
        {
            if (i == 0 && j == 0)
                cout << "\xE2\x95\x94";
            else if (i == 0 && j == width - 1)
                cout << "\xE2\x95\x97";
            else if (i == height - 1 && j == 0)
                cout << "\xE2\x95\x9A";
            else if (i == height - 1 && j == width - 1)
                cout << "\xE2\x95\x9D";
            else if (i == 0 || i == height - 1)
                cout << "\xE2\x95\x90";
            else if (j == 0 || j == width - 1)
                cout << "\xE2\x95\x91";
            else
                cout << " ";
        }
    }
    setColor(240);
}
static int currentArtIndex = 0;
void randomizeSideImage()
{
    srand(time(0));
    currentArtIndex = rand() % 5;
}
void drawRightSideImage()
{
    int x = 77;
    int y = 2;
    vector<string> artCollection;
    string art1 = R"(
             ¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶
      ¶¶¶¶¶¶¶¶¶           ¶¶¶¶¶¶¶¶¶
    ¶¶¶¶¶          ¶¶¶¶¶          ¶¶¶¶¶
    ¶¶           ¶¶¶¶¶¶¶¶¶            ¶¶
    ¶¶          ¶¶¶¶¶¶¶¶¶¶¶           ¶¶
    ¶¶              ¶¶¶¶¶¶            ¶¶
    ¶¶             ¶¶¶¶¶¶¶            ¶¶
    ¶¶      ¶¶   ¶¶¶¶¶¶¶¶¶            ¶¶
    ¶¶     ¶¶¶¶¶¶¶¶¶¶¶¶¶¶     ¶ ¶     ¶¶
    ¶¶     ¶¶¶¶¶¶¶¶¶¶¶        ¶¶      ¶¶
    ¶¶    ¶¶¶¶¶¶¶¶¶¶¶¶¶       ¶¶¶     ¶¶
    ¶¶    ¶¶¶  ¶¶¶¶¶¶¶¶¶¶¶     ¶¶     ¶¶
    ¶¶      ¶¶    ¶¶¶¶¶¶¶¶¶    ¶¶     ¶¶
    ¶¶       ¶     ¶¶¶¶¶¶¶¶¶ ¶¶¶      ¶¶
    ¶¶             ¶¶¶¶¶¶¶¶¶¶¶        ¶¶
    ¶¶             ¶¶¶¶¶¶¶ ¶         ¶¶
     ¶¶       ¶¶   ¶¶¶¶¶¶¶          ¶¶
      ¶¶      ¶¶¶¶¶¶¶¶¶¶¶          ¶¶
       ¶¶             ¶¶¶         ¶¶
        ¶¶           ¶¶          ¶¶
         ¶¶         ¶¶          ¶¶
          ¶¶      ¶¶¶          ¶¶
           ¶¶¶               ¶¶
             ¶¶            ¶¶¶
               ¶¶¶       ¶¶¶
                 ¶¶¶  ¶¶¶
                    ¶¶
    )";
    artCollection.push_back(art1);
    string art2 = R"(
               $$$$$$$$$$
          $$$$$$$$$$$$$$$$$$$$
       $$$$$$$$$$$$$$$$$$$$$$$$$$
     $$$$$$$$$     $$       $$$$$$$
    $$$$$$$        $$          $$$$$
   $$$$$$          $$$           $$$$
  $$$$$           $$$$            $$$$$
 $$$$$            $$$$             $$$$
$$$$$             $$$$$             $$$$
$$$$             $$$$$$              $$$
$$$$             $$$$$$              $$$$
$$$$            $$$$$$$$$            $$$$
$$$$          $$$$$$$$$$$$$          $$$$
$$$$        $$$$$$$$$$$$$$$$$        $$$$
$$$$      $$$$$$$$$  $$$$$$$$$$      $$$$
$$$$$  $$$$$$$$          $$$$$$$$   $$$$
 $$$$$$$$$$                  $$$$$$$$$$
  $$$$$                           $$$$$
  $$$$$$                         $$$$$
    $$$$$$                     $$$$$$
     $$$$$$$                 $$$$$$
       $$$$$$$$$$       $$$$$$$$$
         $$$$$$$$$$$$$$$$$$$$$$
             $$$$$$$$$$$$$$
    )";
    artCollection.push_back(art2);
    string art3 = R"(
                   $ 
                  $$$ 
                $$$$$$$$ 
              $$$$$$$$$$$$$ 
           $$$$$$$$$$$$$$$$$$ 
        $$$$$$$$$$$$$$$$$$$$$$$ 
     $$$$$$$$$   $$$$$$$$$$$$$$$$$ 
 $$$$$$$$$$$$$$$   $$$$$$$$$$$$$$$$$$ 
$$$$$$$$$$$$$$$$$$ _____$$$$$$$$$$$$$$$$ 
   $$$$$$$$$$$$$$$$$$       $$$$$$$$ 
      $$$$$$$$$$$$$$$$$ 
         $$$$$$$$$$$$$$$$$ 
           $$$$$$$$$$$$$$$$$$ 
              $$$$$$$$$$$$$$$$$ 
    $$$$$$       $$$$$$$$$$$$$$$$$ 
$$$$$$$$$$$$$$     $$$$$$$$$$$$$$$$$ 
 $$$$$$$$$$$$$$$$$   $$$$$$$$$$$$$$$$ 
    $$$$$$$$$$$$$$$$$   $$$$$$$$$$$ 
       $$$$$$$$$$$$$$$$_$$$$$$$$ 
         $$$$$$$$$$$$$$$$$$$$ 
           $$$$$$$$$$$$$$$ 
              $$$$$$$$$$ 
                $$$$$$ 
    )";
    artCollection.push_back(art3);

    if (currentArtIndex >= artCollection.size())
        currentArtIndex = 0;

    stringstream ss(artCollection[currentArtIndex]);
    string line;
    int currentY = y;

    setColor(240 + 5);

    while (getline(ss, line))
    {
        GotoXY(x, currentY++);
        cout << line;
    }

    setColor(240);
}

void drawSettingsArt()
{
    int x = 75;
    int y = 6;
    string art = R"(
              ███     ███
            ███████ ███████
            ██░░░█████░░░██
           ██░██░░███░██░░██
           ██░██░░███░██░░██
           ██░░░░░███░░░░░██
           ███░░░█████░░░███
          ███████████████████
         ███████░░██░░████████
        ███████████████████████
       ██████▒▒▒▒▒▒▒▒▒▒▒▒▒██████
       ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒░▒▒▒░▒▒██
       █▒░▒▒▒░▒▒▒▒▒░▒▒▒▒▒▒▒▒▒▒▒█
      ██▒▒░░░▒▒░▒▒▒▒▒▒▒▒▒▒░░░▒▒██
      ██▒░░█░░▒▒▒▒▒▒▒░▒▒▒░░█░░▒██
     ███▒░░░██░░░░░░░░░░░██░░░▒███
    █████▒▒░░░███████████░░░▒▒█████
    █████ ▒▒▒░░░░░░░░░░░░░▒▒▒ █████
     █████ ▒▒▒▒░░░░░░░░░▒▒▒▒ █████
      ██████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒███████
     █████████           ██████████
    ███ ███                 ███ ████
        ███                     ███
    )";

    stringstream ss(art);
    string line;
    int currentY = y;

    setColor(240 + 3);

    while (getline(ss, line))
    {
        GotoXY(x, currentY++);
        cout << line;
    }

    setColor(240);
}

void showButtonLoad(int m) {
    std::vector<std::wstring> loadButtonText;

    if (langChoice == 1) {
        loadButtonText = {
            m == 3 ? L"L: Load File" : L"Space: Load File",
            L"R: Rename File",
            L"D: Delete File",
            m == 3 ? L"ESC: Back" : L"X: Back"
        };
    }
    else {
        loadButtonText = {
            m == 3 ? L"L: Tải File" : L"Phím cách: Tải File",
            L"R: Đổi tên File",
            L"D: Xóa File",
            m == 3 ? L"ESC: Quay lại" : L"X: Quay lại"
        };
    }

    if (timeFl.size() >= 1) {
        for (size_t i = 0; i < loadButtonText.size(); ++i) {
            float posX, posY;

            if (i < 2) {
                posX = WINDOW_W / 2 - OPAREC_W / 2 + 400 * i + 50;
                posY = currentMenu == 3 ? WINDOW_H / 2 + 225 : WINDOW_H / 2 + 175;
            }
            else {
                posX = WINDOW_W / 2 - OPAREC_W / 2 + 350 * (i - 2) + 50;
                posY = currentMenu == 3 ? WINDOW_H / 2 + 300 : WINDOW_H / 2 + 250;
            }

            sf::Text showText(font, loadButtonText[i], 35);
            sf::FloatRect bounds = showText.getLocalBounds();

            showText.setOrigin(bounds.getCenter());
            showText.setPosition(sf::Vector2f({ posX , posY }));
            showText.setFillColor(sf::Color::Yellow);

            window.draw(showText);
        }
    }
}

void showInputText(int slr, const sf::Event& event) {
    float posXB = 0, posYB = 0, posXS = 0, posYS = 0;
    float recWB = 800, recHB = 250, recWS = 700, recHS = 75;

    if (slr == 3) {
        posXB = WINDOW_W / 2;
        posYB = WINDOW_H / 2 - 110;

        posXS = WINDOW_W / 2;
        posYS = WINDOW_H / 2 + 25;

        titleRecInput.setString(langChoice == 1 ? L"Enter the new name" : L"Nhập tên mới cho file");
    }

    if (slr == 1) {
        posXB = WINDOW_W / 2;
        posYB = WINDOW_H / 2 - 160;

        posXS = WINDOW_W / 2;
        posYS = WINDOW_H / 2 - 25;

        titleRecInput.setString(langChoice == 1 ? L"Enter the name of file" : L"Nhập tên cho file");
    }

    recBig.setSize(sf::Vector2f({ recWB, recHB }));
    recBig.setOrigin(sf::Vector2f({ recWB / 2, 0 }));
    recBig.setPosition(sf::Vector2f({ posXB , posYB }));
    recBig.setFillColor(sf::Color(225, 223, 255));

    recSmall.setSize(sf::Vector2f({ recWS, recHS }));
    recSmall.setOrigin(sf::Vector2f({ recWS / 2, 0 }));
    recSmall.setPosition(sf::Vector2f({ posXS , posYS }));
    recSmall.setFillColor(sf::Color::White);

    sf::FloatRect bounds = titleRecInput.getLocalBounds();
    titleRecInput.setOrigin(bounds.getCenter());
    titleRecInput.setPosition(sf::Vector2f({WINDOW_W / 2 - (float)titleRecInput.getCharacterSize() / 2 + 15, slr == 3 ? (float)WINDOW_H / 2 - 50 : (float)WINDOW_H / 2 - 100}));
    titleRecInput.setFillColor(sf::Color::Black);

    int res = -1;

    if (const sf::Event::TextEntered* textEventData = event.getIf<sf::Event::TextEntered>())
    {
        uint32_t unicode = textEventData->unicode;

        if (unicode == 8)
        {
            if (!inputString.empty())
                inputString.pop_back();
        }
        else if (unicode == 13)
        {
            if (slr == 3) {
                res = renameGame(nameOfFile[oldFile], inputString);                
            }

            if (slr == 1) {
                nameFile.setString(inputString);
                res = saveGame(inputString);

                inputText.setString("");
                titleRecInput.setString("");
                inputString = "";
                rKey = 0;
                tKey = 0;
                pressR = 0;
                oldFile = -1;
                drawRec = 0;
            }
        }
        else if (unicode > 32 && unicode != 127)
        {   
            ++pressR;
            if (pressR != 1 && (currentMenu == 3 || currentMenu == 22) && inputString.length() < 11)
            {
                inputString += static_cast<char>(unicode);
                pressR = 2;
            }
        }

        inputText.setFillColor(sf::Color::Blue);
        inputText.setString(inputString);

        sf::FloatRect bounds = inputText.getLocalBounds();

        inputText.setOrigin(bounds.getCenter());
        inputText.setPosition(sf::Vector2f({ WINDOW_W / 2, slr == 3 ? WINDOW_H / 2 + 65 : (float) WINDOW_H / 2 + 15 }));
        inputText.setCharacterSize(40);
    }

    if (res == 1) {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyReleased>()) {
            {
                if (keyPressed->code == sf::Keyboard::Key::R || keyPressed->code == sf::Keyboard::Key::T) {
                    inputText.setString("");
                    titleRecInput.setString("");
                    inputString = "";
                    rKey = 0;
                    tKey = 0;
                    pressR = 0;
                    oldFile = -1;
                    drawRec = 0;
                }
            }
        }

        
    }
}