#pragma once

#include <SDl2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <Texture.h>
#include <vector>
#include <Timer.h>
#include <sstream>

bool checkBoxCollision( SDL_Rect a, SDL_Rect b );
bool checkTileCollision( SDL_Rect a, std::vector<SDL_Rect>& b );

class Tile
{
    public:
		// bottler timer
		LTimer time;

		//Initializes position and type
		Tile( int x, int y, int tileType, int rotate );

		//Shows the tile
		void render(SDL_Rect& camera );

		//Get the tile type
		int getType();

		void changeType(int type);

		//Get the collision box
		SDL_Rect getBox();

		void showItem(int type);

		void hideItem();

		int getTempItem();

		void setTempItem(int type);

		int getItemType();

        //Gets the collision boxes
		// std::vector<SDL_Rect>& getColliders();

    private:
		//The attributes of the tile
		SDL_Rect mBox;

		// Item type
		int itemType;

		int tempItem;

		int rotateAngle;

        //Tile's collision boxes
	    // std::vector<SDL_Rect> mColliders;

		//The tile type
		int mType;
};