#include "Controller.h"
#include "AudioManager.h"
#include <windows.h>
#include "View.h"
#include <iostream>
#include "Interface.h"
int main() {
    AudioManager::getInstance().init();
    AudioManager::getInstance().playBackgroundMusic("audio/background_music.wav");
    startGame();
    AudioManager::getInstance().cleanup();  
    
    return 0;
}
