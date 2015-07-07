//
//  game.h
//  FreeType2Demo
//
//  Created by Chukie on 15/7/5.
//  Copyright (c) 2015年 com.dream.freetype. All rights reserved.
//

#ifndef FreeType2Demo_game_h
#define FreeType2Demo_game_h



#include "Scene.h"
#include <string>
#include "glCommon.h"

using namespace std;

class Game
{
public:
    Game(int width,int height);
    
    void init();
    void deinit();
    
    void resizeWindow(int width,int height);
    
    void start();
    void stop();
    
    void run();
    
    void onEvent(SDL_Event*);
    void onKeydown(SDL_KeyboardEvent*);
    
    void tick();
    void draw();
    
protected:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_GLContext glcontext;
    
    
    Scene* curScene;
    int mWidth,mHeight;
    long ticks;
    std::string baseWindowTitle;
    bool keepGoing;
};

#endif
