
CC := gcc

FLAGS := `sdl2-config --cflags --libs`

OBJ := main.o physics/physics.o maps/maps.o

IMAGE_HEADERS := assets/mouse.h

.PHONY: clean

all: $(OBJ)
	$(CC) $(OBJ) -o MWGH $(FLAGS)

main.o: main.c $(IMAGE_HEADERS)
	$(CC) main.c -c -o main.o $(FLAGS)

physics/physics.o: physics/physics.c
	$(CC) physics/physics.c -c -o physics/physics.o $(FLAGS)

maps/maps.o: maps/maps.c maps/maps.h main.h
	$(CC) maps/maps.c -c -o maps/maps.o $(FLAGS)


#Convert mouse.png to an array
assets/mouse.h: assets/mouse.png assets/convert
	./assets/convert assets/mouse.png assets/mouse.h mouseData

assets/convert: assets/convert.c assets/stb_image.h
	$(CC) assets/convert.c -o assets/convert -lm

clean:
	-rm $(OBJ)
	-rm $(IMAGE_HEADERS)
	-rm assets/convert
	-rm MWGH
