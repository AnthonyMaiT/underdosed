#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <Globals.h>
#include <Constants.h>
#include <Tile.h>
#include <Player.h>
#include <Order.h>
#include <Texture.h>

Player::Player(int x, int y)
{
    // Initialize the collision box
    mBox.x = x;
    mBox.y = y;
    mBox.w = playerWidth;
    mBox.h = playerHeight;

    // Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    itemType = NO_ITEM;
}

void Player::resetPlayer(int x, int y)
{
    // Initialize the collision box
    mBox.x = x;
    mBox.y = y;
    mBox.w = playerWidth;
    mBox.h = playerHeight;

    // Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    itemType = NO_ITEM;

    rotateAngle = 0;
}

void Player::handleEvent(SDL_Event &e)
{
    // If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        // Adjust the velocity
        switch (e.key.keysym.sym)
        {
            case SDLK_w:
                isRotated(0);
                mVelY -= PLAYER_VELOCITY;
                rotateAngle = 0;
                break;
            case SDLK_s:
                isRotated(180);
                mVelY += PLAYER_VELOCITY;
                rotateAngle = 180;
                break;
            case SDLK_a:
                isRotated(270);
                mVelX -= PLAYER_VELOCITY;
                rotateAngle = 270;
                break;
            case SDLK_d:
                isRotated(90);
                mVelX += PLAYER_VELOCITY;
                rotateAngle = 90;
                break;
            case SDLK_SPACE:
                if (itemType > NO_ITEM)
                {
                    dropItem();
                }
                else
                {
                    pickupItem();
                }
                break;
            case SDLK_LCTRL:
                startTask();
                break;
        }
    }
    // If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        // Adjust the velocity
        switch (e.key.keysym.sym)
        {
            case SDLK_w:
                mVelY += PLAYER_VELOCITY;
                break;
            case SDLK_s:
                mVelY -= PLAYER_VELOCITY;
                break;
            case SDLK_a:
                mVelX += PLAYER_VELOCITY;
                break;
            case SDLK_d:
                mVelX -= PLAYER_VELOCITY;
                break;
        }
    }
}

void Player::removeVel()
{
    mVelX = 0;
    mVelY = 0;
}

void Player::handleControllerEvent(SDL_Event& e)
{
     if (e.type == SDL_JOYAXISMOTION)
    {
        if (e.jaxis.axis == 0)
        {
            //Left of dead zone
            if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
            {
                isRotated(270);
                mVelX = -PLAYER_VELOCITY;
                rotateAngle = 270;
            }
            //Right of dead zone
            else if( e.jaxis.value > JOYSTICK_DEAD_ZONE )
            {
                isRotated(90);
                mVelX = PLAYER_VELOCITY;
                rotateAngle = 90;
            }
            else
            {
                mVelX = 0;
            }
            
        }
        else if (e.jaxis.axis == 1)
        {
            //Left of dead zone
            if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
            {
                isRotated(0);
                mVelY = -PLAYER_VELOCITY;
                rotateAngle = 0;
            }
            //Right of dead zone
            else if( e.jaxis.value > JOYSTICK_DEAD_ZONE )
            {
                isRotated(180);
                mVelY = PLAYER_VELOCITY;
                rotateAngle = 180;
            }
            else
            {
                mVelY = 0;
            }
            
        }
        
    }
    else if (e.type == SDL_JOYBUTTONDOWN)
    {
        if (e.jbutton.button == SDL_CONTROLLER_BUTTON_A)
        {
            if (itemType > NO_ITEM)
            {
                dropItem();
            }
            else
            {
                pickupItem();
            }
        }
        else if (e.jbutton.button == SDL_CONTROLLER_BUTTON_X)
        {
            startTask();
        }
    }
}

void Player::move()
{
    // Move the dot left or right
    mBox.x += mVelX;

    // If the dot went too far to the left or right or touched a wall
    if ((mBox.x < 0) || (mBox.x + playerWidth > SCREEN_WIDTH) || touchesWall(mBox))
    {
        // move back
        mBox.x -= mVelX;
    }

    // Move the dot up or down
    mBox.y += mVelY;

    // If the dot went too far up or down or touched a wall
    if ((mBox.y < 0) || (mBox.y + playerHeight > SCREEN_HEIGHT) || touchesWall(mBox))
    {
        // move back
        mBox.y -= mVelY;
    }
}

void Player::setCamera(SDL_Rect &camera)
{
    // Center the camera over the dot
    camera.x = (mBox.x + playerWidth / 2) - SCREEN_WIDTH / 2;
    camera.y = (mBox.y + playerHeight / 2) - SCREEN_HEIGHT / 2;

    // Keep the camera in bounds
    if (camera.x < 0)
    {
        camera.x = 0;
    }
    if (camera.y < 0)
    {
        camera.y = 0;
    }
    if (camera.x > SCREEN_WIDTH - camera.w)
    {
        camera.x = SCREEN_WIDTH - camera.w;
    }
    if (camera.y > SCREEN_HEIGHT - camera.h)
    {
        camera.y = SCREEN_HEIGHT - camera.h;
    }
}

void Player::dropItem()
{
    // Go through the tiles
    for (int i = 0; i < TOTAL_TILES; ++i)
    {
        SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
        if (checkBoxCollision(playerBox, tiles[i]->getBox()) && (tiles[i]->getType() >= TILE_TABLE))
        {
            if (((tiles[i]->getItemType()) == ITEM_NO_SHOT) && (itemType == ITEM_YELLOW_BOTTLE || itemType == ITEM_RED_BOTTLE || itemType == ITEM_BLUE_BOTTLE))
            {
                tiles[i]->showItem(itemType+1);
                itemType = NO_ITEM;
                break;
            }
            else if (((tiles[i]->getItemType()) == ITEM_RED_SHOT) && (itemType == ITEM_YELLOW_BOTTLE || itemType == ITEM_BLUE_BOTTLE))
            {
                if (itemType == ITEM_YELLOW_BOTTLE)
                {
                    tiles[i]->showItem(ITEM_ORANGE_SHOT);
                }
                else if (itemType == ITEM_BLUE_BOTTLE)
                {
                    tiles[i]->showItem(ITEM_PURPLE_SHOT);
                }
                itemType = NO_ITEM;
                break;
            }
            else if (((tiles[i]->getItemType()) == ITEM_YELLOW_SHOT) && (itemType == ITEM_RED_BOTTLE || itemType == ITEM_BLUE_BOTTLE))
            {
                if (itemType == ITEM_RED_BOTTLE)
                {
                    tiles[i]->showItem(ITEM_ORANGE_SHOT);
                }
                else if (itemType == ITEM_BLUE_BOTTLE)
                {
                    tiles[i]->showItem(ITEM_GREEN_SHOT);
                }
                itemType = NO_ITEM;
                break;  
            }
            else if (((tiles[i]->getItemType()) == ITEM_BLUE_SHOT) && (itemType == ITEM_RED_BOTTLE || itemType == ITEM_YELLOW_BOTTLE))
            {
                if (itemType == ITEM_RED_BOTTLE)
                {
                    tiles[i]->showItem(ITEM_PURPLE_SHOT);
                }
                else if (itemType == ITEM_YELLOW_BOTTLE)
                {
                    tiles[i]->showItem(ITEM_GREEN_SHOT);
                }
                itemType = NO_ITEM;
                break;  
            }
            else if (((tiles[i]->getItemType()) == ITEM_OPENED_PILL_BOTTLE) && (itemType == ITEM_YELLOW_PILLS || itemType == ITEM_RED_PILLS || itemType == ITEM_BLUE_PILLS))
            {
                if (itemType == ITEM_YELLOW_PILLS)
                {
                    tiles[i]->showItem(ITEM_YELLOW_PILL_BOTTLE);
                }
                else if (itemType == ITEM_RED_PILLS)
                {
                    tiles[i]->showItem(ITEM_RED_PILL_BOTTLE);
                }
                else if (itemType == ITEM_BLUE_PILLS)
                {
                    tiles[i]->showItem(ITEM_BLUE_PILL_BOTTLE);
                }
                itemType = NO_ITEM;
                break;
            }
            else if (((tiles[i]->getItemType()) == ITEM_RED_PILL_BOTTLE) && (itemType == ITEM_YELLOW_PILLS || itemType == ITEM_BLUE_PILLS))
            {
                if (itemType == ITEM_YELLOW_PILLS)
                {
                    tiles[i]->showItem(ITEM_ORANGE_PILL_BOTTLE);
                }
                else if (itemType == ITEM_BLUE_PILLS)
                {
                    tiles[i]->showItem(ITEM_PURPLE_PILL_BOTTLE);
                }
                itemType = NO_ITEM;
                break;
            }
            else if (((tiles[i]->getItemType()) == ITEM_YELLOW_PILL_BOTTLE) && (itemType == ITEM_RED_PILLS || itemType == ITEM_BLUE_PILLS))
            {
                if (itemType == ITEM_RED_PILLS)
                {
                    tiles[i]->showItem(ITEM_ORANGE_PILL_BOTTLE);
                }
                else if (itemType == ITEM_BLUE_PILLS)
                {
                    tiles[i]->showItem(ITEM_GREEN_PILL_BOTTLE);
                }
                itemType = NO_ITEM;
                break;  
            }
            else if (((tiles[i]->getItemType()) == ITEM_BLUE_PILL_BOTTLE) && (itemType == ITEM_RED_PILLS || itemType == ITEM_YELLOW_PILLS))
            {
                if (itemType == ITEM_RED_PILLS)
                {
                    tiles[i]->showItem(ITEM_PURPLE_PILL_BOTTLE);
                }
                else if (itemType == ITEM_YELLOW_PILLS)
                {
                    tiles[i]->showItem(ITEM_GREEN_PILL_BOTTLE);
                }
                itemType = NO_ITEM;
                break;  
            }
            else if (((tiles[i]->getItemType()) == ITEM_OPENED_CAPSULE_BOTTLE) && (itemType >= ITEM_YELLOW_CAPSULE_TRAY || itemType == ITEM_PURPLE_CAPSULE_TRAY))
            {
                tiles[i]->showItem(itemType + 6);
                itemType = NO_ITEM;
                break;  
            }
            else if (((tiles[i]->getType()) == TILE_CAPSULE_MAKER) && (tiles[i]->getTempItem() == NO_ITEM) && (itemType == ITEM_BLUE_BUCKET || itemType == ITEM_RED_BUCKET || itemType == ITEM_YELLOW_BUCKET))
            {
                if (itemType == ITEM_YELLOW_BUCKET)
                {
                    tiles[i]->setTempItem(ITEM_YELLOW_CAPSULE_TRAY);
                }
                else if (itemType == ITEM_RED_BUCKET)
                {
                    tiles[i]->setTempItem(ITEM_RED_CAPSULE_TRAY);
                }
                else if (itemType == ITEM_BLUE_BUCKET)
                {
                    tiles[i]->setTempItem(ITEM_BLUE_CAPSULE_TRAY);
                }
                tiles[i]->time.stop();
                tiles[i]->time.start();
                itemType = NO_ITEM;
                break;
            }
            else if (((tiles[i]->getType()) == TILE_CAPSULE_MAKER) && (tiles[i]->getTempItem() == ITEM_BLUE_CAPSULE_TRAY) && (itemType == ITEM_RED_BUCKET || itemType == ITEM_YELLOW_BUCKET))
            {
                if (itemType == ITEM_RED_BUCKET)
                {
                    tiles[i]->setTempItem(ITEM_PURPLE_CAPSULE_TRAY);
                }
                else if (itemType == ITEM_YELLOW_BUCKET)
                {
                    tiles[i]->setTempItem(ITEM_YELLOW_CAPSULE_BOTTLE);
                }
                tiles[i]->time.stop();
                tiles[i]->time.start();
                itemType = NO_ITEM;
                break;  
            }
            else if (((tiles[i]->getType()) == TILE_CAPSULE_MAKER) && (tiles[i]->getTempItem() == ITEM_RED_CAPSULE_TRAY) && (itemType == ITEM_BLUE_BUCKET || itemType == ITEM_YELLOW_BUCKET))
            {
                if (itemType == ITEM_BLUE_BUCKET)
                {
                    tiles[i]->setTempItem(ITEM_PURPLE_CAPSULE_TRAY);
                }
                else if (itemType == ITEM_YELLOW_BUCKET)
                {
                    tiles[i]->setTempItem(ITEM_ORANGE_CAPSULE_TRAY);
                }
                tiles[i]->time.stop();
                tiles[i]->time.start();
                itemType = NO_ITEM;
                break;  
            }
            else if (((tiles[i]->getType()) == TILE_CAPSULE_MAKER) && (tiles[i]->getTempItem() == ITEM_YELLOW_CAPSULE_TRAY) && (itemType == ITEM_BLUE_BUCKET || itemType == ITEM_RED_BUCKET))
            {
                if (itemType == ITEM_BLUE_BUCKET)
                {
                    tiles[i]->setTempItem(ITEM_PURPLE_CAPSULE_TRAY);
                }
                else if (itemType == ITEM_RED_BUCKET)
                {
                    tiles[i]->setTempItem(ITEM_ORANGE_CAPSULE_TRAY);
                }
                tiles[i]->time.stop();
                tiles[i]->time.start();
                itemType = NO_ITEM;
                break;  
            }
            else if (((tiles[i]->getType()) == TILE_CAPSULE_MAKER) && (tiles[i]->getTempItem() == ITEM_BLUE_CAPSULE_TRAY) && (itemType == ITEM_RED_BUCKET || itemType == ITEM_YELLOW_BUCKET))
            {
                if (itemType == ITEM_RED_BUCKET)
                {
                    tiles[i]->showItem(ITEM_PURPLE_CAPSULE_TRAY);
                }
                else if (itemType == ITEM_YELLOW_BUCKET)
                {
                    tiles[i]->showItem(ITEM_YELLOW_CAPSULE_BOTTLE);
                }
                tiles[i]->time.stop();
                tiles[i]->time.start();
                itemType = NO_ITEM;
                break;  
            }
            else if (((tiles[i]->getType()) == TILE_TRASH_CAN) && (itemType == 3 || itemType == 4 || itemType == 6 || itemType == 7 || itemType == 9 || itemType == 10 || itemType == 18 || itemType == 21 || itemType == 24 || itemType == 27 || (itemType >= ITEM_YELLOW_CAPSULE_TRAY && itemType <= ITEM_PURPLE_CAPSULE_TRAY)))
            {
                itemType = NO_ITEM;
                break;
            }
            else if (((tiles[i]->getItemType()) == NO_ITEM))
            {
                if ((tiles[i]->getType() == TILE_CONVEYER))
                {
                    int *item = std::find(std::begin(shotList), std::end(shotList), itemType);
                    int *pillItem = std::find(std::begin(pillList), std::end(pillList), itemType);
                    int *capsuleItem = std::find(std::begin(capsuleList), std::end(capsuleList), itemType);
                    if (item != std::end(shotList) || pillItem != std::end(pillList) || capsuleItem != std::end(capsuleList))
                    {
                        for (int j = 0; j < orders.size(); ++j)
                        {
                            if (orders[j]->getItemType() == itemType)
                            {
                                orders[j]->time.stop();
                                orders[j]->setItemType(NO_ITEM);
                                orders[j]->changePoint();
                                break;
                            }
                        }
                        itemType = NO_ITEM;
                        if (item != std::end(shotList))
                        {
                            if (!shotTimers[0]->isStarted())
                            {
                                shotTimers[0]->start();
                            }
                            else if (!shotTimers[1]->isStarted())
                            {
                                shotTimers[1]->start();
                            }
                            else if (!shotTimers[2]->isStarted())
                            {
                                shotTimers[2]->start();
                            }
                        }
                        else if (pillItem != std::end(pillList))
                        {
                            if (!pillTimers[0]->isStarted())
                            {
                                pillTimers[0]->start();
                            }
                            else if (!pillTimers[1]->isStarted())
                            {
                                pillTimers[1]->start();
                            }
                            else if (!pillTimers[2]->isStarted())
                            {
                                pillTimers[2]->start();
                            }
                        }
                        else if (capsuleList != std::end(capsuleList))
                        {
                            if (!capsuleTimers[0]->isStarted())
                            {
                                capsuleTimers[0]->start();
                            }
                            else if (!capsuleTimers[1]->isStarted())
                            {
                                capsuleTimers[1]->start();
                            }
                            else if (!capsuleTimers[2]->isStarted())
                            {
                                capsuleTimers[2]->start();
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else if ((tiles[i]->getType() == TILE_CAPSULE_MAKER || tiles[i]->getType() == TILE_TRASH_CAN))
                {
                    break;
                }
                else if ((tiles[i]->getType() >= TILE_NO_SHOT && tiles[i]->getType() <= TILE_BROKEN_SHOT_FOUR))
                {
                    break;
                }
                else if ((tiles[i]->getType() >= TILE_SHOT_ONE && tiles[i]->getType() <= TILE_SHOT_TWO))
                {
                    break;
                }
                else if ((tiles[i]->getType() >= TILE_CLOSED_PILL_BOTTLE_ONE && tiles[i]->getType() <= TILE_CLOSED_PILL_BOTTLE_FOUR))
                {
                    break;
                }
                else if ((tiles[i]->getType() >= TILE_OPENED_PILL_BOTTLE_ONE && tiles[i]->getType() <= TILE_OPENED_PILL_BOTTLE_FOUR))
                {
                    break;
                }
                else if ((tiles[i]->getType() >= TILE_CLOSED_CAPSULE_BOTTLE_ONE && tiles[i]->getType() <= TILE_CLOSED_CAPSULE_BOTTLE_FOUR))
                {
                    break;
                }
                else if ((tiles[i]->getType() >= TILE_OPENED_CAPSULE_BOTTLE_ONE && tiles[i]->getType() <= TILE_OPENED_CAPSULE_BOTTLE_FOUR))
                {
                    break;
                }
                else
                {
                    tiles[i]->showItem(itemType);
                    itemType = NO_ITEM;
                    break;
                }
                
            }
        }
    }
}

void Player::pickupItem()
{
    // Go through the tiles
    for (int i = 0; i < TOTAL_TILES; ++i)
    {
        if ((tiles[i]->getType() == TILE_YELLOW_GEN) && ((tiles[i]->getItemType()) == NO_ITEM))
        {
            SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
            // if the collision box touches the bottom wall
            if (checkBoxCollision(playerBox, tiles[i]->getBox()))
            {
                itemType = ITEM_YELLOW_BUCKET;
                break;
            }
        }
        else if ((tiles[i]->getType() == TILE_RED_GEN) && ((tiles[i]->getItemType()) == NO_ITEM))
        {
            SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
            // if the collision box touches the bottom wall
            if (checkBoxCollision(playerBox, tiles[i]->getBox()))
            {
                itemType = ITEM_RED_BUCKET;
                break;
            }
        }
        else if ((tiles[i]->getType() == TILE_BLUE_GEN) && ((tiles[i]->getItemType()) == NO_ITEM))
        {
            SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
            // if the collision box touches the bottom wall
            if (checkBoxCollision(playerBox, tiles[i]->getBox()))
            {
                itemType = ITEM_BLUE_BUCKET;
                break;
            }
        }
        else if ((tiles[i]->getType() >= TILE_BROKEN_SHOT_ONE && tiles[i]->getType() <= TILE_BROKEN_SHOT_FOUR))
        {
            SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
            // if the collision box touches the bottom wall
            if (checkBoxCollision(playerBox, tiles[i]->getBox()))
            {
                itemType = ITEM_BROKEN_SHOT;
                tiles[i]->changeType(tiles[i]->getType() - 1);
                break;
            }
        }
        else if ((tiles[i]->getType() >= TILE_SHOT_ONE && tiles[i]->getType() <= TILE_SHOT_FOUR))
        {
            SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
            // if the collision box touches the bottom wall
            if (checkBoxCollision(playerBox, tiles[i]->getBox()))
            {
                if (tiles[i]->getType() >= TILE_SHOT_TWO)
                {
                    tiles[i]->changeType(tiles[i]->getType() - 1);
                }
                else
                {
                    tiles[i]->changeType(TILE_NO_SHOT);
                }
                itemType = ITEM_NO_SHOT;
                break;
            }
        }
        else if ((tiles[i]->getType() >= TILE_CLOSED_PILL_BOTTLE_ONE && tiles[i]->getType() <= TILE_CLOSED_PILL_BOTTLE_FOUR))
        {
            SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
            // if the collision box touches the bottom wall
            if (checkBoxCollision(playerBox, tiles[i]->getBox()))
            {
                if (tiles[i]->getType() >= TILE_CLOSED_PILL_BOTTLE_TWO)
                {
                    tiles[i]->changeType(tiles[i]->getType() - 1);
                }
                else
                {
                    tiles[i]->changeType(TILE_NO_SHOT);
                }
                itemType = ITEM_CLOSED_PILL_BOTTLE;
                break;
            }
        }
        else if ((tiles[i]->getType() >= TILE_OPENED_PILL_BOTTLE_ONE && tiles[i]->getType() <= TILE_OPENED_PILL_BOTTLE_FOUR))
        {
            SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
            // if the collision box touches the bottom wall
            if (checkBoxCollision(playerBox, tiles[i]->getBox()))
            {
                if (tiles[i]->getType() >= TILE_OPENED_PILL_BOTTLE_TWO)
                {
                    tiles[i]->changeType(tiles[i]->getType() - 1);
                }
                else
                {
                    tiles[i]->changeType(TILE_NO_SHOT);
                }
                itemType = ITEM_OPENED_PILL_BOTTLE;
                break;
            }
        }
        else if ((tiles[i]->getType() >= TILE_CLOSED_CAPSULE_BOTTLE_ONE && tiles[i]->getType() <= TILE_CLOSED_CAPSULE_BOTTLE_FOUR))
        {
            SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
            // if the collision box touches the bottom wall
            if (checkBoxCollision(playerBox, tiles[i]->getBox()))
            {
                if (tiles[i]->getType() >= TILE_CLOSED_CAPSULE_BOTTLE_TWO)
                {
                    tiles[i]->changeType(tiles[i]->getType() - 1);
                }
                else
                {
                    tiles[i]->changeType(TILE_NO_SHOT);
                }
                itemType = ITEM_CLOSED_CAPSULE_BOTTLE;
                break;
            }
        }
        else if ((tiles[i]->getType() >= TILE_OPENED_CAPSULE_BOTTLE_ONE && tiles[i]->getType() <= TILE_OPENED_CAPSULE_BOTTLE_FOUR))
        {
            SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
            // if the collision box touches the bottom wall
            if (checkBoxCollision(playerBox, tiles[i]->getBox()))
            {
                if (tiles[i]->getType() >= TILE_OPENED_CAPSULE_BOTTLE_TWO)
                {
                    tiles[i]->changeType(tiles[i]->getType() - 1);
                }
                else
                {
                    tiles[i]->changeType(TILE_NO_SHOT);
                }
                itemType = ITEM_OPENED_CAPSULE_BOTTLE;
                break;
            }
        }
        else if ((tiles[i]->getType() >= TILE_TABLE) && ((tiles[i]->getItemType()) > NO_ITEM))
        {
            SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
            // if the collision box touches the bottom wall
            if (checkBoxCollision(playerBox, tiles[i]->getBox()))
            {
                itemType = tiles[i]->getItemType();
                tiles[i]->hideItem();
                if (tiles[i]->time.isStarted())
                {
                    tiles[i]->time.stop();
                }
                break;
            }
        }
    }
}

void Player::startTask()
{
    // Go through the tiles
    for (int i = 0; i < TOTAL_TILES; ++i)
    {
        if ((tiles[i]->getType() == TILE_BOTTLER || tiles[i]->getType() ==  TILE_PILL_MAKER) && ((tiles[i]->getItemType()) == ITEM_YELLOW_BUCKET || (tiles[i]->getItemType()) == ITEM_RED_BUCKET || (tiles[i]->getItemType()) == ITEM_BLUE_BUCKET))
        {
            SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
            // if the collision box touches the bottom wall
            if (checkBoxCollision(playerBox, tiles[i]->getBox()))
            {
                if (tiles[i]->time.isPaused())
                {
                    tiles[i]->time.unpause();
                }
                else if (!tiles[i]->time.isStarted())
                {
                    tiles[i]->time.start();
                }
                break;
            }
        }
        else if ((tiles[i]->getType() == TILE_BIOHAZRD) && ((tiles[i]->getItemType()) == ITEM_BROKEN_SHOT))
        {
            SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
            // if the collision box touches the bottom wall
            if (checkBoxCollision(playerBox, tiles[i]->getBox()))
            {
                if (tiles[i]->time.isPaused())
                {
                    tiles[i]->time.unpause();
                }
                else if (!tiles[i]->time.isStarted())
                {
                    tiles[i]->time.start();
                }
                break;
            }
        }
        else if ((tiles[i]->getType() == TILE_SINK) && ((tiles[i]->getItemType()) == ITEM_CLOSED_PILL_BOTTLE || tiles[i]->getItemType() == ITEM_CLOSED_CAPSULE_BOTTLE))
        {
            SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
            // if the collision box touches the bottom wall
            if (checkBoxCollision(playerBox, tiles[i]->getBox()))
            {
                if (tiles[i]->time.isPaused())
                {
                    tiles[i]->time.unpause();
                }
                else if (!tiles[i]->time.isStarted())
                {
                    tiles[i]->time.start();
                }
                break;
            }
        }
    }
}

void Player::isRotated(int newRotateAngle)
{
    if (rotateAngle != newRotateAngle)
    {
        // go through tilies
        for (int i = 0; i < TOTAL_TILES; ++i)
        {
            if ((tiles[i]->getType() == TILE_BOTTLER || tiles[i]->getType() == TILE_PILL_MAKER) && (tiles[i]->time.isStarted()))
            {
                SDL_Rect playerBox = setTableItemCollision(mBox, rotateAngle, PLAYER_VELOCITY);
                // if the collision box touches the bottom wall
                if (checkBoxCollision(playerBox, tiles[i]->getBox()))
                {
                    tiles[i]->time.pause();
                    break;
                }
            }
        }
    }
}

int Player::getItemType()
{
    return itemType;
}

void Player::render(SDL_Rect &camera, int playerNumber)
{
    // Show the dot
    gPlayerSheetTextures.render(mBox.x - camera.x, mBox.y - camera.y, &gPlayerClips[playerNumber*TOTAL_ITEMS + itemType], rotateAngle);
}

bool touchesWall(SDL_Rect box)
{
    // Go through the tiles
    for (int i = 0; i < TOTAL_TILES; ++i)
    {
        if ((tiles[i]->getType() >= 1))
        {
            // if the collision box touches the bottom wall
            if (checkBoxCollision(box, tiles[i]->getBox()))
            {
                return true;
            }
        }
    }

    // If no wall tiles were touched
    return false;
}

SDL_Rect setTableItemCollision(SDL_Rect mBox, int rotateAngle, int PLAYER_VELOCITY)
{
    SDL_Rect player_box = {mBox.x, mBox.y, mBox.w, mBox.h};
    if (rotateAngle == 0)
    {
        player_box = {mBox.x, mBox.y - PLAYER_VELOCITY, mBox.w, mBox.h};
    }
    else if (rotateAngle == 180)
    {
        player_box = {mBox.x, mBox.y + PLAYER_VELOCITY, mBox.w, mBox.h};
    }
    else if (rotateAngle == 90)
    {
        player_box = {mBox.x + PLAYER_VELOCITY, mBox.y, mBox.w, mBox.h};
    }
    else if (rotateAngle == 270)
    {
        player_box = {mBox.x - PLAYER_VELOCITY, mBox.y, mBox.w, mBox.h};
    }
    return player_box;
}