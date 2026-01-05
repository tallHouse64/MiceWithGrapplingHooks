
CC := gcc

FLAGS := `sdl2-config --cflags --libs`

findEntryPointTest: findEntryPointTest.c ../physics/physics.c
	$(CC) findEntryPointTest.c ../physics/physics.c -o findEntryPointTest $(FLAGS)

clean:
	-rm findEntryPointTest
	-rm findEntryPointTest.exe
