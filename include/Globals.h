#ifndef GLOBALS_H
#define GLOBALS_H

#include "Texture.h"
#include "Constants.h"
#include "vector"
#include "Tile.h"
#include "Order.h"
#include "Timer.h"
#include "SDL2/SDL.h"

//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;

extern TTF_Font* gFont;
extern TTF_Font* homeFont;
extern TTF_Font* titleFont;

extern LTexture level1Instructions;
extern LTexture level2Instructions;
extern LTexture level3Instructions;

// sheet textures
extern LTexture gTileSheetTextures;
// sprite clips
extern SDL_Rect gTileClips[TOTAL_TILE_SPRITES];
// tiles
extern LTimer* shotTimers[3];
extern LTimer* pillTimers[3];
extern LTimer* capsuleTimers[3];
extern std::vector<Order*> orders;
// player texture
extern LTexture gPlayerSheetTextures;
// sprite clips
extern SDL_Rect gPlayerClips[TOTAL_ITEMS*4];
extern LTexture gItemSheetTextures;
extern SDL_Rect gItemClips[TOTAL_ITEMS];
extern LTexture gameTimeTexture;
extern LTexture pointsTexture;
extern Tile* tiles[TOTAL_TILES];
extern Tile* homeTiles[TOTAL_HOME_TILES];

extern SDL_GameController* secondPlayerController;
extern SDL_GameController* thirdPlayerController;
extern SDL_GameController* fourthPlayerController;

extern int screenNumber;

extern int bucketList[3];
extern int shotList[6];
extern int pillList[6];
extern int capsuleList[6];

extern bool showPauseMenu;
extern bool showEndMenu;

extern int level1HighScore;
extern int level2HighScore;
extern int level3HighScore;

extern std::vector<int> orderList;
extern std::vector<int> orderList2;
extern std::vector<int> orderList3;

#endif 

