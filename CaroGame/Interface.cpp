#include "Interface.h"
#include "View.h"
#include "controller.h"

bool isSoundOn = true;
bool isMoving = false;
float trackW = 80.f, trackH = 30;
float thumbW = 30, thumbH = 30;
float thumbSpeed = trackW - thumbW;
int currentMenu = 0;
const sf::Font font("Boheiman.ttf");
const float OPAREC_W = 1200;
const float OPAREC_H = 600;
const float OPAREC_X = WINDOW_W / 2;
const float OPAREC_Y = WINDOW_H - OPAREC_H - 60;
sf::RenderWindow window(sf::VideoMode({ WINDOW_W, WINDOW_H }), "CSLT - Group 11 - Caro game", sf::Style::Titlebar | sf::Style::Close);
sf::Color colorBackGround = sf::Color::Black;
sf::Text titleRecInput(font, "", 50);
sf::RectangleShape recBig;
sf::RectangleShape recSmall;
sf::RectangleShape toggleTrack;
sf::RectangleShape toggleThumb;
sf::Text cursor(font, "X", 35);
sf::Text playerX(font, "X", 15);
sf::Text playerO(font, "O", 15);
sf::Text winText(font, "", 100);
sf::Text continueText(font, "", 35);
sf::Text saveText(font, "", 35);
sf::Text loadText(font, "", 10);
sf::Text inputText(font, "", 30);
sf::Text nameFile(font, "", 60);
sf::Text titleLoad(font, "", 80);

static sf::Texture texMain;
static sf::Texture texMenu;
static sf::Texture texLoad;
static sf::Texture texAbout;
static sf::Texture texBoard;
static sf::Texture texSet;
static sf::RectangleShape grayBar;
static sf::RectangleShape colorBar;
static const float BAR_WIDTH = 1000;
static const float BAR_HEIGHT = 50;
static const float SPEED = 0.3f;
static float currentWidthBar = 0;

void drawWinningLine(float xStart, float yStart, float xEnd, float yEnd) {
    float dx = xEnd - xStart;
    float dy = yEnd - yStart;
    float length = (std::sqrt(dx * dx + dy * dy)) * 75;
    float angle = std::atan2(dy, dx) * 180 / 3.14159f;

    sf::RectangleShape line;

    line.setSize(sf::Vector2f(length, 3.f));

    line.setFillColor(turn == -1 ? sf::Color(210, 4, 45) : sf::Color(0, 128, 0));
    if (res == 0) line.setFillColor(sf::Color::Blue);
    line.setPosition(angle < 90 ? sf::Vector2f({ startX + (xStart) * (cs / BOARD_SIZE), angle != 0 ? startY + yStart * (cs / BOARD_SIZE) : startY + 30 + yStart * (cs / BOARD_SIZE) }) : sf::Vector2f({ startX + 30 + xStart * (cs / BOARD_SIZE), startY + yStart * (cs / BOARD_SIZE) }));
    if (angle >= 135) line.setPosition(sf::Vector2f({ startX + (xStart + 1) * (cs / BOARD_SIZE), startY + yStart * (cs / BOARD_SIZE) }));
    if (dy != 0 ) line.setRotation(angle < 90 || angle >= 135 ? sf::degrees(angle) : sf::degrees(90));

    window.draw(line);
}

void soundBar(int w) {
    float barW = 700, barH = 30;

    grayBar.setSize(sf::Vector2f({ barW, barH }));
    grayBar.setPosition(sf::Vector2f({ (WINDOW_W - barW) / 2 - 100, 475 - barH }));
    grayBar.setFillColor(sf::Color(128, 128, 128));

    colorBar.setFillColor(sf::Color::Cyan);
    colorBar.setPosition(grayBar.getPosition());

    colorBar.setSize(sf::Vector2f({ (float)w * (barW / 100), barH}));
    window.draw(grayBar);
    window.draw(colorBar);
}

void initKeyToggle(sf::Vector2f pos) {
    toggleTrack.setSize({ trackW, trackH });
    toggleTrack.setFillColor(sf::Color(11, 218, 81));
    toggleTrack.setPosition(pos);

    toggleThumb.setSize({ thumbW, thumbH });
    toggleThumb.setFillColor(sf::Color::White);
    toggleThumb.setPosition(sf::Vector2f({ pos.x + trackW - thumbW, pos.y + trackH - thumbH }));
}

void drawOpaRec(float w, float h, float x, float y, sf::Color color, int outline) {
    sf::RectangleShape opaRec;

    opaRec.setSize(sf::Vector2f({ w, h }));
    opaRec.setOrigin(sf::Vector2f({ OPAREC_W / 2, 0 }));
    opaRec.setPosition(sf::Vector2f({ x, y }));
    opaRec.setFillColor(color);
    if (outline != 0) {
        opaRec.setOutlineThickness(5.0f);
        opaRec.setOutlineColor(turn == 1 ? sf::Color(0, 128, 0) : sf::Color(210, 4, 45));
        if (res == 0) opaRec.setOutlineColor(sf::Color::Blue);
    }

    window.draw(opaRec);
}

static int processBar() {
    if (grayBar.getSize().x == 0) {
        grayBar.setSize(sf::Vector2f({ BAR_WIDTH, BAR_HEIGHT }));
        grayBar.setPosition(sf::Vector2f({ (WINDOW_W - BAR_WIDTH) / 2, 450 - BAR_HEIGHT }));
        grayBar.setFillColor(sf::Color(128, 128, 128));

        colorBar.setFillColor(sf::Color::Cyan);
        colorBar.setPosition(grayBar.getPosition());
    }

    if (currentWidthBar < BAR_WIDTH) {
        currentWidthBar += SPEED;
    }
    else {
        ++currentWidthBar;
    }

    colorBar.setSize(currentWidthBar < BAR_WIDTH ? sf::Vector2f({ currentWidthBar, BAR_HEIGHT }) : sf::Vector2f({ BAR_WIDTH, BAR_HEIGHT }));

    if (currentWidthBar >= BAR_WIDTH + 250) return 1;
    return 0;
}

std::vector<sf::RectangleShape> createThickGrid(int w, int h, int cellSize, sf::Color color, float thickness, float offsetX, float offsetY) {
    std::vector<sf::RectangleShape> lines;
    sf::Color gridColor = color;

    for (int i = 0; i <= w; i += cellSize) {
        sf::RectangleShape verticalLine;

        verticalLine.setSize(sf::Vector2f(thickness, (float)h));
        verticalLine.setFillColor(gridColor);
        verticalLine.setPosition(sf::Vector2f({ (float)i - thickness / 2.0f + offsetX, offsetY }));

        lines.push_back(verticalLine);
    }

    for (int j = 0; j <= h; j += cellSize) {
        sf::RectangleShape horizontalLine;

        horizontalLine.setSize(sf::Vector2f((float)w, thickness));
        horizontalLine.setFillColor(gridColor);
        horizontalLine.setPosition(sf::Vector2f({ offsetX, (float)j - thickness / 2.0f + offsetY }));

        lines.push_back(horizontalLine);
    }

    return lines;
}

void declare() {
    cursor.setFillColor(sf::Color::Black);
    cursor.setPosition(sf::Vector2f({ startX + 18, startY + 6 }));

    if (!texMain.loadFromFile("image/background.JPG")) std::cout << "Error at background of main\n";
    if (!texMenu.loadFromFile("image/backgroundMainMenu.png")) std::cout << "Error at background of menu\n";
    if (!texLoad.loadFromFile("image/backgroundLoad.png")) std::cout << "Error at background of load\n";
    if (!texAbout.loadFromFile("image/backgroundAbout.png")) std::cout << "Error at background of about\n";
    if (!texBoard.loadFromFile("image/backgroundBoard.png")) std::cout << "Error at background of board\n";
    if (!texSet.loadFromFile("image/backgroundSet.png")) std::cout << "Error at background of set\n";

    initKeyToggle({ WINDOW_W - OPAREC_X - OPAREC_W / 2 + 425, OPAREC_Y + 85 });
}

void startGame() {
    //currentMenu = 3;
    //lKey = 1;

    const sf::Color colorGrid(128, 128, 128, 100);
    sf::Font fontTitle("Pixelic.ttf");
    sf::Text title(fontTitle, L"CARO GAME", 150);

    sf::FloatRect bounds = title.getLocalBounds();

    title.setFillColor(sf::Color::White);
    title.setOrigin(bounds.getCenter());
    title.setPosition(sf::Vector2f({ WINDOW_W / 2, WINDOW_H / 2 - 125 }));
    title.setStyle(sf::Text::Bold);

    declare();

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* keyL = event->getIf<sf::Event::KeyPressed>()) {
                if (lKey == -1 && keyL->code != sf::Keyboard::Key::L) {
                    lKey = 0;
                }
            }

            run(*event);

            if (currentMenu == 1) handleMainMenu(*event);
            if (currentMenu == 21) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyReleased>()) {
                    if (keyPressed->code == sf::Keyboard::Key::Escape) currentMenu = 20;
                }
            }
            if (currentMenu == 22 && tKey == 1) handleSave(*event);
            if (currentMenu == 22) ansContinue(*event);
            if (currentMenu == 3) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyReleased>()) {
                    if (keyPressed->code == sf::Keyboard::Key::L) sMM = 22;
                }
            }
            if ((currentMenu == 3 || currentMenu == 22) && rKey == 1) handleRename(*event);
            if ((currentMenu == 3 || currentMenu == 22) && dKey == 1) {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyReleased>()) {
                    if (keyPressed->code == sf::Keyboard::Key::D) handleDelete();
                }               
            }
        }

        //window.clear(colorBackGround);
        window.clear();

        sf::Sprite backgroundSprite(texMain);
        sf::Vector2u textureSize = texMain.getSize();
        if (currentMenu == 1 || currentMenu == 20 || currentMenu == 21) {
            backgroundSprite.setTexture(texMenu, true);
            textureSize = texMenu.getSize();
        }
        else if (currentMenu == 3) {
            backgroundSprite.setTexture(texLoad, true);
            textureSize = texLoad.getSize();
        }
        else if (currentMenu == 4) {
            backgroundSprite.setTexture(texAbout, true);
            textureSize = texAbout.getSize();
        }
        else if (currentMenu == 5) {
            backgroundSprite.setTexture(texSet, true);
            textureSize = texSet.getSize();
        }
        else if (currentMenu == 22) {
            backgroundSprite.setTexture(texBoard, true);
            textureSize = texBoard.getSize();
        }

        sf::Vector2u windowSize = window.getSize();
        float scaleX = (float)windowSize.x / textureSize.x;
        float scaleY = (float)windowSize.y / textureSize.y;

        backgroundSprite.setScale(sf::Vector2f({ scaleX, scaleY }));

        window.draw(backgroundSprite);

        if (currentMenu == 1) showMainMenu();
        if (currentMenu == 20) {
            showPlayerMenu();
            //sMM = 20;
        }
        if (currentMenu == 21) showModeMenu(1);
        if (currentMenu == 22) {
            drawBoard();
            showPlayerInfo();

            window.draw(cursor);
            window.draw(saveText);
            window.draw(loadText);;
            window.draw(inputText);
            window.draw(nameFile);
            updateCellAtScreen(1, 1, 1, 1);

            if (lKey == 1 && tKey != 1) {
                drawOpaRec(1250, OPAREC_H, WINDOW_W / 2 - 25, WINDOW_H - OPAREC_H - 100, sf::Color(34, 37, 93));
                //sf::Texture texLoadInBoard("image/backgroundLoadInBoard.png");
                //sf::Sprite spriteTexLoadInBoard(texLoadInBoard);

                //spriteTexLoadInBoard.setOrigin(sf::Vector2f({ 1250 / 2, 0 }));
                //spriteTexLoadInBoard.setPosition(sf::Vector2f({ WINDOW_W / 2, WINDOW_H - OPAREC_H - 100 }));

                //window.draw(spriteTexLoadInBoard);

                displayListOfFile();
                showButtonLoad(22);

                if (drawRec == 1 && rKey == 1) {
                    window.draw(recBig);
                }
                window.draw(titleRecInput);
                if (drawRec == 1 && rKey == 1) window.draw(recSmall);
                window.draw(inputText);
            }

            if (tKey == 1 && lKey != 1) {
                if (drawRec == 1) window.draw(recBig);
                window.draw(titleRecInput);
                if (drawRec == 1) window.draw(recSmall);
                window.draw(inputText);
            }

            if (winText.getString() != "" && isKeyDown(Key::X)) {
                winText.setString("");
            }

            if (res == 0 || res == 1) {
                askContinue();
                if (res == 1) {
                    pair<pii, pii> res = getWinLine(board);

                    float xStart = res.ss.ss;
                    float yStart = res.ss.ff;
                    float xEnd = res.ff.ss;
                    float yEnd = res.ff.ff;
                    drawWinningLine(xStart, yStart, xEnd, yEnd);
                }

                if (winText.getString() != "") drawOpaRec(1200, 175, WINDOW_W / 2, WINDOW_H / 2 - 75, sf::Color(255, 255, 255, 175), 1);
            }

            window.draw(winText);
        }
        if (currentMenu == 3) {
            displayListOfFile();
            showButtonLoad(3);

            titleLoad.setString(langChoice == 1 ? L"LOAD GAME" : L"TẢI GAME");
            titleLoad.setCharacterSize(100);
            sf::FloatRect bounds = titleLoad.getLocalBounds();

            titleLoad.setOrigin(bounds.getCenter());
            titleLoad.setPosition(sf::Vector2f({ WINDOW_W / 2, OPAREC_Y - titleLoad.getCharacterSize() / 2 }));
            titleLoad.setFillColor(sf::Color(35, 71, 139));

            window.draw(titleLoad);

            if (rKey == 1) {
                if (drawRec == 1) window.draw(recBig);
                window.draw(titleRecInput);
                if (drawRec == 1) window.draw(recSmall);
                window.draw(inputText);
            }
        }
        if (currentMenu == 4) {
            showAbout();
        }
        if (currentMenu == 5) {
            showSettingsMenu();
        }
        if (currentMenu == 0) {
            currentMenu = processBar();

            window.draw(title);
            window.draw(grayBar);
            window.draw(colorBar);
        }
        if (currentMenu == -1) window.close();
        window.display();
    }

}
