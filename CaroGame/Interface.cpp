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
    cursor.setFillColor(sf::Color(173, 173, 173));
    cursor.setPosition(sf::Vector2f({ startX + 22, startY + 11 }));

    if (!texMain.loadFromFile("backgroundMainMenu.JPG")) std::cout << "Error at background of main menu\n";
    if (!texLoadSet.loadFromFile("backgroundLoad_Set.JPG")) std::cout << "Error at background of load/setting\n";
    if (!texAbout.loadFromFile("backgroundAbout.JPG")) std::cout << "Error at background of about\n";
    if (!texBoard.loadFromFile("backgroundBoard.png")) std::cout << "Error at background of board\n";
}

void backGround() {
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
            if (drawRec == 1) window.draw(recBig);
            window.draw(titleRecInput);
            if (drawRec == 1) window.draw(recSmall);
            window.draw(inputText);
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

//#include "giaoDien.h"
//#include "View.h"
//#include "controller.h"
//#include <optional>
//#include <algorithm>
//#include <vector>
//
//int currentMenu = 0;
//
//const sf::Font font("coiny-regular.ttf");
//sf::RenderWindow window(sf::VideoMode({ WINDOW_W, WINDOW_H }), "Bello");
//sf::Color colorBackGround = sf::Color::White;
//
//sf::Text cursor(font, "X", 35);
//sf::Text playerX(font, "X", 15);
//sf::Text playerO(font, "O", 15);
//sf::Text winText(font, "", 35);
//sf::Text continueText(font, "", 35);
//sf::Text saveText(font, "", 35);
//sf::Text loadText(font, "", 35);
//sf::Text inputText(font, "", 30);
//sf::Text nameFile(font, "", 30);
//
//
//static sf::RectangleShape grayBar;
//static sf::RectangleShape colorBar;
//
//static constexpr float barWidth = 1000.f;
//static constexpr float barHeight = 50.f;
//static constexpr float speed = 0.25f;
//static float currentWidthBar = 0.f;
//
//
//void drawPixelSky(sf::RenderWindow& window) {
//    sf::VertexArray sky(sf::PrimitiveType::TriangleStrip, 6);
//    auto s = window.getSize();
//
//    sf::Color top(30, 120, 200);
//    sf::Color mid(100, 180, 235);
//    sf::Color bottom(190, 230, 255);
//
//    sky[0] = { {0.f, 0.f}, top };
//    sky[1] = { {(float)s.x, 0.f}, top };
//
//    sky[2] = { {0.f, s.y * 0.5f}, mid };
//    sky[3] = { {(float)s.x, s.y * 0.5f}, mid };
//
//    sky[4] = { {0.f, (float)s.y}, bottom };
//    sky[5] = { {(float)s.x, (float)s.y}, bottom };
//
//    window.draw(sky);
//}
//
//void drawPixelCloud(sf::RenderWindow& window, float x, float y, int scale, sf::Color base, sf::Color highlight) {
//    static const int cloud[][2] = {
//        {2,0},{3,0},{4,0},
//        {1,1},{2,1},{3,1},{4,1},{5,1},
//        {0,2},{1,2},{2,2},{3,2},{4,2},{5,2},{6,2},
//        {1,3},{2,3},{3,3},{4,3},{5,3}
//    };
//
//    sf::RectangleShape px({ (float)scale, (float)scale });
//
//    px.setFillColor(base);
//    for (auto& p : cloud) {
//        px.setPosition({ x + p[0] * scale, y + p[1] * scale });
//        window.draw(px);
//    }
//
//    px.setFillColor(highlight);
//    px.setPosition({ x + 2 * scale, y });
//    window.draw(px);
//    px.setPosition({ x + 3 * scale, y });
//    window.draw(px);
//}
//
//void drawCloudLayers(sf::RenderWindow& window) {
//    drawPixelCloud(window, 120, 100, 16,
//        sf::Color(230, 245, 255),
//        sf::Color(255, 255, 255));
//
//    drawPixelCloud(window, 900, 140, 14,
//        sf::Color(230, 245, 255),
//        sf::Color(255, 255, 255));
//
//    drawPixelCloud(window, 300, 220, 24,
//        sf::Color(210, 235, 250),
//        sf::Color(255, 255, 255));
//
//    drawPixelCloud(window, 800, 260, 28,
//        sf::Color(210, 235, 250),
//        sf::Color(255, 255, 255));
//}
//
//void drawWideCloud(sf::RenderWindow& window, float x, float y, int s) {
//    sf::RectangleShape px({ (float)s, (float)s });
//    px.setFillColor(sf::Color(245, 250, 255));
//
//    int shape[][2] = {
//        {2,0},{3,0},{4,0},{5,0},
//        {1,1},{2,1},{3,1},{4,1},{5,1},{6,1},
//        {0,2},{1,2},{2,2},{3,2},{4,2},{5,2},{6,2},{7,2},
//        {1,3},{2,3},{3,3},{4,3},{5,3},{6,3}
//    };
//
//    for (auto& p : shape) {
//        px.setPosition({ x + p[0] * s, y + p[1] * s });
//        window.draw(px);
//    }
//}
//
//static void processBar() {
//    if (grayBar.getSize().x == 0) {
//        grayBar.setSize({ barWidth, barHeight });
//        grayBar.setPosition({ (WINDOW_W - barWidth) / 2.f, 700.f });
//        grayBar.setFillColor(sf::Color(120, 120, 120));
//
//        colorBar.setPosition(grayBar.getPosition());
//        colorBar.setFillColor(sf::Color::Cyan);
//    }
//
//    currentWidthBar = std::min(currentWidthBar + speed, barWidth);
//    colorBar.setSize({ currentWidthBar, barHeight });
//}
//
//sf::VertexArray drawGrid(int w, int h, int cellSize, sf::Color color) {
//    sf::VertexArray grid(sf::PrimitiveType::Lines);
//
//    for (int i = 0; i <= w; i += cellSize)
//    {
//        grid.append({ { (float)i, 0.f }, color });
//        grid.append({ { (float)i, (float)h }, color });
//    }
//
//    for (int j = 0; j <= h; j += cellSize)
//    {
//        grid.append({ { 0.f, (float)j }, color });
//        grid.append({ { (float)w, (float)j }, color });
//    }
//
//    return grid;
//}
//
//std::vector<sf::RectangleShape> createThickGrid(int w, int h, int cellSize, sf::Color color, float thickness, float offsetX, float offsetY) {
//    std::vector<sf::RectangleShape> lines;
//
//    for (int x = 0; x <= w; x += cellSize)
//    {
//        sf::RectangleShape line;
//        line.setSize({ thickness, (float)h });
//        line.setFillColor(color);
//        line.setPosition({
//            offsetX + x - thickness / 2.f,
//            offsetY
//            });
//        lines.push_back(line);
//    }
//
//    for (int y = 0; y <= h; y += cellSize)
//    {
//        sf::RectangleShape line;
//        line.setSize({ (float)w, thickness });
//        line.setFillColor(color);
//        line.setPosition({
//            offsetX,
//            offsetY + y - thickness / 2.f
//            });
//        lines.push_back(line);
//    }
//
//    return lines;
//}
//
//void backGround() {
//    sf::Text title(font, L"CARO GAME", 100);
//    sf::Text guide(font, "Press any key to start...", 30);
//
//    title.setFillColor(sf::Color::Black);
//    title.setStyle(sf::Text::Bold);
//    title.setPosition({ WINDOW_W / 2.f - 330.f, WINDOW_H / 2.f - 150.f });
//
//    guide.setFillColor(sf::Color::Red);
//    guide.setPosition({ WINDOW_W / 2.f - guide.getGlobalBounds().size.x / 2.f, 360.f });
//
//    while (window.isOpen()) {
//        while (const std::optional<sf::Event> event = window.pollEvent())
//        {
//            if (event->is<sf::Event::Closed>())
//                window.close();
//
//            run(*event);
//        }
//        window.clear();
//
//        drawPixelSky(window);
//        drawCloudLayers(window);
//        drawWideCloud(window, 120, 90, 18);
//        drawWideCloud(window, 700, 130, 16);
//        drawWideCloud(window, 300, 220, 26);
//
//        if (currentMenu == 0) {
//            processBar();
//            window.draw(title);
//            //window.draw(guide);
//            window.draw(grayBar);
//            window.draw(colorBar);
//
//            if (currentWidthBar >= barWidth) currentMenu = 1;
//        } else if (currentMenu == 1) {
//            showMainMenu();
//        } else if (currentMenu == 20) {
//            showPlayerMenu();
//        } else if (currentMenu == 22) {
//            drawBoard();
//            showPlayerInfo();
//            window.draw(cursor);
//            window.draw(winText);
//        }
//
//        window.display();
//    }
//}

