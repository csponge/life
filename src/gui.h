#ifndef GUI_H
#define GUI_H

#include <stdbool.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
typedef struct _Button Button;

struct _Button {
    float x;
    float y;
	char *text;
    SDL_Texture *texture;
};

typedef struct {
	bool alive;
	float x;
	float y;
} Cell;

typedef struct {
	float x;
	float y;
	int rows;
	int cols;
	int cell_h;
	int cell_w;
	Cell ***cells;
} CellGrid;

typedef struct {
    SDL_Renderer *renderer;
    TTF_Font *font;
} DrawInfo;

typedef struct _guiElement{
    void *element;
    void (*draw)(DrawInfo *info, void *element);
    void (*free)(void *element);
} GuiElement;

/**
 * Creates a new `GuiElement` struct of type `Button`.
 * Returns NULL if failed.
 * */
GuiElement *new_button(float x, float y);

/**
 * Creates a new `GuiElement` struct of type `Grid`.
 * Returns NULL if failed.
 * */
GuiElement *new_cell_grid(int rows, int cols, float x, float y);

/** 
 * Copies the text to the button struct. Returns the number 
 * of bytes written. Returns -1 if failed.
 * */
int button_set_text(Button *btn, char *text, size_t len);

#endif
