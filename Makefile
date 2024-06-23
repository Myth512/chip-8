build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -o ./bin/chip8 
run:
	./bin/chip8 'roms/IBM Logo.ch8'
clean:
	rm ./bin/*
