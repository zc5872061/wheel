//
//  game.cpp
//  FreeType2Demo
//
//  Created by Chukie on 15/7/5.
//  Copyright (c) 2015年 com.dream.freetype. All rights reserved.
//
#include "game.h"

Uint32 lastMouseState;

Game::Game(int w, int h) {
    mWidth = w;
    mHeight = h;
    ticks = 0;
    keepGoing = true;
    baseWindowTitle = "test ";
    curScene = NULL;
}

void Game::init()
{
    
    SDL_Init(SDL_INIT_EVERYTHING);
    
    if((window = SDL_CreateWindow(baseWindowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWidth, mHeight, SDL_WINDOW_OPENGL)) == 0) {
        
        stop();
        return;
    }
    
    if((renderer = SDL_CreateRenderer(window, -1, 0)) == NULL) {
        
        stop();
        return;
    }
    
    if((glcontext = SDL_GL_CreateContext(window)) == NULL) {
    
        stop();
        return;
    }
    glewInit();
    
    
    //Initialize OpenGL
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    resizeWindow(mWidth, mHeight);
    
    //lastMouseState = 0;
    curScene = new Scene();
    //Initialize things here

}

void Game::deinit()
{
    SDL_GL_DeleteContext(glcontext);
    
    SDL_DestroyWindow(window);
    
    SDL_Quit();
}

void Game::resizeWindow(int w,int h)
{
    if(h == 0)
        h = 1;
    
    GLfloat nRange = 200.0f;
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    glViewport(0, 0, mWidth, mHeight);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
//    if (w <= h)
//        glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
//    else
//        glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);
    glOrtho (0.0, mWidth, mHeight, 0, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glClearDepth(1.0f);
    glDisable(GL_CULL_FACE);
    
    
}

void Game::start()
{
    run();
}

void Game::stop()
{
    keepGoing = false;
}

void Game::run()
{
    init();
    
    SDL_Event event;
    
    //Timing Variables
    double msPerFrame = 1000.0 / 60.0;
    double unprocessed = 0.0;
    Uint32 lastTime = SDL_GetTicks();
    Uint32 lastFPSTime = SDL_GetTicks();
    int frames = 0;
    int ticks = 0;
    bool vSync = false;
    bool ticked = false;
    //
    
    
    while(keepGoing) {
        ticked = false;
        
        while(SDL_PollEvent(&event)) {
            onEvent(&event);
        }
        
        //Do timing (keep us running at 60 ticks per second)
        Uint32 now = SDL_GetTicks();
        unprocessed += (now - lastTime) / msPerFrame;
        lastTime = now;
        
        while(unprocessed > 1) {
            tick();
            unprocessed--;
            ticked = true;
            ticks++;
        }
        if((ticked && vSync) || !vSync) {
            draw();
            frames++;
        }
        
        if(now > lastFPSTime + 1000) {
            //Put the FPS and ticks in the window title
            //std::stringstream ss;
            //ss << baseWindowTitle << "FPS: "<< frames << " Ticks: " << ticks << " # of Lights: " << curScene->lights.size();
            SDL_SetWindowTitle(window, "test");
            lastFPSTime += 1000;
            frames = 0;
            ticks = 0;
        }
        
        //Lets let the computer rest
        SDL_Delay(1);
    }
    
    deinit();
}

void Game::onEvent(SDL_Event* event)
{
    switch(event->type) {
        case SDL_QUIT:
            keepGoing = false;
            break;
        case SDL_KEYDOWN:
            onKeydown(&event->key);
            break;
    }
}

void Game::onKeydown(SDL_KeyboardEvent* key)
{
    switch(key->keysym.sym) {
        case SDLK_ESCAPE:
            stop();
            break;
    }
}

void Game::tick()
{
    lastMouseState = SDL_GetMouseState(NULL, NULL);
    ticks++;
    curScene->tick();
}

void Game::draw()
{
    glLoadIdentity();
    //glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    curScene->draw();
    
        
    SDL_GL_SwapWindow(window);
}
