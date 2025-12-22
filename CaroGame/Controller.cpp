#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "AI.h"
#include "AudioManager.h"
#include "Interface.h"
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
static void endGame(const sf::Event& event, int result, int player, int playWBot = 0);
static void runGameLoop(const sf::Event &event);
static void updateKeyToggle();

void handleSave(const sf::Event& event);
void handleLoad(bool startFromMenu, const sf::Event& event);
void handleRename(const sf::Event& event);
void handleDelete();

int checkInputString(std::string s) {
    for (int i = 0; i < timeFl.size(); ++i) {
        if (timeFl[i].ff == s) return 0;
    }

    return 1;
}

void updateKeyToggle() {
    float leftPos = toggleTrack.getPosition().x;
    float rightPos = toggleTrack.getPosition().x + trackW - thumbW;
    AudioManager& audio = AudioManager::getInstance();

    if (isMoving) {
        if (isSoundOn == 1) { 
            if (toggleThumb.getPosition().x < rightPos) {
                toggleThumb.move(sf::Vector2f{ thumbSpeed, 0 });
                toggleTrack.setFillColor(sf::Color(11, 218, 81));
            }
            else {
                toggleThumb.setPosition(sf::Vector2f{ rightPos, toggleThumb.getPosition().y });
                isMoving = false; 
            }
        }
        else {
            if (toggleThumb.getPosition().x > leftPos) {
                toggleThumb.move(sf::Vector2f{ (-1) * thumbSpeed, 0 });
                toggleTrack.setFillColor(sf::Color(128, 128, 128));
            }
            else {
                toggleThumb.setPosition(sf::Vector2f{ leftPos, toggleThumb.getPosition().y });
                isMoving = false;
            }
        }
    }
}

bool isKeyDown(sf::Keyboard::Key key)
{
    return sf::Keyboard::isKeyPressed(key);
}

static void handleSettings(const sf::Event& event)
{
    bool exitSettings = false;
    AudioManager &audio = AudioManager::getInstance();
    int choice = handleVol(event);
    
    switch (choice)
    {
    case 1:
        audio.toggleMute();
        updateKeyToggle();
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

void run(const sf::Event& event)
{
    int choice = sMM;

    switch (choice) {
    case 1: {
        res = -1;
        if (const auto* keyPressed = event.getIf<sf::Event::KeyReleased>()) {
            if (keyPressed->code == sf::Keyboard::Key::Enter) currentMenu = 20;
            if (keyPressed->code == sf::Keyboard::Key::Escape) currentMenu = 1;
        }
        break;
    }
    case 2:
    {
        currentMenu = 3;
        break;
    }
    case 21:
    {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyReleased>()) {
            if (currentMenu == 21 && keyPressed->code == sf::Keyboard::Key::Enter) {
                currentMenu = 22;
                sMM = 211;
            }

            if (currentMenu == 20 && keyPressed->code == sf::Keyboard::Key::Enter) {
                currentMenu = 21;
            }
        }


        if (0 < diffChoice && diffChoice < 4) {
            DIFF = diffChoice;
        }
        else break;
        break;
    }
    case 211: {
        AudioManager::getInstance().stopBackgroundMusic();
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
        AudioManager::getInstance().stopBackgroundMusic();
        if (loadFromMenu == 1) {
            if (const auto* keyPressed = event.getIf<sf::Event::KeyReleased>()) {
                if (keyPressed->code == sf::Keyboard::Key::L) {
                    currentMenu = 22;
                }
            }
        }
        else currentMenu = 22;

        ++statusOfGame;
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
        handleSettings(event);
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
}

static void runGameLoop(const sf::Event &event)
{
    bool exitRequested = false;

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
     
}

static bool processInput(const sf::Event &event, int preMenu)
{
    if (currentMenu == 22)
    {
        if (isKeyDown(Key::Up) && lKey == 0 && tKey == 0)
            moveCursor(0);
        if (isKeyDown(Key::Down) && lKey == 0 && tKey == 0)
            moveCursor(1);
        if (isKeyDown(Key::Left) && lKey == 0 && tKey == 0)
            moveCursor(2);
        if (isKeyDown(Key::Right) && lKey == 0 && tKey == 0)
            moveCursor(3);

        if (isKeyDown(Key::Enter) && lKey == 0 && tKey == 0)
            handleTurn();

        if (isKeyDown(Key::L) && tKey == 0) lKey = 1;
        
        if (isKeyDown(Key::T) && lKey == 0 && timeFl.size() < 20)
            tKey = 1;
 
            if (const auto* keyPressed = event.getIf<sf::Event::KeyReleased>()) {
                if (lKey != 1 && tKey != 1 && rKey != 1 && keyPressed->code == sf::Keyboard::Key::Escape) {
                    AudioManager::getInstance().playBackgroundMusic("audio/background_music.wav");
                    statusOfGame = 0;
                    nameFile.setString("");
                    currentMenu = 20;
                    sMM = 20;
                    diffChoice = 0;
                    return true;
                }

                if (keyPressed->code == sf::Keyboard::Key::W && lKey == 0 && tKey == 0)
                    moveCursor(0);
                if (keyPressed->code == sf::Keyboard::Key::S && lKey == 0 && tKey == 0)
                    moveCursor(1);
                if (keyPressed->code == sf::Keyboard::Key::A && lKey == 0 && tKey == 0)
                    moveCursor(2);
                if (keyPressed->code == sf::Keyboard::Key::D && lKey == 0 && tKey == 0)
                    moveCursor(3);
        }
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

    int screenX = startX + (cursorCol + 1) * 18 + 42 * (cursorCol);
    int screenY = startY + (cursorRow + 1) * 6 + 54 * (cursorRow);


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
        okToCheck = 1;
        showPlayerInfo();
        int screenX = LEFT + cursorCol * 4 + 2;
        int screenY = TOP + cursorRow * 2 + 1;
    }
}

void handleSave(const sf::Event &event)
{
    showInputText(1, event);
    drawRec = 1;
}

int handleLoadMiniBoard(int num) {
    char p = '0';
    int flag = 0;
    int tmp;
        std::ifstream f(timeFl[num].ff);
        if (!f.is_open()) return 0;

        f >> tmp;
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
}

void handleRename(const sf::Event& event)
{
    showInputText(3, event);
}

void handleDelete()
{
    deleteGame(timeFl[selectedFile].ff);
    dKey = 0;
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
}

void handleBotPlay(const sf::Event& event)
{
    bool exitRequested = false;
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
            result = getGameState(board, _POINT{cursorRow, cursorCol, -turn});
            cursorRow = tmpX;
            cursorCol = tmpY;
            int screenX = LEFT + cursorCol * 4 + 2;
            int screenY = TOP + cursorRow * 2 + 1;
        }
        if (result != 2)
        {
            endGame(event, result, -turn, 1);
            return;
        }
}

void ansContinue(const sf::Event &event) {
    if (askCon == 1)
    {
        res = -1;
        askCon = 0;
        resetData();
        if (!bot)
            runGameLoop(event);
        else
            handleBotPlay(event);
    }
}
