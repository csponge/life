life : main.o stage.o gui.o input.o init.o draw.o
	cc -lm -lSDL2 -lSDL2_image -lSDL2_ttf -o ./build/life ./build/main.o ./build/stage.o ./build/gui.o ./build/input.o ./build/init.o ./build/draw.o

main.o : ./src/main.c 
	cc -g -c ./src/main.c -o ./build/main.o

stage.o : ./src/stage.c
	cc -g -c ./src/stage.c -o ./build/stage.o

gui.o : ./src/gui.c
	cc -g -c ./src/gui.c -o ./build/gui.o

input.o : ./src/input.c
	cc -g -c ./src/input.c -o ./build/input.o

init.o : ./src/init.c
	cc -g -c ./src/init.c -o ./build/init.o

draw.o : ./src/draw.c
	cc -g -c ./src/draw.c -o ./build/draw.o

clean : 
	rm ./build/*
