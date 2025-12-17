#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Interface.h"

using Key = sf::Keyboard::Key;

extern int dKey;
extern int rKey;
extern int lKey;
extern int tKey;
extern int diffChoice;

void run(const sf::Event &event);
void handleBotPlay(const sf::Event& event);
void handleSave(const sf::Event &event);
void handleLoad(bool startFromMenu, const sf::Event &event);
void handleRename(const sf::Event& event);
void handleDelete();
bool isKeyDown(sf::Keyboard::Key key);
int handleLoadMiniBoard(size_t num);
void ansContinue(const sf::Event &event);

#endif
