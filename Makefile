SDL_INCLUDE = -I/usr/local/include/SDL2 -D_THREAD_SAFE
SDL_LIB = -L/usr/local/lib -lSDL2

all:
	g++ main.cpp -framework OpenGL -o main $(SDL_INCLUDE) $(SDL_LIB)
