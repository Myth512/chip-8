build:
	gcc -Wall -std=c99 ./src/*.c -o ./bin/chip8 -lSDL2
run:
	./bin/chip8
clean:
	rm ./bin/*
