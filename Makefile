CC = g++-4.9
SDL_INCLUDE = -I/usr/local/include -D_THREAD_SAFE
SDL_LIB = -L/usr/local/lib -lSDL2 -lGLEW -lSOIL

all:
	$(CC) main.cpp scene.cpp fbo.cpp helper.c -framework OpenGL -o main $(SDL_INCLUDE) $(SDL_LIB)
