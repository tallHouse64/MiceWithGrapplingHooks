
CC := gcc

FLAGS := `sdl2-config --cflags --libs`

.PHONY: clean

TEST_EXECUTABLES := findEntryPointTest detectCollisionTest

findEntryPointTest: findEntryPointTest.c ../physics/physics.c
	$(CC) findEntryPointTest.c ../physics/physics.c -o findEntryPointTest $(FLAGS)

detectCollisionTest: detectCollisionTest.c ../physics/physics.c
	$(CC) detectCollisionTest.c ../physics/physics.c -o detectCollisionTest $(FLAGS)

lerpinttest: lerpinttest.c ../main.h
	$(CC) lerpinttest.c -o lerpinttest $(FLAGS)

clean:
	-rm $(TEST_EXECUTABLES)
