#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <random>

#include <Constants.h>
#include <Timer.h>
#include <Texture.h>
#include <Tile.h>
#include <Order.h>
#include <Player.h>
#include <Utils.h>
#include <Globals.h>

int main(int argc, char* args[])
{
    // start up SDL
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            // main loop flag
            bool quit = false;
			// the dot that will be moving around the screen
			Player player1(200, 200);
			Player player2(776, 200);
			Player player3(200, 600);
			Player player4(776, 600);
            // event handler
            SDL_Event e;
			// points
			int points = 0;
			//Level camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			// time
			LTimer gameTime;
			LTimer addOrder;
			// in memory text stream
			std::stringstream gameTimeText;
			// 
			std::stringstream pointsStream;
			// rng for orders
			std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_int_distribution<int> distr(0, orderList.size() - 1);
			SDL_Color textColor = {0, 0, 0, 255};
			SDL_Color redColor = {0xFF, 0, 0, 255};

			for (int i = 0; i < 3; ++i)
			{
				shotTimers[i] = new LTimer();
			}
			for (int i = 0; i < 3; ++i)
			{
				pillTimers[i] = new LTimer();
			}
			for (int i = 0; i < 3; ++i)
			{
				capsuleTimers[i] = new LTimer();
			}
            while (!quit)
            {
                // handle events on queue
                while(SDL_PollEvent(&e) != 0)
                {
					if (e.type == SDL_JOYDEVICEADDED)
					{
						loadControllers();
					}
                    // user requests quit
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
					// handle input for dot
					if (screenNumber == SCREEN_TITLE)
					{
						if (e.type == SDL_MOUSEBUTTONDOWN)
						{
							SDL_Rect startButton = {SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2+78, 200, 100};
							// get mouse position
							int x, y;
							SDL_GetMouseState(&x, &y);
							if (x > startButton.x && x < startButton.x + startButton.w && y > startButton.y && y < startButton.y + startButton.w)
							{
								for (int i = 0; i < TOTAL_HOME_TILES; ++i)
								{
									if (homeTiles[i] != NULL)
									{
										delete homeTiles[i];
										homeTiles[i] = NULL;
									}
								}
								if (!setTileClipsLevelSelect())
								{
									printf("Could not set tile clips\n");
									quit = true;
								}
								setLevelHighScoreTiles();
								screenNumber = SCREEN_LEVELS;
							}
						}
					}
					else if (screenNumber == SCREEN_LEVELS)
					{
						if (e.type == SDL_MOUSEBUTTONDOWN)
						{
							SDL_Rect level1Button = {SCREEN_WIDTH/2-256, SCREEN_HEIGHT/2-64, 128, 128};
							SDL_Rect level2Button = {SCREEN_WIDTH/2-64, SCREEN_HEIGHT/2-64, 128, 128};
							SDL_Rect level3Button = {SCREEN_WIDTH/2+128, SCREEN_HEIGHT/2-64, 128, 128};
							SDL_Rect backButton = {SCREEN_WIDTH/2 - 64, SCREEN_HEIGHT-128, 128, 64};
							// get mouse position
							int x, y;
							SDL_GetMouseState(&x, &y);
							if (x > level1Button.x && x < level1Button.x + level1Button.w && y > level1Button.y && y < level1Button.y + level1Button.w)
							{
								for (int i = 0; i < TOTAL_TILES; ++i)
								{
									if (tiles[i] != NULL)
									{
										delete tiles[i];
										tiles[i] = NULL;
									}
								}
								if (!setTileClipsLevel1())
								{
									printf("Could not set tile clips\n");
									quit = true;
								}
								screenNumber = SCREEN_LEVEL_1_INSTRUCTIONS;
							}
							else if (x > level2Button.x && x < level2Button.x + level2Button.w && y > level2Button.y && y < level2Button.y + level2Button.w)
							{
								for (int i = 0; i < TOTAL_TILES; ++i)
								{
									if (tiles[i] != NULL)
									{
										delete tiles[i];
										tiles[i] = NULL;
									}
								}
								if (!setTileClipsLevel2())
								{
									printf("Could not set tile clips\n");
									quit = true;
								}
								screenNumber = SCREEN_LEVEL_2_INSTRUCTIONS;
							}
							else if (x > level3Button.x && x < level3Button.x + level3Button.w && y > level3Button.y && y < level3Button.y + level3Button.w)
							{
								for (int i = 0; i < TOTAL_TILES; ++i)
								{
									if (tiles[i] != NULL)
									{
										delete tiles[i];
										tiles[i] = NULL;
									}
								}
								if (!setTileClipsLevel3())
								{
									printf("Could not set tile clips\n");
									quit = true;
								}
								screenNumber = SCREEN_LEVEL_3_INSTRUCTIONS;
							}
							else if (x > backButton.x && x < backButton.x + backButton.w && y > backButton.y && y < backButton.y + backButton.w)
							{
								for (int i = 0; i < TOTAL_HOME_TILES; ++i)
								{
									if (tiles[i] != NULL)
									{
										delete tiles[i];
										tiles[i] = NULL;
									}
								}
								if (!setTileClipsHome())
								{
									printf("Could not set tile clips\n");
									quit = true;
								}
								screenNumber = SCREEN_TITLE;
							}
						}
					}
					else if (screenNumber == SCREEN_LEVEL_1_INSTRUCTIONS || screenNumber == SCREEN_LEVEL_2_INSTRUCTIONS || screenNumber == SCREEN_LEVEL_3_INSTRUCTIONS)
					{
						if (e.type == SDL_KEYDOWN)
						{
							if (e.key.keysym.sym == SDLK_RETURN)
							{
								player1.resetPlayer(200, 200);
								player2.resetPlayer(776, 200);
								player3.resetPlayer(200, 600);
								player4.resetPlayer(776, 600);
								points = 0;
								gameTime.start();
								addOrder.start();
								orders.push_back(new Order(ITEM_YELLOW_SHOT));
								screenNumber++;
							}
						}
					}
					else if (screenNumber == SCREEN_LEVEL_1 || screenNumber == SCREEN_LEVEL_2 || screenNumber == SCREEN_LEVEL_3)
					{
						if (e.type == SDL_KEYDOWN)
						{
							if (e.key.keysym.sym == SDLK_ESCAPE)
							{
								if (showPauseMenu == false && showEndMenu == false)
								{
									gameTime.pause();
									addOrder.pause();
									showPauseMenu = true;
								}
								else if (showPauseMenu == true)
								{
									gameTime.unpause();
									addOrder.unpause();
									showPauseMenu = false;
								}
							}
							if (showPauseMenu == true)
							{
								if (e.key.keysym.sym == SDLK_RETURN)
								{
									setLevelHighScoreTiles();
									gameTime.stop();
									addOrder.stop();
									orders.clear();
									screenNumber = SCREEN_LEVELS;
									showPauseMenu = false;
								}
							}
							if (showEndMenu == true)
							{
								if (e.key.keysym.sym == SDLK_RETURN)
								{
									setLevelHighScoreTiles();
									screenNumber = SCREEN_LEVELS;
									showEndMenu = false;
								}
							}
						}
						if (showPauseMenu == false && showEndMenu == false)
						{
							player1.handleEvent(e);
							if (e.jaxis.which == 0)
							{
								player2.handleControllerEvent(e);
							}
							else if (e.jaxis.which == 1)
							{
								player3.handleControllerEvent(e);
							}
							else if (e.jaxis.which == 2)
							{
								player4.handleControllerEvent(e);
							}
						}
						else
						{
							player1.removeVel();
							player2.removeVel();
							player3.removeVel();
							player4.removeVel();
						}
					}
                }
				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
				if (screenNumber == SCREEN_TITLE)
				{
					for (int i = 0; i < TOTAL_HOME_TILES; ++i)
					{
						homeTiles[i]->render(camera);
					}
					LTexture title;
					LTexture start;
					if (!title.loadFromRenderedText(titleFont, "UNDERDOSED", redColor))
					{
						printf("Could not load title text!\n");
					}
					if (!start.loadFromRenderedText(homeFont, "START", textColor))
					{
						printf("Could not load start text!\n");
					}
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
					SDL_Rect startButton = {SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2+78, 200, 100};

					title.render(SCREEN_WIDTH/2-title.getWidth()/2, 150);
					SDL_RenderFillRect(gRenderer, &startButton);
					start.render(SCREEN_WIDTH/2-start.getWidth()/2, SCREEN_HEIGHT/2-start.getHeight()/2+128);
				}
				else if (screenNumber == SCREEN_LEVELS)
				{
					LTexture title;
					LTexture level1;
					LTexture level2;
					LTexture level3;
					LTexture back;
					for (int i = 0; i < TOTAL_HOME_TILES; ++i)
					{
						homeTiles[i]->render(camera);
					}
					if (!title.loadFromRenderedText(homeFont, "LEVEL SELECT", textColor))
					{
						printf("Could not load title text!\n");
					}
					if (!level1.loadFromRenderedText(homeFont, "1", textColor))
					{
						printf("Could not load start text!\n");
					}
					if (!level2.loadFromRenderedText(homeFont, "2", textColor))
					{
						printf("Could not load start text!\n");
					}
					if (!level3.loadFromRenderedText(homeFont, "3", textColor))
					{
						printf("Could not load start text!\n");
					}
					if (!back.loadFromRenderedText(homeFont, "Back", textColor))
					{
						printf("Could not load start text!\n");
					}
					SDL_SetRenderDrawColor(gRenderer, 0x46, 0x46, 0x46, 0xFF);

					SDL_Rect backButton = {SCREEN_WIDTH/2 - 64, SCREEN_HEIGHT-128, 128, 64};

					title.render(SCREEN_WIDTH/2-title.getWidth()/2, 150);
					SDL_RenderFillRect(gRenderer, &backButton);
					level1.render(SCREEN_WIDTH/2-192-level1.getWidth()/2, SCREEN_HEIGHT/2-level1.getHeight()/2);
					level2.render(SCREEN_WIDTH/2-level2.getWidth()/2, SCREEN_HEIGHT/2-level2.getHeight()/2);
					level3.render(SCREEN_WIDTH/2+192-level3.getWidth()/2, SCREEN_HEIGHT/2-level3.getHeight()/2);
					back.render(SCREEN_WIDTH/2-back.getWidth()/2, SCREEN_HEIGHT-64-back.getHeight());
				}
				else if (screenNumber == SCREEN_LEVEL_1)
				{
					// move the dot
					player1.move();
					player1.setCamera(camera);
					player2.move();
					player2.setCamera(camera);
					player3.move();
					player3.setCamera(camera);
					player4.move();
					player4.setCamera(camera);
					// set text to be rendered
					if (gameTime.getTicks()/1000.f > 80)
					{
						for (int i = 0; i < 3; i++)
						{
							shotTimers[i]->stop();
						}
						orders.clear();
						addOrder.stop();
						gameTime.stop();
						if (level1HighScore < points)
						{
							level1HighScore = points;
						}
						showEndMenu = true;
					}
					if (addOrder.getTicks()/1000.f > 12 && gameTime.isStarted())
					{
						addOrder.stop();
						
						orders.push_back(new Order(orderList[distr(rng)]));
						addOrder.start();
					}
					gameTimeText.str("");
					gameTimeText << (std::round(80-gameTime.getTicks()/1000.f));
					if (!gameTimeTexture.loadFromRenderedText(gFont, gameTimeText.str().c_str(), textColor))
					{
						printf("Unable to render time texture!\n");
					}
					pointsStream.str("");
					pointsStream << points;
					if (!pointsTexture.loadFromRenderedText(gFont, pointsStream.str().c_str(), textColor))
					{
						printf("Unable to render point texture!\n");
					}

					for (int i = 0; i < orders.size(); ++i)
					{
						if (showPauseMenu == true)
						{
							orders[i]->time.pause();
						}
						else
						{
							orders[i]->time.unpause();
						}
						if (orders[i]->hasPoint())
						{
							points++;
							orders[i]->changePoint();
							orders.erase(orders.begin() + i);
							break;
						}
						if (orders[i]->time.getTicks()/1000.f > 30)
						{
							orders[i]->time.stop();
							orders[i]->setItemType(0);
							// points--;
							orders.erase(orders.begin());
						}
						orders[i]->render(i);
					}
			
					gameTimeTexture.render((SCREEN_WIDTH/2), (32- gameTimeTexture.getHeight()/2));
					pointsTexture.render((SCREEN_WIDTH-32-pointsTexture.getWidth()), (32- pointsTexture.getHeight()/2));

					// render level
					for (int i = 0; i < TOTAL_TILES; ++i)
					{
						tiles[i]->render(camera);
					}
				
					for (int i = 0; i < 3; ++i)
					{
						if (shotTimers[i]->getTicks()/1000.f > 5)
						{
							tiles[127]->changeType(tiles[127]->getType()+1);
							shotTimers[i]->stop();
						}
					}
					
					player1.render(camera, 0);
					player2.render(camera, 1);
					player3.render(camera, 2);
					player4.render(camera, 3);
				}

				else if (screenNumber == SCREEN_LEVEL_2)
				{
					// move the dot
					player1.move();
					player1.setCamera(camera);
					player2.move();
					player2.setCamera(camera);
					player3.move();
					player3.setCamera(camera);
					player4.move();
					player4.setCamera(camera);
					// set text to be rendered
					if (gameTime.getTicks()/1000.f > 100)
					{
						for (int i = 0; i < 3; i++)
						{
							shotTimers[i]->stop();
							pillTimers[i]->stop();
						}
						orders.clear();
						gameTime.stop();
						addOrder.stop();
						if (level2HighScore < points)
						{
							level2HighScore = points;
						}
						showEndMenu = true;
					}
					if (addOrder.getTicks()/1000.f > 12 && gameTime.isStarted())
					{
						addOrder.stop();
						
						orders.push_back(new Order(orderList[distr(rng)]));
						addOrder.start();
					}
					gameTimeText.str("");
					gameTimeText << (std::round(100-gameTime.getTicks()/1000.f));
					if (!gameTimeTexture.loadFromRenderedText(gFont, gameTimeText.str().c_str(), textColor))
					{
						printf("Unable to render time texture!\n");
					}
					pointsStream.str("");
					pointsStream << points;
					if (!pointsTexture.loadFromRenderedText(gFont, pointsStream.str().c_str(), textColor))
					{
						printf("Unable to render point texture!\n");
					}

					for (int i = 0; i < orders.size(); ++i)
					{
						if (showPauseMenu == true)
						{
							orders[i]->time.pause();
						}
						else
						{
							orders[i]->time.unpause();
						}
						if (orders[i]->hasPoint())
						{
							points++;
							orders[i]->changePoint();
							orders.erase(orders.begin() + i);
							break;
						}
						if (orders[i]->time.getTicks()/1000.f > 30)
						{
							orders[i]->time.stop();
							orders[i]->setItemType(0);
							// points--;
							orders.erase(orders.begin());
						}
						orders[i]->render(i);
					}
			
					gameTimeTexture.render((SCREEN_WIDTH/2), (32- gameTimeTexture.getHeight()/2));
					pointsTexture.render((SCREEN_WIDTH-32-pointsTexture.getWidth()), (32- pointsTexture.getHeight()/2));

					// render level
					for (int i = 0; i < TOTAL_TILES; ++i)
					{
						tiles[i]->render(camera);
					}
				
					for (int i = 0; i < 3; ++i)
					{
						if (showPauseMenu == true)
						{
							shotTimers[i]->pause();
							pillTimers[i]->pause();
						}
						else
						{
							shotTimers[i]->unpause();
							pillTimers[i]->unpause();
						}
						if (shotTimers[i]->getTicks()/1000.f > 5)
						{
							tiles[6]->changeType(tiles[6]->getType()+1);
							shotTimers[i]->stop();
						}
						if (pillTimers[i]->getTicks()/1000.f > 5)
						{
							if (tiles[10]->getType() == TILE_NO_SHOT)
							{
								tiles[10]->changeType(TILE_CLOSED_PILL_BOTTLE_ONE);
							}
							else if (tiles[10]->getType() >= TILE_CLOSED_PILL_BOTTLE_ONE || tiles[10]->getType() <= TILE_CLOSED_PILL_BOTTLE_FOUR)
							{
								tiles[10]->changeType(tiles[10]->getType()+1);
							}
							pillTimers[i]->stop();
						}
					}
					
					player1.render(camera, 0);
					player2.render(camera, 1);
					player3.render(camera, 2);
					player4.render(camera, 3);
				}
				else if (screenNumber == SCREEN_LEVEL_3)
				{
					// move the dot
					player1.move();
					player1.setCamera(camera);
					player2.move();
					player2.setCamera(camera);
					player3.move();
					player3.setCamera(camera);
					player4.move();
					player4.setCamera(camera);
					// set text to be rendered
					if (gameTime.getTicks()/1000.f > 120)
					{
						for (int i = 0; i < 3; i++)
						{
							shotTimers[i]->stop();
							pillTimers[i]->stop();
							capsuleTimers[i]->stop();
						}
						orders.clear();
						gameTime.stop();
						addOrder.stop();
						if (level3HighScore < points)
						{
							level3HighScore = points;
						}
						showEndMenu = true;
					}
					if (addOrder.getTicks()/1000.f > 12)
					{
						addOrder.stop();
						
						orders.push_back(new Order(orderList[distr(rng)]));
						addOrder.start();
					}
					gameTimeText.str("");
					gameTimeText << (std::round(120-gameTime.getTicks()/1000.f));
					if (!gameTimeTexture.loadFromRenderedText(gFont, gameTimeText.str().c_str(), textColor))
					{
						printf("Unable to render time texture!\n");
					}
					pointsStream.str("");
					pointsStream << points;
					if (!pointsTexture.loadFromRenderedText(gFont, pointsStream.str().c_str(), textColor))
					{
						printf("Unable to render point texture!\n");
					}

					for (int i = 0; i < orders.size(); ++i)
					{
						if (showPauseMenu == true)
						{
							orders[i]->time.pause();
						}
						else
						{
							orders[i]->time.unpause();
						}
						if (orders[i]->hasPoint())
						{
							points++;
							orders[i]->changePoint();
							orders.erase(orders.begin() + i);
							break;
						}
						if (orders[i]->time.getTicks()/1000.f > 30)
						{
							orders[i]->time.stop();
							orders[i]->setItemType(0);
							// points--;
							orders.erase(orders.begin());
						}
						orders[i]->render(i);
					}
			
					gameTimeTexture.render((SCREEN_WIDTH/2), (32- gameTimeTexture.getHeight()/2));
					pointsTexture.render((SCREEN_WIDTH-32-pointsTexture.getWidth()), (32- pointsTexture.getHeight()/2));

					// render level
					for (int i = 0; i < TOTAL_TILES; ++i)
					{
						tiles[i]->render(camera);
					}
				
					for (int i = 0; i < 3; ++i)
					{
						if (showPauseMenu == true)
						{
							shotTimers[i]->pause();
							pillTimers[i]->pause();
							capsuleTimers[i]->pause();
						}
						else
						{
							shotTimers[i]->unpause();
							pillTimers[i]->unpause();
							capsuleTimers[i]->unpause();
						}
						if (shotTimers[i]->getTicks()/1000.f > 5)
						{
							tiles[169]->changeType(tiles[169]->getType()+1);
							shotTimers[i]->stop();
						}
						if (pillTimers[i]->getTicks()/1000.f > 5)
						{
							if (tiles[173]->getType() == TILE_NO_SHOT)
							{
								tiles[173]->changeType(TILE_CLOSED_PILL_BOTTLE_ONE);
							}
							else if (tiles[173]->getType() >= TILE_CLOSED_PILL_BOTTLE_ONE || tiles[173]->getType() <= TILE_CLOSED_PILL_BOTTLE_FOUR)
							{
								tiles[173]->changeType(tiles[173]->getType()+1);
							}
							pillTimers[i]->stop();
						}
						if (capsuleTimers[i]->getTicks()/1000.f > 5)
						{
							if (tiles[174]->getType() == TILE_NO_SHOT)
							{
								tiles[174]->changeType(TILE_CLOSED_CAPSULE_BOTTLE_ONE);
							}
							else if (tiles[174]->getType() >= TILE_CLOSED_CAPSULE_BOTTLE_ONE || tiles[174]->getType() <= TILE_CLOSED_CAPSULE_BOTTLE_FOUR)
							{
								tiles[174]->changeType(tiles[174]->getType()+1);
							}
							capsuleTimers[i]->stop();
						}
					}
					
					player1.render(camera, 0);
					player2.render(camera, 1);
					player3.render(camera, 2);
					player4.render(camera, 3);
				}
				else if (screenNumber == SCREEN_LEVEL_1_INSTRUCTIONS)
				{
					level1Instructions.render(0, 0);
				}
				else if (screenNumber == SCREEN_LEVEL_2_INSTRUCTIONS)
				{
					level2Instructions.render(0, 0);
				}
				else if (screenNumber == SCREEN_LEVEL_3_INSTRUCTIONS)
				{
					level3Instructions.render(0, 0);
				}
				if (showPauseMenu == true)
				{
					SDL_Rect pauseMenu = {SCREEN_WIDTH/2-SCREEN_WIDTH/4, SCREEN_HEIGHT/2-SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
					SDL_SetRenderDrawColor(gRenderer, 0x46, 0x46, 0x46, 0xFF);
					SDL_RenderFillRect(gRenderer, &pauseMenu);
					LTexture pause;
					LTexture enter;
					LTexture returnBack;
					SDL_Color white = {0xFF, 0xFF, 0xFF};
					if (!pause.loadFromRenderedText(gFont, "PAUSED", white))
					{
						printf("Failed to load paused text!\n");
					}
					if (!enter.loadFromRenderedText(gFont, "Press Enter to go Home", white))
					{
						printf("Failed to load paused text!\n");
					}
					if (!returnBack.loadFromRenderedText(gFont, "Press Escape to Return", white))
					{
						printf("Failed to load paused text!\n");
					}
					pause.render(SCREEN_WIDTH/2-pause.getWidth()/2, SCREEN_HEIGHT/2-64);
					enter.render(SCREEN_WIDTH/2-enter.getWidth()/2, SCREEN_HEIGHT/2+64);
					returnBack.render(SCREEN_WIDTH/2-returnBack.getWidth()/2, SCREEN_HEIGHT/2+128);
				}
				if (showEndMenu == true)
				{
					SDL_Color white = {0xFF, 0xFF, 0xFF};
					SDL_Rect pauseMenu = {SCREEN_WIDTH/2-SCREEN_WIDTH/4, SCREEN_HEIGHT/2-SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
					SDL_SetRenderDrawColor(gRenderer, 0x46, 0x46, 0x46, 0xFF);
					SDL_RenderFillRect(gRenderer, &pauseMenu);
					LTexture pause;
					LTexture enter;
					LTexture endPoints;
					if (!endPoints.loadFromRenderedText(gFont, std::to_string(points) + " points", white))
					{
						printf("Failed to load paused text!\n");
					}
					if (!pause.loadFromRenderedText(gFont, "GAME OVER", white))
					{
						printf("Failed to load paused text!\n");
					}
					if (!enter.loadFromRenderedText(gFont, "Press Enter to go Home", white))
					{
						printf("Failed to load paused text!\n");
					}
					if (points > 1)
					{
						gTileSheetTextures.render(SCREEN_WIDTH/2-224, SCREEN_HEIGHT/2, &gTileClips[TILE_YELLOW_GEN]);
					}
					if (points > 2)
					{
						gTileSheetTextures.render(SCREEN_WIDTH/2-96, SCREEN_HEIGHT/2, &gTileClips[TILE_YELLOW_GEN]);
					}
					if (points > 3)
					{
						gTileSheetTextures.render(SCREEN_WIDTH/2+32, SCREEN_HEIGHT/2, &gTileClips[TILE_YELLOW_GEN]);
					}
					if (points > 4)
					{
						gTileSheetTextures.render(SCREEN_WIDTH/2+160, SCREEN_HEIGHT/2, &gTileClips[TILE_YELLOW_GEN]);
					}
					endPoints.render(SCREEN_WIDTH/2-endPoints.getWidth()/2, SCREEN_HEIGHT/2-64);
					pause.render(SCREEN_WIDTH/2-pause.getWidth()/2, SCREEN_HEIGHT/2-128);
					enter.render(SCREEN_WIDTH/2-enter.getWidth()/2, SCREEN_HEIGHT/2+128);
				}


				SDL_RenderPresent( gRenderer );
            }
        }

         // free resources and close sdl
         close();
    }
    return 0;
}