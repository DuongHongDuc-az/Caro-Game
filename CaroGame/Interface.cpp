#include "Interface.h"
#include "View.h"
#include "controller.h"

int currentMenu = 0;
const sf::Font font("Boheiman.ttf");
const float OPAREC_W = 1200;
const float OPAREC_H = 600;
const float OPAREC_X = WINDOW_W / 2;
const float OPAREC_Y = WINDOW_H - OPAREC_H - 60;
sf::RenderWindow window(sf::VideoMode({ WINDOW_W, WINDOW_H }), "Bello");
sf::Color colorBackGround = sf::Color::Black;
sf::Text titleRecInput(font, "", 50);
sf::RectangleShape recBig;
sf::RectangleShape recSmall;
sf::Text cursor(font, "X", 35);
sf::Text playerX(font, "X", 15);
sf::Text playerO(font, "O", 15);
sf::Text winText(font, "", 35);
sf::Text continueText(font, "", 35);
sf::Text saveText(font, "", 35);
sf::Text loadText(font, "", 10);
sf::Text inputText(font, "", 30);
sf::Text nameFile(font, "", 30);
sf::Text titleLoad(font, "", 80);

static sf::Texture texMain;
static sf::Texture texLoadSet;
static sf::Texture texAbout;
static sf::Texture texBoard;
static sf::RectangleShape grayBar;
static sf::RectangleShape colorBar;
static const float BAR_WIDTH = 1000;
static const float BAR_HEIGHT = 50;
static const float SPEED = 0.3; 
static float currentWidthBar = 0;

void drawOpaRec(float w, float h, float x, float y, sf::Color color) {
    sf::RectangleShape opaRec;

    opaRec.setSize(sf::Vector2f({w, h}));
    opaRec.setOrigin(sf::Vector2f({OPAREC_W / 2, 0}));
    opaRec.setPosition(sf::Vector2f({ x, y }));
    opaRec.setFillColor(color);

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

sf::VertexArray drawGrid(int w, int h, int cellSize, sf::Color color) {
    const sf::Color colorGrid(color);
    sf::VertexArray grid(sf::PrimitiveType::Lines);

    for (int i = 0; i <= w; i += cellSize) {
        grid.append(sf::Vertex{ sf::Vector2f((float)i, 0), colorGrid });
        grid.append(sf::Vertex{ sf::Vector2f((float)i, (float)h), colorGrid });
    }

    for (int j = 0; j <= h; j += cellSize) {
        grid.append(sf::Vertex{ sf::Vector2f(0, (float)j), colorGrid });
        grid.append(sf::Vertex{ sf::Vector2f((float)w, (float)j), colorGrid });
    }
    return grid;
}

void declare() {
    cursor.setFillColor(sf::Color::Black);
    cursor.setPosition(sf::Vector2f({ startX + 22, startY + 11 }));

    if (!texMain.loadFromFile("backgroundMainMenu.JPG")) std::cout << "Error at background of main menu\n";
    if (!texLoadSet.loadFromFile("backgroundLoad_Set.JPG")) std::cout << "Error at background of load/setting\n";
    if (!texAbout.loadFromFile("backgroundAbout.JPG")) std::cout << "Error at background of about\n";
    if (!texBoard.loadFromFile("backgroundBoard.png")) std::cout << "Error at background of board\n";
}

void backGround() {
    currentMenu = 3;

    const sf::Color colorGrid(128, 128, 128, 100);
    sf::Font fontTitle("Pixelic.ttf");
    sf::Text title(fontTitle, L"CARO GAME", 150);

    sf::FloatRect bounds = title.getLocalBounds();

    title.setFillColor(sf::Color::White);
    title.setOrigin(bounds.getCenter());
    title.setPosition(sf::Vector2f({ WINDOW_W / 2, WINDOW_H / 2 - 125}));
    title.setStyle(sf::Text::Bold);

    declare();
    
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* keyL = event->getIf<sf::Event::KeyPressed>()) {
                if (lKey == -1 && keyL->code != sf::Keyboard::Key::L) {
                    lKey = 0;
                }
            }
            //if (event->is<sf::Event::KeyPressed>()) {
            //}
            run(*event);

            //if (currentMenu == 21) handleBotPlay(*event);
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
            if (currentMenu == 3 && rKey == 1) handleRename(*event);
            if (currentMenu == 3 && dKey == 1) {
                handleDelete();
            }
        }
        
        //window.clear(colorBackGround);
        window.clear();

        sf::Sprite backgroundSprite(texMain);
        sf::Vector2u textureSize = texMain.getSize();
        if (currentMenu == 3 || currentMenu == 5) {
            backgroundSprite.setTexture(texLoadSet, true);
            textureSize = texLoadSet.getSize();
        }
        else if (currentMenu == 4) {
            backgroundSprite.setTexture(texAbout, true);
            textureSize = texAbout.getSize();
        }
        else if (currentMenu == 22) {
            backgroundSprite.setTexture(texBoard, true);
            textureSize = texBoard.getSize();
        }
        
        sf::Vector2u windowSize = window.getSize();
        float scaleX = (float)windowSize.x / textureSize.x;
        float scaleY = (float)windowSize.y / textureSize.y;
        
        backgroundSprite.setScale(sf::Vector2f({ scaleX, scaleY }));

        //window.draw(drawGrid(WINDOW_W, WINDOW_H, CELLSIZE, { 128, 128, 128, 100 }));
        window.draw(backgroundSprite);

        if (currentMenu == 1) showMainMenu();
        if (currentMenu == 20) showPlayerMenu();
        if (currentMenu == 21) showModeMenu(1);
        if (currentMenu == 22) {
            drawBoard();
            showPlayerInfo();

            window.draw(cursor);
            window.draw(continueText);
            window.draw(saveText);
            window.draw(loadText);;
            window.draw(inputText);
            window.draw(nameFile);
            updateCellAtScreen(1, 1, 1, 1);

            window.draw(winText);

            if (lKey == 1) {
                drawOpaRec(1400, OPAREC_H, WINDOW_W / 2 - 100, WINDOW_H - OPAREC_H - 100, sf::Color::Blue);
                displayListOfFile();
                showButtonLoad();

                if (drawRec == 1) window.draw(recBig);
                window.draw(titleRecInput);
                if (drawRec == 1) window.draw(recSmall);
                window.draw(inputText);
            }

            if (tKey == 1) {
                std::cout << "tKey: " << drawRec << "\n";

                if (drawRec == 1) window.draw(recBig);
                window.draw(titleRecInput);
                if (drawRec == 1) window.draw(recSmall);
                window.draw(inputText);
            }

            if (winText.getString() != "" && isKeyDown(Key::X)) {
                winText.setString("");
            }

            if (res == 1) askContinue();
        }
        if (currentMenu == 3) {
            drawOpaRec(1400, OPAREC_H, WINDOW_W /2 - 100, WINDOW_H - OPAREC_H - 50, sf::Color(0, 0, 0, 150));
            displayListOfFile();
            showButtonLoad();

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
            drawOpaRec(OPAREC_W, OPAREC_H, OPAREC_X, OPAREC_Y, sf::Color(0, 0, 0, 50));
        }
        if (currentMenu == 5) {
            showSettingsMenu();
            drawOpaRec(OPAREC_W, OPAREC_H, OPAREC_X, OPAREC_Y, sf::Color(0, 0, 0, 50));
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
