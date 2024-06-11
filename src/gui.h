#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
typedef struct _Button Button;

struct _Button {
    float x;
    float y;
    int w;
    int h;
	char *text;
    SDL_Texture *texture;
};

typedef struct {
    SDL_Renderer *renderer;
    TTF_Font *font;
} DrawInfo;

typedef struct _guiElement{
    void *element;
    void (*draw)(DrawInfo *info, void *element);
} GuiElement;

/**
 * Creates a new `GuiElement` struct of type `Button`.
 * Returns NULL if failed.
 * */
GuiElement *new_button(float x, float y, int w, int h);

/** 
 * Copies the text to the button struct. Returns the number 
 * of bytes written. Returns -1 if failed.
 * */
int button_set_text(Button *btn, char *text, size_t len);

/**
 * Destroy the button 
 * */
void destroy_button(Button *btn);

#endif
