#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include <Constants.h>
#include <Timer.h>
#include <Texture.h>
#include <Tile.h>
#include <Order.h>

//The dot that will move around on the screen
class Player
{
    public:
		//Maximum axis velocity of the dot
		static const int PLAYER_VELOCITY = 8;

		//Initializes the variables
		Player(int x, int y);

		void resetPlayer(int x, int y);

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot and check collision against tiles
		void move(  );

		void handleControllerEvent(SDL_Event& e);

		//Centers the camera over the dot
		void setCamera( SDL_Rect& camera );

		void dropItem();

		void pickupItem();

		void startTask();

		void isRotated(int newRotateAngle);
		
		int getItemType();

		void removeVel();

		//Shows the dot on the screen
		void render(SDL_Rect &camera, int playerNumber);

    private:
		// width/height of player
		int playerWidth = 40, playerHeight = 40;
		// rotated
		int rotateAngle = 0;
		//Collision box of the dot
		SDL_Rect mBox;

		// has Item
		int itemType;

		//The velocity of the dot
		int mVelX, mVelY;
};

bool touchesWall(SDL_Rect box);
SDL_Rect setTableItemCollision(SDL_Rect mBox, int rotateAngle, int PLAYER_VELOCITY);