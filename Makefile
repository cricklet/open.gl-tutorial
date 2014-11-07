SDL_INCLUDE = -I/usr/local/include/SDL -D_GNU_SOURCE=1 -D_THREAD_SAFE
SDL_LIB = -L/usr/local/lib -lSDLmain -lSDL -Wl,-framework,Cocoa

all:
	g++ main.cpp -framework OpenGL -o main $(SDL_INCLUDE) $(SDL_LIB)
