#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <Constants.h>
#include <Globals.h>
#include <Texture.h>
#include <Tile.h>

Tile::Tile( int x, int y, int tileType, int rotate )
{
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

    //Set the collision box
    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

    //Get the tile type
    mType = tileType;

    rotateAngle = rotate;

    itemType = NO_ITEM;

    tempItem = NO_ITEM;

    // if (mType == 0)
    // {
    //     mColliders.resize(1);
    //     mColliders[0].w = .1;
    //     mColliders[0].h = 128;
    //     mColliders[0].x = x + 128;
    //     mColliders[0].y = y;
    // }
    // else if (mType == 1)
    // {
    //     mColliders.resize(1);
    //     mColliders[0].w = 128;
    //     mColliders[0].h = .1;
    //     mColliders[0].x = x;
    //     mColliders[0].y = y + 128;
    // }
}

void Tile::render(SDL_Rect& camera )
{
    if (showPauseMenu == true)
    {
        if (time.isPaused() == true)
        {
            time.changeWasPaused();
        }
        else
        {
            time.pause();
        }
    }
    else
    {
        if (time.getWasPaused() == true)
        {
            time.changeWasPaused();
        }
        else
        {
            time.unpause();
        }
    }
    //If the tile is on screen
    if( checkBoxCollision( camera, mBox ) )
    {
        //Show the tile
        gTileSheetTextures.render(mBox.x - camera.x, mBox.y - camera.y, &gTileClips[ mType ], rotateAngle );
        gItemSheetTextures.render(mBox.x - camera.x, mBox.y - camera.y, &gItemClips[ itemType ]);
        if (time.isStarted() == true && (itemType == ITEM_YELLOW_BUCKET || itemType == ITEM_RED_BUCKET || itemType == ITEM_BLUE_BUCKET) )
        {
            if (time.getTicks()/1000.f > 5)
            {
                time.stop();
                if (TILE_BOTTLER == mType)
                {
                    itemType++;
                }
                else if (mType == TILE_PILL_MAKER)
                {
                    if (itemType == ITEM_YELLOW_BUCKET)
                    {
                        itemType = ITEM_YELLOW_PILLS;
                    }
                    else if (itemType == ITEM_BLUE_BUCKET)
                    {
                        itemType = ITEM_BLUE_PILLS;
                    }
                    else if (itemType == ITEM_RED_BUCKET)
                    {
                        itemType = ITEM_RED_PILLS;
                    }
                }
            }
            else
            {
                LTexture gTimeTexture;
                std::stringstream timeText;
                timeText.str("");
                timeText << (5 - std::round((time.getTicks() / 1000.f)));
                SDL_Color textColor = {0xFF, 0xFF, 0xFF};
                if (!gTimeTexture.loadFromRenderedText(gFont, timeText.str().c_str(), textColor))
                {
                    printf("Could not load time texture");
                }
                gTimeTexture.render(mBox.x + TILE_WIDTH/2 - gTimeTexture.getWidth()/2, mBox.y + TILE_HEIGHT/2 - gTimeTexture.getHeight()/2);
            }
        }
        else if (time.isStarted() == true && itemType == ITEM_BROKEN_SHOT)
        {
            if (time.getTicks()/1000.f > 5)
            {
                time.stop();
                int n = 0;
                if (screenNumber == SCREEN_LEVEL_1)
                {
                    n = 163;
                }
                else if (screenNumber == SCREEN_LEVEL_2)
                {
                    n = 4;
                }
                else if (screenNumber == SCREEN_LEVEL_3)
                {
                    n = 12;
                }
                if (tiles[n]->getType() >= TILE_SHOT_ONE)
                {
                    tiles[n]->changeType(tiles[n]->getType()+1);
                }
                else
                {
                   tiles[n]->changeType(TILE_SHOT_ONE); 
                }
                itemType = NO_ITEM;
            }
            else
            {
                LTexture gTimeTexture;
                std::stringstream timeText;
                timeText.str("");
                timeText << (5 - std::round((time.getTicks() / 1000.f)));
                SDL_Color textColor = {0xFF, 0xFF, 0xFF};
                if (!gTimeTexture.loadFromRenderedText(gFont, timeText.str().c_str(), textColor))
                {
                    printf("Could not load time texture");
                }
                gTimeTexture.render(mBox.x + TILE_WIDTH/2 - gTimeTexture.getWidth()/2, mBox.y + TILE_HEIGHT/2 - gTimeTexture.getHeight()/2);
            }
        }
        else if (time.isStarted() == true && itemType == ITEM_CLOSED_PILL_BOTTLE)
        {
            if (time.getTicks()/1000.f > 5)
            {
                time.stop();
                int n = 0;
                if (screenNumber == SCREEN_LEVEL_2)
                {
                    n = 1;
                }
                else if (screenNumber == SCREEN_LEVEL_3)
                {
                    n = 11;
                }
                if (tiles[n]->getType() >= TILE_OPENED_PILL_BOTTLE_ONE)
                {
                    tiles[n]->changeType(tiles[n]->getType()+1);
                }
                else
                {
                   tiles[n]->changeType(TILE_OPENED_PILL_BOTTLE_ONE); 
                }
                itemType = NO_ITEM;
            }
            else
            {
                LTexture gTimeTexture;
                std::stringstream timeText;
                timeText.str("");
                timeText << (5 - std::round((time.getTicks() / 1000.f)));
                SDL_Color textColor = {0xFF, 0xFF, 0xFF};
                if (!gTimeTexture.loadFromRenderedText(gFont, timeText.str().c_str(), textColor))
                {
                    printf("Could not load time texture");
                }
                gTimeTexture.render(mBox.x + TILE_WIDTH/2 - gTimeTexture.getWidth()/2, mBox.y + TILE_HEIGHT/2 - gTimeTexture.getHeight()/2);
            }
        }
        else if (time.isStarted() == true && itemType == ITEM_CLOSED_CAPSULE_BOTTLE)
        {
            if (time.getTicks()/1000.f > 5)
            {
                time.stop();
                int n = 0;
                if (screenNumber == SCREEN_LEVEL_3)
                {
                    n = 9;
                }
                if (tiles[n]->getType() >= TILE_OPENED_CAPSULE_BOTTLE_ONE)
                {
                    tiles[n]->changeType(tiles[n]->getType()+1);
                }
                else
                {
                   tiles[n]->changeType(TILE_OPENED_CAPSULE_BOTTLE_ONE); 
                }
                itemType = NO_ITEM;
            }
            else
            {
                LTexture gTimeTexture;
                std::stringstream timeText;
                timeText.str("");
                timeText << (5 - std::round((time.getTicks() / 1000.f)));
                SDL_Color textColor = {0xFF, 0xFF, 0xFF};
                if (!gTimeTexture.loadFromRenderedText(gFont, timeText.str().c_str(), textColor))
                {
                    printf("Could not load time texture");
                }
                gTimeTexture.render(mBox.x + TILE_WIDTH/2 - gTimeTexture.getWidth()/2, mBox.y + TILE_HEIGHT/2 - gTimeTexture.getHeight()/2);
            }
        }
        else if (time.isStarted() == true && (tempItem >= ITEM_YELLOW_CAPSULE_TRAY && tempItem <= ITEM_PURPLE_CAPSULE_TRAY) && (mType == TILE_CAPSULE_MAKER))
        {
            if (time.getTicks()/1000.f > 5)
            {
                time.stop();
                itemType = tempItem;
                tempItem = NO_ITEM;
                time.start();
            }
            else
            {
                LTexture gTimeTexture;
                std::stringstream timeText;
                timeText.str("");
                timeText << (5 - std::round((time.getTicks() / 1000.f)));
                SDL_Color textColor = {0xFF, 0xFF, 0xFF};
                if (!gTimeTexture.loadFromRenderedText(gFont, timeText.str().c_str(), textColor))
                {
                    printf("Could not load time texture");
                }
                gTimeTexture.render(mBox.x + TILE_WIDTH/2 - gTimeTexture.getWidth()/2, mBox.y + TILE_HEIGHT/2 - gTimeTexture.getHeight()/2);
            }
        }
        else if (time.isStarted() == true && (itemType >= ITEM_YELLOW_CAPSULE_TRAY && itemType <= ITEM_PURPLE_CAPSULE_TRAY) && (mType == TILE_CAPSULE_MAKER))
        {
            if (time.getTicks()/1000.f > 5)
            {
                time.stop();
                itemType = ITEM_GRAY_CAPSULE_TRAY;
            }
            else
            {
                LTexture gTimeTexture;
                std::stringstream timeText;
                timeText.str("");
                timeText << (5 - std::round((time.getTicks() / 1000.f)));
                SDL_Color textColor = {0xFF, 0x00, 0x00};
                if (!gTimeTexture.loadFromRenderedText(gFont, timeText.str().c_str(), textColor))
                {
                    printf("Could not load time texture");
                }
                gTimeTexture.render(mBox.x + TILE_WIDTH/2 - gTimeTexture.getWidth()/2, mBox.y + TILE_HEIGHT/2 - gTimeTexture.getHeight()/2);
            }
        }
    }
}

// std::vector<SDL_Rect>& Tile::getColliders()
// {
//     return mColliders;
// }

int Tile::getType()
{
    return mType;
}

void Tile::changeType(int type)
{
    mType = type;
}

int Tile::getTempItem()
{
    return tempItem;
}

void Tile::setTempItem(int type)
{
    tempItem = type;
}

int Tile::getItemType()
{
    return itemType;
}

SDL_Rect Tile::getBox()
{
    return mBox;
}

void Tile::showItem(int type)
{
    itemType = type;
}

void Tile::hideItem()
{
    itemType = NO_ITEM;
}

bool checkBoxCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }
    //If none of the sides from A are outside B
    return true;
}

bool checkTileCollision( SDL_Rect a, std::vector<SDL_Rect>& b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Go through the B boxes
    for( int Bbox = 0; Bbox < b.size(); Bbox++ )
    {
        //Calculate the sides of rect B
        leftB = b[ Bbox ].x;
        rightB = b[ Bbox ].x + b[ Bbox ].w;
        topB = b[ Bbox ].y;
        bottomB = b[ Bbox ].y + b[ Bbox ].h;

        //If no sides from A are outside of B
        if( ( ( bottomA <= topB ) || ( topA >= bottomB ) || ( rightA <= leftB ) || ( leftA >= rightB ) ) == false )
        {
            //A collision is detected
            return true;
        }
    }
    

    //If neither set of collision boxes touched
    return false;
}
