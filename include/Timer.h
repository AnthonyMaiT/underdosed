#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>

class LTimer
{
    public:
        // init variables
        LTimer();

        // the various clock actions
        void start();
        void stop();
        void pause();
        void unpause();

        // gets the timer's name
        Uint32 getTicks();

        // checks the status of the timer
        bool isStarted();
        bool isPaused();
        bool getWasPaused();
        void changeWasPaused();
    private:
        // the clock time when the timer started
        Uint32 mStartTicks;
        // the ticks stored when the timer was paused
        Uint32 mPausedTicks;
        // the timer status
        bool mPaused;
        bool mStarted;
        bool wasPaused;
};