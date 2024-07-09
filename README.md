# Game Of Life

## Description
This is an implementation of [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) in C using SDL2.
The game includes an interactive grid, where players can select individual cells to become *alive*. When the **Play** button is
clicked, the rules of the game of life are applied to the initial seed provided by the player. This runs indefinitely, or until
the player stops the game. 

## Dependencies
The following dependencies are required for building the project:
* GNU Make
* SDL2
* SDL2_image
* SDL2_ttf

## Building
Make sure to install all necessary [dependencies](#dependencies). Then, run the following command `make && ./build/life` to build and run the game.
