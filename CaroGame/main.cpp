#include "Controller.h"
#include "AudioManager.h"
#include<windows.h>
#include "View.h"
#include<iostream>
int main() {
    fixConsoleWindow();
    system("pause");
    setConsoleFont();
    SetConsoleOutputCP(65001);
    AudioManager::getInstance().init();
    setColor(240);
    showSplashScreen();
    AudioManager::getInstance().playBackgroundMusic("audio/background_music.wav");
    SetConsoleCP(65001);
    system("color F0");
    system("cls");
    run();
    AudioManager::getInstance().cleanup();
    return 0;
}
