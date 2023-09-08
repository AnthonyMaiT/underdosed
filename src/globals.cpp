#include "Globals.h"
#include "Texture.h"
#include "Constants.h"
#include "vector"
#include "Tile.h"
#include "Order.h"
#include "Timer.h"
#include "SDL2/SDL.h"

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;
TTF_Font* homeFont = NULL;
TTF_Font* titleFont = NULL;

LTexture level1Instructions;
LTexture level2Instructions;
LTexture level3Instructions;

// sheet textures
LTexture gTileSheetTextures;
// sprite clips
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];
// tiles
LTimer* shotTimers[3];
LTimer* pillTimers[3];
LTimer* capsuleTimers[3];
std::vector<Order*> orders;
// player texture
LTexture gPlayerSheetTextures;
// sprite clips
SDL_Rect gPlayerClips[TOTAL_ITEMS*4];
LTexture gItemSheetTextures;
SDL_Rect gItemClips[TOTAL_ITEMS];
LTexture gameTimeTexture;
LTexture pointsTexture;
Tile* tiles[TOTAL_TILES];
Tile* homeTiles[TOTAL_HOME_TILES];

bool showPauseMenu = false;

bool showEndMenu = false;

int level1HighScore = 0;
int level2HighScore = 0;
int level3HighScore = 0;

SDL_GameController* secondPlayerController = NULL;
SDL_GameController* thirdPlayerController = NULL;
SDL_GameController* fourthPlayerController = NULL;

int screenNumber = 0;

int bucketList[3] = {ITEM_BLUE_BUCKET, ITEM_RED_BUCKET, ITEM_YELLOW_BUCKET};

int shotList[6] = {ITEM_BLUE_SHOT, ITEM_RED_SHOT, ITEM_YELLOW_SHOT, ITEM_GREEN_SHOT, ITEM_PURPLE_SHOT, ITEM_ORANGE_SHOT};

int pillList[6] = {ITEM_BLUE_PILL_BOTTLE, ITEM_RED_PILL_BOTTLE, ITEM_YELLOW_PILL_BOTTLE, ITEM_GREEN_PILL_BOTTLE, ITEM_PURPLE_PILL_BOTTLE, ITEM_ORANGE_PILL_BOTTLE};

int capsuleList[6] = {ITEM_BLUE_CAPSULE_BOTTLE, ITEM_RED_CAPSULE_BOTTLE, ITEM_YELLOW_CAPSULE_BOTTLE, ITEM_GREEN_CAPSULE_BOTTLE, ITEM_PURPLE_CAPSULE_BOTTLE, ITEM_ORANGE_CAPSULE_BOTTLE};

std::vector<int> orderList{ITEM_RED_SHOT, ITEM_YELLOW_SHOT, ITEM_ORANGE_SHOT};
std::vector<int> orderList2{ITEM_BLUE_SHOT, ITEM_RED_SHOT, ITEM_YELLOW_SHOT, ITEM_GREEN_SHOT, ITEM_PURPLE_SHOT, ITEM_ORANGE_SHOT, ITEM_BLUE_PILL_BOTTLE, ITEM_RED_PILL_BOTTLE, ITEM_YELLOW_PILL_BOTTLE, ITEM_GREEN_PILL_BOTTLE, ITEM_PURPLE_PILL_BOTTLE, ITEM_ORANGE_PILL_BOTTLE};
std::vector<int> orderList3{ITEM_BLUE_SHOT, ITEM_RED_SHOT, ITEM_YELLOW_SHOT, ITEM_GREEN_SHOT, ITEM_PURPLE_SHOT, ITEM_ORANGE_SHOT, ITEM_BLUE_PILL_BOTTLE, ITEM_RED_PILL_BOTTLE, ITEM_YELLOW_PILL_BOTTLE, ITEM_GREEN_PILL_BOTTLE, ITEM_PURPLE_PILL_BOTTLE, ITEM_ORANGE_PILL_BOTTLE, ITEM_BLUE_CAPSULE_BOTTLE, ITEM_RED_CAPSULE_BOTTLE, ITEM_YELLOW_CAPSULE_BOTTLE, ITEM_GREEN_CAPSULE_BOTTLE, ITEM_PURPLE_CAPSULE_BOTTLE, ITEM_ORANGE_CAPSULE_BOTTLE};