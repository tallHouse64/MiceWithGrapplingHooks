
CC := gcc

FLAGS := `sdl2-config --cflags --libs`

OBJ := main.o physics/physics.o

.PHONY: clean

all: $(OBJ)
	$(CC) $(OBJ) -o MWGH $(FLAGS)

main.o: main.c
	$(CC) main.c -c -o main.o $(FLAGS)

physics/physics.o: physics/physics.c
	$(CC) physics/physics.c -c -o physics/physics.o $(FLAGS)

clean:
	-rm $(OBJ)
	-rm MWGH
