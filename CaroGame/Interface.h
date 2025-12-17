#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Window/Export.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Utf.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <iostream>

//using namespace sf;

const int CELLSIZE = 75;
const int WINDOW_W = 1500;
const int WINDOW_H = 800;

extern int currentMenu;
extern const sf::Font font;
extern const float OPAREC_W;
extern const float OPAREC_H;
extern const float OPAREC_X;
extern const float OPAREC_Y;
extern sf::RenderWindow window;
extern sf::Text titleRecInput;
extern sf::RectangleShape recBig;
extern sf::RectangleShape recSmall;
extern sf::Color colorBackGround;
extern sf::Text cursor;
extern sf::Text playerX;
extern sf::Text playerO;
extern sf::Text winText;
extern sf::Text continueText;
extern sf::Text saveText;
extern sf::Text loadText;
extern sf::Text inputText;
extern sf::Text nameFile;
extern sf::Text titleLoad;

struct Star {
    float x, y;
    float brightness;
};

void drawOpaRec(float w, float h, float x, float y, sf::Color color);
void declare();
void backGround();
sf::VertexArray drawGrid(int w, int h, int cellSize, sf::Color color);
std::vector<sf::RectangleShape> createThickGrid(int w, int h, int cellSize, sf::Color color, float thickness, float offsetX, float offsetY);



//#pragma once
//#include <SFML/Graphics.hpp>
//#include <SFML/Window/Event.hpp>
//#include <SFML/System/Clock.hpp>
//
//#include <vector>
//#include <string>
//#include <iostream>
//
//// ================== CONSTANT ==================
//const int CELLSIZE = 75;
//const int WINDOW_W = 1500;
//const int WINDOW_H = 800;
//
//// ================== GLOBAL ==================
//extern int currentMenu;
//extern const sf::Font font;
//extern sf::RenderWindow window;
//extern sf::Color colorBackGround;
//
//extern sf::Text cursor;
//extern sf::Text playerX;
//extern sf::Text playerO;
//extern sf::Text winText;
//extern sf::Text continueText;
//extern sf::Text saveText;
//extern sf::Text loadText;
//extern sf::Text inputText;
//extern sf::Text nameFile;
//
//// ================== UI / BACKGROUND ===========
//void backGround();
//
//// Sky background
//void drawGradientSky(sf::RenderWindow& window);
//void drawPixelClouds(sf::RenderWindow& window);
//
//// Grid
//sf::VertexArray drawGrid(int w, int h, int cellSize, sf::Color color);
//std::vector<sf::RectangleShape> createThickGrid(
//    int w, int h,
//    int cellSize,
//    sf::Color color,
//    float thickness,
//    float offsetX,
//    float offsetY
//);
