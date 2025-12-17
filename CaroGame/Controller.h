#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "giaoDien.h"

using Key = sf::Keyboard::Key;

extern int lKey;
extern int tKey;

void run(const sf::Event &event);
void handleBotPlay();
void handleSave(const sf::Event &event);
void handleLoad(bool startFromMenu, const sf::Event &event);
bool isKeyDown(sf::Keyboard::Key key);

#endif
