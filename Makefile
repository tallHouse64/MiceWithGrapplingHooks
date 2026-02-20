
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
	# Copy all the source files to nds/source
	cp main.c nds/source/main.c
	cp main.h nds/source/main.h
	cp d.h nds/source/d.h
	cp devents.h nds/source/devents.h
	cp dplatform.h nds/source/dplatform.h
	cp beginCode.h nds/source/beginCode.h
	cp -R physics nds/source/
	cp -R maps nds/source/
	cp -R menus nds/source/
	cp -R platform nds/source/
	cp specialNdsMakefile nds/Makefile
	# Handle the assets folder seperately
	-mkdir nds/source/assets
	cp assets/mouse.h nds/source/assets/mouse.h
	cp assets/font.h nds/source/assets/font.h
	$(MAKE) -C nds
	# Rename the ROM (DS executable)
	mv nds/nds.nds nds/MWGH.nds

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
