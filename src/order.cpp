#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <Globals.h>
#include <Constants.h>
#include <Texture.h>
#include <Order.h>

Order::Order(int type)
{
    itemType = type;
    point = false;
    time.start();
}

void Order::render(int x)
{
    if (time.isStarted())
    {
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_Rect outlineRect = {x*128, 0, 128, 64};
        SDL_RenderDrawRect(gRenderer, &outlineRect);

        gItemSheetTextures.render((x*128), 0, &gItemClips[itemType]);

        LTexture gTimeTexture;
        std::stringstream timeText;
        timeText.str("");
        timeText << (30 - std::round((time.getTicks() / 1000.f)));
        SDL_Color textColor = {0, 0, 0, 0xFF};
        if (!gTimeTexture.loadFromRenderedText(gFont, timeText.str().c_str(), textColor))
        {
            printf("Could not load time texture");
        }
        gTimeTexture.render(x*128 + 64, 32 - gTimeTexture.getHeight()/2);
    }
}

void Order::setItemType(int type)
{
    itemType = type;
}

void Order::changePoint()
{
    point = !point;
}

bool Order::hasPoint()
{
    return point;
}

int Order::getItemType()
{
    return itemType;
}