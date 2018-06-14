CC=cc
C_FLAGS=-Wall -std=c99
SDL=SDL2
FRAMEWORKS=-framework SDL2
default: all
sdl_app.dylib: sdl_app.c memory.h
	$(CC) --shared sdl_app.c -o sdl_app.dylib $(C_FLAGS) $(FRAMEWORKS)

loader: loader.c memory.h
	$(CC) loader.c -o loader $(C_FLAGS) $(FRAMEWORKS)

all: sdl_app.dylib loader

clean:
	rm loader sdl_app.dylib
