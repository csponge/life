#ifndef GUI_H
#define GUI_H

typedef struct _Button Button;

typedef struct {
  float x;
  float y;
} Cord;

typedef struct {
  char *background;
  char *foreground;
} Style;

typedef struct {
  int w;
  int h;
} Rect;

typedef struct {

} ButtonClickedEvent;

struct _Button{
  Cord cordinates;
  Style style;
  Rect rect;
  void (*clicked)(ButtonClickedEvent *e);
  char *text;
};

typedef struct {
  Cord cordinates;
  Rect rect;
} Grid;

typedef struct {
  union e {
    Button button;
    Grid grid;
  };

} GuiElement;

#endif
