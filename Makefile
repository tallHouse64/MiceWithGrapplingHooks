
# This is the compiler for the destination platform
CC := gcc

# This is the compiler that is used for running programs that are needed during compilation
HOSTCC := gcc

FLAGS := `sdl2-config --cflags --libs`

OBJ := main.o physics/physics.o maps/maps.o menus/menus.o

IMAGE_HEADERS := assets/mouse.h assets/font.h

# The name of the executable to generate
EXE := MWGH

.PHONY: clean web

all: $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(FLAGS)

web:
	$(MAKE) CC:=emcc EXE:=web/MWGH.js FLAGS:="-DWEB -sASYNCIFY"

main.o: main.c $(IMAGE_HEADERS)
	$(CC) main.c -c -o main.o $(FLAGS)

physics/physics.o: physics/physics.c
	$(CC) physics/physics.c -c -o physics/physics.o $(FLAGS)

maps/maps.o: maps/maps.c maps/maps.h main.h
	$(CC) maps/maps.c -c -o maps/maps.o $(FLAGS)

menus/menus.o: menus/menus.c menus/menus.h main.h
	$(CC) menus/menus.c -c -o menus/menus.o $(FLAGS)


#Convert mouse.png to an array
assets/mouse.h: assets/mouse.png assets/convert
	./assets/convert assets/mouse.png assets/mouse.h mouseData

assets/font.h: assets/font.png assets/convert
	./assets/convert assets/font.png assets/font.h fontData

assets/convert: assets/convert.c assets/stb_image.h
	$(HOSTCC) assets/convert.c -o assets/convert -lm

clean:
	-rm $(OBJ)
	-rm $(IMAGE_HEADERS)
	-rm web/MWGH.js web/MWGH.wasm
	-rm assets/convert
	-rm MWGH
