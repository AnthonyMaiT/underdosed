#ifndef UTIL_H
#define UTIL_H

// starts up sdl and other apps 
bool init();

bool setTileClipsHome();

bool setTileClipsLevelSelect();

// set tile clips
bool setTileClipsLevel1();

bool setTileClipsLevel2();

bool setTileClipsLevel3();

bool setPlayerClips();

bool setItemClips();

// loads media
bool loadMedia();

void setLevelHighScoreTiles();

void loadControllers();

// frees media and shuts down SDL
void close();

#endif 