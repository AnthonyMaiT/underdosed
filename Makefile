all:
	g++ -c src/*.cpp -std=c++14 -g -Wall -m64 -I include && g++ *.o -o main -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer