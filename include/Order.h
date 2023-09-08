#pragma once

#include <SDl2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <Texture.h>
#include <vector>
#include <Timer.h>
#include <sstream>

class Order
{
    public:
		// bottler timer
		LTimer time;

		//Initializes position and type
		Order( int type );

		int getItemType();

        void render(int x);

		void setItemType(int type = 0);

		bool hasPoint();

		void changePoint();

        //Gets the collision boxes
		// std::vector<SDL_Rect>& getColliders();

    private:

		bool point;

		// Item type
		int itemType;

        //Tile's collision boxes
	    // std::vector<SDL_Rect> mColliders;
};