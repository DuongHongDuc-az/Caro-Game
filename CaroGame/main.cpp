#include "Controller.h"
#include "AudioManager.h"
#include<windows.h>
#include "View.h"
#include<iostream>
int main() {
    void setConsoleFont();
    AudioManager::getInstance().init();
    AudioManager::getInstance().playBackgroundMusic("audio/background_music.wav");
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    system("color F0");
    system("cls");
    run();
    AudioManager::getInstance().cleanup();
    return 0;
}
