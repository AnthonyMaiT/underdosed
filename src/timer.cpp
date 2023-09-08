#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include <Timer.h>

LTimer::LTimer()
{
    // init variables
    mStartTicks = 0;
    mPausedTicks = 0;
    
    mPaused = false;
    mStarted = false;
    wasPaused = false;
}

void LTimer::start()
{
    // start the timer
    mStarted = true;
    // unpause timer
    mPaused = false;
    // get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void LTimer::stop()
{
    // stop the timer
    mStarted = false;
    // unpause the timer
    mPaused = false;
    // clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause()
{
    // if the timer is running and isnt already paused
    if (mStarted && !mPaused)
    {
        // pause the timer
        mPaused = true;
        // calc the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    // if the timer is running and paused
    if(mStarted && mPaused)
    {
        // unpause the timer
        mPaused = false;
        //reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;
        // reset the paused ticks
        mPausedTicks = 0;
    }
}

bool LTimer::getWasPaused()
{
    return wasPaused;
}

void LTimer::changeWasPaused()
{
    wasPaused = !wasPaused;
}

Uint32 LTimer::getTicks()
{
    // the actual timer time
    Uint32 time = 0;
    // if the timer is running
    if (mStarted)
    {
        //if the timer is paused
        if (mPaused)
        {
            // return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else 
        {
            // return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }
    return time;
}

bool LTimer::isStarted()
{
    // timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
    // timer is running and paused
    return mPaused && mStarted;
}