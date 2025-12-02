#include "View.h"
#include "AudioManager.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <fstream>
#include <vector>
#include<sstream>
#include <iomanip>
#include<cwchar>
using namespace std;

int langChoice;
void drawImage(std::wstring imagePath, int x, int y) {
    HWND consoleWindow = GetConsoleWindow();
    HDC hdc = GetDC(consoleWindow);

    HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, imagePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (hBitmap) {
        HDC hMemDC = CreateCompatibleDC(hdc);
        SelectObject(hMemDC, hBitmap);

        BITMAP bm;
        GetObject(hBitmap, sizeof(bm), &bm);

        BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);

        DeleteDC(hMemDC);
        DeleteObject(hBitmap);
    }
    ReleaseDC(consoleWindow, hdc);
}
int showSettingsMenu() {
    system("cls");
    setColor(240);
    AudioManager& audio = AudioManager::getInstance();
    cout << "\n\n\t\t =============== AUDIO SETTINGS ===============";

    cout << "\n\t\t\t 1. Toggle Mute (Currently: "
        << (audio.isMuted() ? "MUTED" : "ON") << ")";

    cout << "\n\t\t\t 2. Decrease Volume";
    cout << "\n\t\t\t 3. Increase Volume";
    cout << "\n\n\t\t\t    Current Volume: " << audio.getVolumeLevel() << "%";

    cout << "\n\n\t\t\t 0. Back to Main Menu";
    cout << "\n\n\t\t ==============================================";
    cout << "\n\n\t\t\t Enter your choice: ";

    int choice;
    cin >> choice;
    return choice;
}
int showMenuSettings() {
    system("cls");
    setColor(240);
    AudioManager& audio = AudioManager::getInstance();

    cout << "\n\n\t\t =============== CÀI ĐẶT ÂM THANH ===============";

    cout << "\n\t\t\t 1. Bật/Tắt Âm (Hiện tại: "
        << (audio.isMuted() ? "TẮT" : "BÂT") << ")";

    cout << "\n\t\t\t 2. Giảm Âm Lượng";
    cout << "\n\t\t\t 3. Tăng Âm Lượng";
    cout << "\n\n\t\t\t    Âm Lượng Hiện Tại: " << audio.getVolumeLevel() << "%";

    cout << "\n\n\t\t\t 0. Quay lại Menu Chinh";
    cout << "\n\n\t\t ================================================";
    cout << "\n\n\t\t\t Nhập lựa chọn cơ bản: ";

    int choice;
    cin >> choice;
    return choice;
}

void fixConsoleWindow() {
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
    SetWindowLong(consoleWindow, GWL_STYLE, style);
    MoveWindow(consoleWindow, 100, 100, 1100, 700, TRUE);
}

void GotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void updateCellAtScreen(int screenX, int screenY, int player) {
    GotoXY(screenX, screenY);

    if (player == 1) {
        setColor(240+12);
        cout << "X";
    }
    else if (player == -1) {
        setColor(240+10);
        cout << "O";
    }
    else {
        int col = (screenX - LEFT) / 4;
        int row = (screenY - TOP) / 2;

        if ((row + col) % 2 == 0)
            setColor(240+7);  
        else
            setColor(240+8);     

        cout << " ";
    }

    setColor(240);
}


void updateCellAtBoardIndex(int col, int row, int player) {
    int screenX = LEFT + col * 4 + 2;
    int screenY = TOP + row * 2 + 1;
    updateCellAtScreen(screenX, screenY, player);
}

void drawBoard() {
    system("cls");
    setColor(249);
    for (int i = 0; i <= BOARD_SIZE; ++i) {
        for (int j = 0; j <= BOARD_SIZE; ++j) {
            GotoXY(LEFT + 4 * j, TOP + 2 * i);
            if (i == 0 && j == 0) cout << "╔";
            else if (i == 0 && j == BOARD_SIZE) cout << "╗";
            else if (i == BOARD_SIZE && j == 0) cout << "╚";
            else if (i == BOARD_SIZE && j == BOARD_SIZE) cout << "╝";
            else if (i == 0) std::cout << "╦";
            else if (i == BOARD_SIZE) cout << "╩";
            else if (j == 0) std::cout << "╠";
            else if (j == BOARD_SIZE) cout << "╣";
            else cout << "╬";

            if (j < BOARD_SIZE) cout << "═══"; 

            if (i < BOARD_SIZE) {
                GotoXY(LEFT + 4 * j, TOP + 2 * i + 1);
                cout << "║";
            }
        }
    }
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            updateCellAtBoardIndex(c, r, 0); 
        }
    }
    GotoXY(0, 0);
    setColor(240); 
}
void displayTimeOfFile(int infoX, int infoY) {
    setColor(240);
    std::string fileName, timeSaved, dateSaved;

    std::ifstream f("timeFile.txt");

    int numOfFile = 0;

    f >> numOfFile;
    timeFl.resize(numOfFile);

    for (int i = 0; i < timeFl.size(); ++i) {
        f >> fileName >> dateSaved >> timeSaved;
        timeFl[i].first = fileName;
        timeFl[i].second.first = dateSaved;
        timeFl[i].second.second = timeSaved;

        GotoXY(infoX, ++infoY);
        cout << left << setw(15) << fileName << setw(10) << timeSaved << setw(1) << dateSaved;
    }


    f.close();
}

void showPlayerInfo() {
    setColor(240);
    int infoX = LEFT + BOARD_SIZE * 4 + 5;
    int infoY = TOP + 2;
    GotoXY(infoX, infoY);
    cout << "====================";
    GotoXY(infoX, infoY + 1);
    if (langChoice == 1) cout << (turn == 1 ? ">> " : "   ") << player1.name << " (X)  Moves: " << player1.moves;
    else cout << (turn == 1 ? ">> " : "   ") << player1.name << " (X)  Di chuyển: " << player1.moves;
    GotoXY(infoX, infoY + 2);
    if (langChoice == 1) cout << (turn == -1 ? ">> " : "   ") << player2.name << " (O)  Moves: " << player2.moves;
    else cout << (turn == -1 ? ">> " : "   ") << player2.name << " (O)  Di chuyển: " << player2.moves;
    setColor(240);
    GotoXY(infoX, infoY + 3);
    cout << "====================";
    GotoXY(infoX, infoY + 5);
    if (langChoice == 1) cout << "L: Save Game" << langChoice;
    else cout << "L: Lưu Game";
    GotoXY(infoX, infoY + 6);
    if (langChoice == 1) cout << "T: Load Game";
    else cout << "T: Tải Game";
    GotoXY(infoX, infoY + 7);
    if (langChoice == 1) cout << "R: Rename Saved File";
    else cout << "R: Đổi tên file";
    GotoXY(infoX, infoY + 8);
    if (langChoice == 1) cout << "X: Delete Saved File";
    else cout << "X: Xóa file";
    GotoXY(infoX, infoY + 9);
    displayTimeOfFile(infoX, infoY + 9);
    GotoXY(infoX, infoY + (timeFl.size() > 0 ? timeFl.size() + 11 : 11));
    if (langChoice == 1) cout << "Esc: Exit to Menu";
    else cout << "Esc: Thoát Menu";
    setColor(240);
}

void displayMessage(const std::string& message, int x, int y) {
    setColor(240);
    GotoXY(x, y);
    cout << message;
}

void clearMessage(int x, int y, int length) {
    setColor(240);
    GotoXY(x, y);
    for (int i = 0; i < length; ++i) std::cout << " ";
}

void showWinEffect(int player) {
    int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    int msg_y = TOP + 15;
    string message;
    int color = 240 + 14;

    if (player == 1) {
        message = (langChoice == 1 ? "   PLAYER X WINS!   " : "   NGƯOI CHƠI X THẮNG!   ");
        color = 240 + 12;
    }
    else if (player == -1) {
        message = (langChoice == 1 ? "   PLAYER O WINS!   " : "   NGƯỜI CHƠI 0 THẮNG!   ");
        color = 240 + 1;
    }
    else {
        message = (langChoice == 1 ? "        DRAW!       " : "        Hòa!       ");
        color = 14;
    }

    for (int i = 0; i < 3; ++i) {
        setColor(color);
        displayMessage(message, msg_x, msg_y);
        Sleep(200);
        clearMessage(msg_x, msg_y, (int)message.length());
        Sleep(200);
    }
    setColor(color);
    displayMessage(message, msg_x, msg_y);
    setColor(240);
}

int showMainMenu() {
    system("cls");
    setColor(240);
    drawImage(L"(image//menu.bmp)", 600, 50);
    cout << "\n\n\t\t =============== CARO GAME MENU ===============";
    cout << "\n\t\t\t     1. New Game";
    cout << "\n\t\t\t     2. Load Game";
    cout << "\n\t\t\t     3. About Game";
    cout << "\n\t\t\t     4. Settings";
    cout << "\n\t\t\t     5. Language";
    cout << "\n\t\t\t     0. Exit";
    cout << "\n\n\t\t ==============================================";
    cout << "\n\n\t\t\t Enter your choice: ";

    int choice;
    cin >> choice;

    if (choice == 5) {
        system("cls");
        cout << "\n\n\t\t =============== SELECT LANGUAGE ===============";
        cout << "\n\t\t\t     1. English";
        cout << "\n\t\t\t     2. Vietnamese";
        cout << "\n\n\t\t ==============================================";
        cout << "\n\n\t\t\t Enter your choice: ";

        int* lang = &langChoice;
        cin >> *lang;

        if (langChoice == 1) {
            //lang = 0;
            cout << "\n\t\t You selected English.";
            cout << "\n\t\t Press any key to return menu";
        }
        else if (langChoice == 2) {
            //lang = 1;
            cout << "\n\t\t Bạn đã chọn Tiếng Việt .";
            cout << "\n\t\t Nhấn phím bắt kỳ để quay lại menu...";
        }
        else {
            cout << "\n\t\t Invalid choice. Keeping current language.";
        }

        _getch();
        return (langChoice == 1 ? showMainMenu() : hienMenuChinh());
    }

    return choice;
}

int hienMenuChinh() {
    system("cls");
    cout << "\n\n\t\t =============== MENU TRO CHOI CARO ===============";
    cout << "\n\t\t\t     1. Trò chơi mới";
    cout << "\n\t\t\t     2. Tải trò chơi";
    cout << "\n\t\t\t     3. Giới thiệu trò chơi";
    cout << "\n\t\t\t     4. Cài đặt";
    cout << "\n\t\t\t     5. Ngôn ngữ";
    cout << "\n\t\t\t     0. Thoát";
    cout << "\n\n\t\t ================================================";
    cout << "\n\n\t\t\t Nhập lựa chọn của bạn: ";
    int luaChon;
    cin >> luaChon;

    if (luaChon == 5) {
        system("cls");
        cout << "\n\n\t\t =============== CHỌN NGÔN NGỮ ===============";
        cout << "\n\t\t\t     1. Tiếng Anh (English)";
        cout << "\n\t\t\t     2. Tiếng Việt (Vietnamese)";
        cout << "\n\n\t\t ============================================";
        cout << "\n\n\t\t\t Nhập lựa chọn của bạn: ";

        int* lang = &langChoice;
        cin >> *lang;

        if (langChoice == 1) {
            //lang = 0;
            cout << "\n\t\t You selected English.";
            cout << "\n\t\t Press any key to return menu";
        }
        else if (langChoice == 2) {
            //lang = 1;
            cout << "\n\t\t Bạn đã chọn Tiếng Việt.";
            cout << "\n\t\t Nhấn phím bất kì để quay lại menu...";
        }
        else {
            cout << "\n\t\t Invalid choice. Keeping current language.";
        }
        _getch();
        return (langChoice == 1 ? showMainMenu() : hienMenuChinh());
    }

    return luaChon;
}

bool askContinue() {
    int msg_x = LEFT + BOARD_SIZE * 4 + 5;
    int msg_y = TOP + 17;
    setColor(240+5);
    langChoice ? displayMessage("Continue? (Y/N)", msg_x, msg_y) : displayMessage("Tiếp tục? (O/F)", msg_x, msg_y);
    setColor(240);
    char c;
    while (true) {
        c = toupper(_getch());
        if (c == 'Y' || c == 'N') {
            clearMessage(msg_x, msg_y, 25);
            return (c == 'Y');
        }
        if (c == 'O' || c == 'F') {
            clearMessage(msg_x, msg_y, 25);
            return (c == 'O');
        }
    }
}

string getFileNameFromUser(const string& prompt, int x, int y) {
    string filename;
    displayMessage(prompt, x, y);
    cin >> filename;
    clearMessage(x, y, (int)prompt.length() + 30);
    return filename;
}

void showAbout() {
    system("cls");
    setColor(240);
    if (langChoice == 1) {
        cout << "\n\n\t\t ================= ABOUT GAME =================";
        cout << "\n\t\t\t  GAME: CARO CONSOLE EDITION";
        cout << "\n\t\t\t  Version: 1.0";
        cout << "\n\t\t\t  Author: Group 11";
        cout << "\n\t\t\t  Language: C++ (Console)";
        cout << "\n\t\t\t  TIP: Game has 12x12 checkers, bet 5 consecutive cards to win.";
        cout << "\n\t\t\t  CONTROL KEYS:";
        cout << "\n\t\t\t    - W/A/S/D or arrow: Move";
        cout << "\n\t\t\t    - Enter: Select / Confirm";
        cout << "\n\t\t\t    - L: Save game";
        cout << "\n\t\t\t    - T: Load game";
        cout << "\n\t\t\t    - ESC: Exit to Menu";
        cout << "\n\n\t\t ==============================================";
        cout << "\n\n\t\t  Press any key to go back...";
    }
    else {
        cout << "\n\n\t\t ================= THÔNG TIN TRÒ CHƠI =================";
        cout << "\n\t\t\t  TRÒ CHƠI: CARO - PHIÊN BẢN CONSOLE";
        cout << "\n\t\t\t  Phiên bản: 1.0";
        cout << "\n\t\t\t  Tác giả: Nhóm 11";
        cout << "\n\t\t\t  Ngôn ngữ lập trình: C++ (Console)";
        cout << "\n\t\t\t  MẸO: Bàn cờ 12x12, người chơi đặt 5 quân liên tiếp để thắng.";
        cout << "\n\t\t\t  PHÍM ĐIỀU KHIỂN:";
        cout << "\n\t\t\t    - W/A/S/D hoặc phím di chuyển: Di chuyển";
        cout << "\n\t\t\t    - Enter: Đánh cờ / Xác nhận";
        cout << "\n\t\t\t    - L: Lưu trò chơi";
        cout << "\n\t\t\t    - T: Tải trò chơi";
        cout << "\n\t\t\t    - ESC: Thoát về Menu chính";
        cout << "\n\n\t\t =====================================================";
        cout << "\n\n\t\t  Nhấn phím bất kì để thoát...";
    }
    _getch();
}
void setConsoleFont() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    fontInfo.FontFamily = FF_DONTCARE;
    fontInfo.FontWeight = FW_NORMAL;
    wcscpy_s(fontInfo.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}

void waitForMouseClick() {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
	GetConsoleMode(hStdin, &mode); //lẤY CHẾ ĐỘ CONSOLE HIỆN TẠI
    SetConsoleMode(hStdin, (mode & ~ENABLE_QUICK_EDIT_MODE) | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    FlushConsoleInputBuffer(hStdin);
    INPUT_RECORD irInBuf[128];
	DWORD cNumRead;
    while (true) {
        if (!ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead)) continue;
        for(DWORD i = 0; i < cNumRead; i++) {
            if (irInBuf[i].EventType == MOUSE_EVENT) {
                if(irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                    SetConsoleMode(hStdin, mode); // KHÔI PHỤC LẠI CHẾ ĐỘ CONSOLE BAN ĐẦU
                    return;
				}
            }
            if (irInBuf[i].EventType == KEY_EVENT) {
                if (irInBuf[i].Event.KeyEvent.bKeyDown) {
                    SetConsoleMode(hStdin, mode);
                    return;
                }
            }
		}
    }
}

void printCentered(string text, int y, int color) {
    int consoleWidth = 80;
    int textLength = text.length();
    int x = (consoleWidth - textLength) / 2; 
    if (x < 0) x = 0;
    GotoXY(x, y);
    setColor(color);
    cout << text;
}

void showSplashScreen() {
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
    int y = 5; 
    while (getline(ss, line)) {
        if (line.length() > 0 && line.find_first_not_of(" \t\r\n") != string::npos) {
            printCentered(line, y++, 240 + 9); 
        }
    }
    int promptY = 20;
    GotoXY(20, promptY);
    setColor(240 + 12); 
    cout << "Ấn phím xuống để vào";
    GotoXY(25, promptY + 2);
    setColor(240 + 8);
    cout << "Comming soon";
    waitForMouseClick();
    system("cls");
    setColor(240);
}