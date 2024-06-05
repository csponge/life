life : main.o stage.o input.o init.o draw.o
	cc -lSDL2 -lSDL2_image -lSDL2_ttf -o ./build/life ./build/main.o ./build/stage.o ./build/input.o ./build/init.o ./build/draw.o

main.o : ./src/main.c 
	cc -c ./src/main.c -o ./build/main.o

stage.o : ./src/stage.c
	cc -c ./src/stage.c -o ./build/stage.o

input.o : ./src/input.c
	cc -c ./src/input.c -o ./build/input.o

init.o : ./src/init.c
	cc -c ./src/init.c -o ./build/init.o

draw.o : ./src/draw.c
	cc -c ./src/draw.c -o ./build/draw.o

clean : 
	rm ./build/*
