
# This is the compiler for the destination platform
CC := gcc

# This is the compiler that is used for running programs that are needed during compilation
HOSTCC := gcc

FLAGS := `sdl2-config --cflags --libs`

OBJ := main.o physics/physics.o maps/maps.o menus/menus.o

IMAGE_HEADERS := assets/mouse.h assets/font.h

# The name of the executable to generate
EXE := MWGH

.PHONY: clean web nds

all: $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(FLAGS)

web:
	$(MAKE) CC:=emcc EXE:=web/MWGH.js FLAGS:="-DWEB -sASYNCIFY"

nds: $(IMAGE_HEADERS)
	-mkdir nds
	-mkdir nds/source
	-mkdir nds/source/physics
	-mkdir nds/source/assets
	-mkdir nds/source/maps
	-mkdir nds/source/menus
	-mkdir nds/source/assets
	# Copy all the source files to nds/source
	cp main.c nds/source/main.c
	cp main.h nds/source/main.h
	cp d.h nds/source/d.h
	cp devents.h nds/source/devents.h
	cp dplatform.h nds/source/dplatform.h
	cp beginCode.h nds/source/beginCode.h
	# Copy physics
	cp physics/physics.c nds/source/physics/physics.c
	cp physics/physics.h nds/source/physics/physics.h
	# Copy maps
	cp maps/maps.c nds/source/maps/maps.c
	cp maps/maps.h nds/source/maps/maps.h
	# Copy menus
	cp menus/menus.c nds/source/menus/menus.c
	cp menus/menus.h nds/source/menus/menus.h
	# Copy platform
	cp -R platform nds/source/
	# Copy assets
	cp assets/mouse.h nds/source/assets/mouse.h
	cp assets/icon.png nds/source/assets/icon.png
	cp assets/font.h nds/source/assets/font.h
	$(MAKE) --makefile=../ndsMakefile --directory=nds

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
	-rm -r nds
	-rm assets/convert
	-rm MWGH
