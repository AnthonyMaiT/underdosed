#include "Globals.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>

#include <Constants.h>
#include <Timer.h>
#include <Texture.h>
#include <Tile.h>
#include <Order.h>
#include <Player.h>
#include <Globals.h>

bool init()
{
    //Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "UNDERDOSED", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
                //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
                //Initialize SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
				if (SDL_NumJoysticks() < 1)
				{
					printf("No controllers connected\n");
				}
				else
				{
					// check if first joystick is game controller interface compatible
					if (!SDL_IsGameController(0))
					{
						printf("Controller 1-3 not connected\n");
					}
					else
					{
						// open game controller
						secondPlayerController = SDL_GameControllerOpen(0);
						if (!SDL_IsGameController(1))
						{
							printf("Controller 2-3 not connected\n");
						}
						else
						{
							// open game controller
							thirdPlayerController = SDL_GameControllerOpen(1);
							if (!SDL_IsGameController(2))
							{
								printf("Controller 3 not connected\n");
							}
							else
							{
								// open game controller
								fourthPlayerController = SDL_GameControllerOpen(2);
							}
						}
					}
				}
			}
		}
	}
    return success;
}

bool setItemClips()
{
	bool itemLoaded = true;
	int xSheet = 0;
	int ySheet = 0;
	// clip the sprite sheet
	for (int i = 0; i < TOTAL_ITEMS; ++i)
	{
		gItemClips[i].x = xSheet;
		gItemClips[i].y = ySheet;
		gItemClips[i].w = TILE_WIDTH;
		gItemClips[i].h = TILE_HEIGHT;

		xSheet += TILE_WIDTH;
		if (xSheet >= TILE_WIDTH*3)
		{
			ySheet += TILE_HEIGHT;
			xSheet = 0;
		}
	}
	return itemLoaded;
}

bool setPlayerClips()
{
	bool playerLoaded = true;
	int xSheet = 0;
	int ySheet = 0;
	// clip the sprite sheet
	for (int i = 0; i < TOTAL_ITEMS*4; ++i)
	{
		gPlayerClips[i].x = xSheet;
		gPlayerClips[i].y = ySheet;
		gPlayerClips[i].w = PLAYER_WIDTH;
		gPlayerClips[i].h = PLAYER_HEIGHT;

		xSheet += PLAYER_WIDTH;
		if (xSheet >= PLAYER_WIDTH*TOTAL_ITEMS)
		{
			ySheet += PLAYER_HEIGHT;
			xSheet = 0;
		}
	}
	return playerLoaded;
}

bool setTileClipsLevelSelect()
{
	bool tilesLoaded = true;
	// tile off set
	int x = 0, y = 0;
	// open the map
	std::ifstream map("res/levelSelect.map");
	if (map.fail())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		for (int i = 0; i < TOTAL_HOME_TILES; ++i)
		{
			// determines what tile will be made
			int tileType = -1;
			// read tile from map file
			map >> tileType;
			if (map.fail())
			{
				// stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}
			// if the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				homeTiles[i] = new Tile(x, y, tileType, 0);
			}
			// if don't recognize the tile type
			else
			{
				// stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			// move to next spot
			x += TILE_WIDTH;

			// if we've gone too far
			if (x >= SCREEN_WIDTH)
			{
				// move back
				x = 0;
				// move to the next row
				y += TILE_HEIGHT;
			}
		}
		if (tilesLoaded)
		{
			int xSheet = 0;
			int ySheet = 0;
			// clip the sprite sheet
			for (int i = 0; i < TOTAL_TILE_SPRITES; ++i)
			{
				gTileClips[i].x = xSheet;
				gTileClips[i].y = ySheet;
				gTileClips[i].w = TILE_WIDTH;
				gTileClips[i].h = TILE_HEIGHT;

				xSheet += TILE_WIDTH;
				if (xSheet >= TILE_WIDTH*5)
				{
					ySheet += TILE_HEIGHT;
					xSheet = 0;
				}
			}
			
		}
	}
	// close the file
	map.close();
	// if the map was loaded fine
	return tilesLoaded;
}

bool setTileClipsHome()
{
	bool tilesLoaded = true;
	// tile off set
	int x = 0, y = 0;
	// open the map
	std::ifstream map("res/home.map");
	if (map.fail())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		for (int i = 0; i < TOTAL_HOME_TILES; ++i)
		{
			// determines what tile will be made
			int tileType = -1;
			// read tile from map file
			map >> tileType;
			if (map.fail())
			{
				// stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}
			// if the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				homeTiles[i] = new Tile(x, y, tileType, 0);
			}
			// if don't recognize the tile type
			else
			{
				// stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			// move to next spot
			x += TILE_WIDTH;

			// if we've gone too far
			if (x >= SCREEN_WIDTH)
			{
				// move back
				x = 0;
				// move to the next row
				y += TILE_HEIGHT;
			}
		}
		if (tilesLoaded)
		{
			int xSheet = 0;
			int ySheet = 0;
			// clip the sprite sheet
			for (int i = 0; i < TOTAL_TILE_SPRITES; ++i)
			{
				gTileClips[i].x = xSheet;
				gTileClips[i].y = ySheet;
				gTileClips[i].w = TILE_WIDTH;
				gTileClips[i].h = TILE_HEIGHT;

				xSheet += TILE_WIDTH;
				if (xSheet >= TILE_WIDTH*5)
				{
					ySheet += TILE_HEIGHT;
					xSheet = 0;
				}
			}
			
		}
	}
	// close the file
	map.close();
	// if the map was loaded fine
	return tilesLoaded;
}

bool setTileClipsLevel1()
{
	bool tilesLoaded = true;
	// tile off set
	int x = 0, y = 64;
	// open the map
	std::ifstream map("res/level1.map");
	if (map.fail())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			// determines what tile will be made
			int tileType = -1;
			// read tile from map file
			map >> tileType;
			if (map.fail())
			{
				// stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}
			// if the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tiles[i] = new Tile(x, y, tileType, 0);
				if (i >= 170 && i <= 172)
				{
					tiles[i]->showItem(ITEM_NO_SHOT);
				}
			}
			// if don't recognize the tile type
			else
			{
				// stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			// move to next spot
			x += TILE_WIDTH;

			// if we've gone too far
			if (x >= SCREEN_WIDTH)
			{
				// move back
				x = 0;
				// move to the next row
				y += TILE_HEIGHT;
			}
		}
		if (tilesLoaded)
		{
			int xSheet = 0;
			int ySheet = 0;
			// clip the sprite sheet
			for (int i = 0; i < TOTAL_TILE_SPRITES; ++i)
			{
				gTileClips[i].x = xSheet;
				gTileClips[i].y = ySheet;
				gTileClips[i].w = TILE_WIDTH;
				gTileClips[i].h = TILE_HEIGHT;

				xSheet += TILE_WIDTH;
				if (xSheet >= TILE_WIDTH*5)
				{
					ySheet += TILE_HEIGHT;
					xSheet = 0;
				}
			}
			
		}
	}
	// close the file
	map.close();
	// if the map was loaded fine
	return tilesLoaded;
}

bool setTileClipsLevel2()
{
	bool tilesLoaded = true;
	// tile off set
	int x = 0, y = 64;
	// open the map
	std::ifstream map("res/level2.map");
	if (map.fail())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			// determines what tile will be made
			int tileType = -1;
			// read tile from map file
			map >> tileType;
			if (map.fail())
			{
				// stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}
			// if the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				int rotateAngle = 0;
				if (i >= 7 && i <= 9)
				{
					rotateAngle = 270;
				}
				else if (i == 32 || i == 64 || i == 96 || i == 128)
				{
					rotateAngle = 270;
				}
				
				tiles[i] = new Tile(x, y, tileType,rotateAngle);
				if (i == 43 || i == 59 || i == 75)
				{
					tiles[i]->showItem(ITEM_NO_SHOT);
				}
				if (i == 37 || i == 53 || i == 69)
				{
					tiles[i]->showItem(ITEM_OPENED_PILL_BOTTLE);
				}
			}
			// if don't recognize the tile type
			else
			{
				// stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			// move to next spot
			x += TILE_WIDTH;

			// if we've gone too far
			if (x >= SCREEN_WIDTH)
			{
				// move back
				x = 0;
				// move to the next row
				y += TILE_HEIGHT;
			}
		}
		if (tilesLoaded)
		{
			int xSheet = 0;
			int ySheet = 0;
			// clip the sprite sheet
			for (int i = 0; i < TOTAL_TILE_SPRITES; ++i)
			{
				gTileClips[i].x = xSheet;
				gTileClips[i].y = ySheet;
				gTileClips[i].w = TILE_WIDTH;
				gTileClips[i].h = TILE_HEIGHT;

				xSheet += TILE_WIDTH;
				if (xSheet >= TILE_WIDTH*5)
				{
					ySheet += TILE_HEIGHT;
					xSheet = 0;
				}
			}
			
		}
	}
	// close the file
	map.close();
	// if the map was loaded fine
	return tilesLoaded;
}

bool setTileClipsLevel3()
{
	bool tilesLoaded = true;
	// tile off set
	int x = 0, y = 64;
	// open the map
	std::ifstream map("res/level3.map");
	if (map.fail())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			// determines what tile will be made
			int tileType = -1;
			// read tile from map file
			map >> tileType;
			if (map.fail())
			{
				// stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}
			// if the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				int rotateAngle = 0;

				if (i == 32 || i == 48)
				{
					rotateAngle = 270;
				}
				else if (i >= 170 && i <= 172)
				{
					rotateAngle = 90;
				}
				
				tiles[i] = new Tile(x, y, tileType,rotateAngle);
				if (i == 39 || i == 55 || i == 71 )
				{
					tiles[i]->showItem(ITEM_NO_SHOT);
				}
				if (i == 103 || i == 119 || i == 135 )
				{
					tiles[i]->showItem(ITEM_OPENED_PILL_BOTTLE);
				}
				if (i >= 84 && i <= 86)
				{
					tiles[i]->showItem(ITEM_OPENED_CAPSULE_BOTTLE);
				}
			}
			// if don't recognize the tile type
			else
			{
				// stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			// move to next spot
			x += TILE_WIDTH;

			// if we've gone too far
			if (x >= SCREEN_WIDTH)
			{
				// move back
				x = 0;
				// move to the next row
				y += TILE_HEIGHT;
			}
		}
		if (tilesLoaded)
		{
			int xSheet = 0;
			int ySheet = 0;
			// clip the sprite sheet
			for (int i = 0; i < TOTAL_TILE_SPRITES; ++i)
			{
				gTileClips[i].x = xSheet;
				gTileClips[i].y = ySheet;
				gTileClips[i].w = TILE_WIDTH;
				gTileClips[i].h = TILE_HEIGHT;

				xSheet += TILE_WIDTH;
				if (xSheet >= TILE_WIDTH*5)
				{
					ySheet += TILE_HEIGHT;
					xSheet = 0;
				}
			}
			
		}
	}
	// close the file
	map.close();
	// if the map was loaded fine
	return tilesLoaded;
}

bool loadMedia()
{
    bool success = true;
	SDL_Surface* icon = IMG_Load("res/icon.png");
	if (icon == NULL)
	{
		printf( "Failed to load icon!: %s\n", IMG_GetError() );
		success = false;
	}
	else
	{
		SDL_SetWindowIcon(gWindow, icon);
	}
    //Open the font
    gFont = TTF_OpenFont("res/Aaargh.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
	}
	homeFont = TTF_OpenFont("res/OpenSans-Bold.ttf", 50 );
    if( homeFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
	titleFont = TTF_OpenFont("res/OpenSans-Bold.ttf", 100 );
    if( titleFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    // else
    // {
    //     //Render text
    //     SDL_Color textColor = { 0, 0, 0 };
    // }
    if (!gTileSheetTextures.loadFromFile("res/tiles.png"))
    {
        printf("Could not load maze texture\n");
        success = false;
    }
	else
	{
		if (!setTileClipsHome())
		{
			printf("Could not set tile clips\n");
			success = false;
		}
	}
	if (!gPlayerSheetTextures.loadFromFile("res/player.png"))
	{
		printf("Could not load player texture\n");
		success = false;
	}
	else
	{
		if (!setPlayerClips())
		{
			printf("Could not set player clips\n");
			success = false;
		}
	}
	if (!gItemSheetTextures.loadFromFile("res/items.png"))
	{
		printf("Could not load item texture\n");
		success = false;
	}
	else
	{
		if (!setItemClips())
		{
			printf("Could not set item clips\n");
			success = false;
		}
	}
	if (!level1Instructions.loadFromFile("res/level1Instructions.png"))
	{
		printf("Could not load instruction texture\n");
		success = false;
	}
	if (!level2Instructions.loadFromFile("res/level2Instructions.png"))
	{
		printf("Could not load instruction texture\n");
		success = false;
	}
	if (!level3Instructions.loadFromFile("res/level3Instructions.png"))
	{
		printf("Could not load instruction texture\n");
		success = false;
	}
    return success;
}

void setLevelHighScoreTiles()
{
	if (level1HighScore > 1)
	{
		homeTiles[84]->changeType(TILE_YELLOW_GEN);
	}
	if (level1HighScore > 2)
	{
		homeTiles[85]->changeType(TILE_YELLOW_GEN);
	}
	if (level1HighScore > 3)
	{
		homeTiles[100]->changeType(TILE_YELLOW_GEN);
	}
	if (level1HighScore > 4)
	{
		homeTiles[101]->changeType(TILE_YELLOW_GEN);
	}
	if (level2HighScore > 1)
	{
		homeTiles[87]->changeType(TILE_YELLOW_GEN);
	}
	if (level2HighScore > 2)
	{
		homeTiles[88]->changeType(TILE_YELLOW_GEN);
	}
	if (level2HighScore > 3)
	{
		homeTiles[103]->changeType(TILE_YELLOW_GEN);
	}
	if (level2HighScore > 4)
	{
		homeTiles[104]->changeType(TILE_YELLOW_GEN);
	}
	if (level3HighScore > 1)
	{
		homeTiles[90]->changeType(TILE_YELLOW_GEN);
	}
	if (level3HighScore > 2)
	{
		homeTiles[91]->changeType(TILE_YELLOW_GEN);
	}
	if (level3HighScore > 3)
	{
		homeTiles[106]->changeType(TILE_YELLOW_GEN);
	}
	if (level3HighScore > 4)
	{
		homeTiles[107]->changeType(TILE_YELLOW_GEN);
	}
}

void loadControllers()
{
	if (SDL_NumJoysticks() < 1)
	{
		printf("No controllers connected\n");
	}
	else
	{
		if (!SDL_IsGameController(0))
		{
			printf("Controller 1 not connected\n");
		}
		else
		{
			secondPlayerController = SDL_GameControllerOpen(0);
		}
		if (!SDL_IsGameController(1))
		{
			printf("Controller 2 not connected\n");
		}
		else
		{
			// open game controller
			thirdPlayerController = SDL_GameControllerOpen(1);
		}
		if (!SDL_IsGameController(2))
		{
			printf("Controller 3 not connected\n");
		}
		else
		{
			// open game controller
			fourthPlayerController = SDL_GameControllerOpen(2);
		}
		
	}
}

void close()
{
	// deallocate tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		if (tiles[i] != NULL)
		{
			delete tiles[i];
			tiles[i] = NULL;
		}
	}
	for (int i = 0; i < TOTAL_HOME_TILES; ++i)
	{
		if (homeTiles[i] != NULL)
		{
			delete homeTiles[i];
			homeTiles[i] = NULL;
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		if (shotTimers[i] != NULL)
		{
			delete shotTimers[i];
			shotTimers[i] = NULL;
		}
		if (pillTimers[i] != NULL)
		{
			delete pillTimers[i];
			pillTimers[i] = NULL;
		}
		if (capsuleTimers[i] != NULL)
		{
			delete capsuleTimers[i];
			capsuleTimers[i] = NULL;
		}
	}
	if (secondPlayerController != NULL)
	{
		SDL_GameControllerClose(secondPlayerController);
	}
	secondPlayerController = NULL;
	if (thirdPlayerController != NULL)
	{
		SDL_GameControllerClose(thirdPlayerController);
	}
	thirdPlayerController = NULL;
	if (fourthPlayerController != NULL)
	{
		SDL_GameControllerClose(fourthPlayerController);
	}
	fourthPlayerController = NULL;
	// free textures
	gPlayerSheetTextures.free();
	gItemSheetTextures.free();
    gTileSheetTextures.free();
	level1Instructions.free();
	level2Instructions.free();
	level3Instructions.free();
	orders.clear();
	gameTimeTexture.free();
	pointsTexture.free();
	orderList.clear();
	orderList2.clear();
	orderList3.clear();
    // free global font
    TTF_CloseFont(gFont);
    gFont = NULL;
	TTF_CloseFont(homeFont);
    homeFont = NULL;
	TTF_CloseFont(titleFont);
	titleFont = NULL;
    // destroy window and renderer
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    // close sdl and subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    Mix_Quit();

}