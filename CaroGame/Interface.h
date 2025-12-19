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

extern bool isSoundOn;
extern bool isMoving;
extern float thumbSpeed;
extern float trackW;
extern float trackH;
extern float thumbW; 
extern float thumbH;
extern int currentMenu;

extern const float OPAREC_W;
extern const float OPAREC_H;
extern const float OPAREC_X;
extern const float OPAREC_Y;
extern const sf::Font font;
extern sf::RenderWindow window;
extern sf::Color colorBackGround;
extern sf::RectangleShape recBig;
extern sf::RectangleShape recSmall;
extern sf::RectangleShape toggleTrack;
extern sf::RectangleShape toggleThumb;
extern sf::Text titleRecInput;
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

void soundBar(int w);
void initKeyToggle(sf::Vector2f pos);
void drawOpaRec(float w, float h, float x, float y, sf::Color color);
void declare();
void startGame();
void drawWinningLine(float xStart, float yStart, float xEnd, float yEnd);
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
