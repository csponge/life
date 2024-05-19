#include "defs.h"
#include "draw.h"
#include "structs.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <stdlib.h>

static void fire_bullet(Stage *stage, Entity *player) {
  SDL_Texture *temp = stage->bulletTail->texture;
  
  Entity *bullet = calloc(1, sizeof(Entity));
  stage->bulletTail->next = bullet;
  stage->bulletTail = bullet;

  bullet->x = player->x;
  bullet->y = player->y;
  bullet->dx = BULLET_SPEED;
  bullet->health = 1;
  bullet->texture = temp;

  bullet->y += (player->h / 2) - (bullet->h / 2);
  player->reload = 8;
}

static void do_player(App *app, Stage *stage, Entity *player,
                      SDL_Texture *bullet_texture) {
  player->dx = player->dy = 0;

  if (player->reload > 0) {
    player->reload--;
  }

  if (app->keyboard[SDL_SCANCODE_UP]) {
    player->dy = player->y - PLAYER_SPEED >= 0 ? -PLAYER_SPEED : -player->y;
  }

  if (app->keyboard[SDL_SCANCODE_DOWN]) {
    player->dy = player->y + player->h + PLAYER_SPEED <= SCREEN_HEIGHT
                     ? PLAYER_SPEED
                     : SCREEN_HEIGHT - (player->y + player->h);
  }

  if (app->keyboard[SDL_SCANCODE_LEFT]) {
    player->dx = player->x - PLAYER_SPEED >= 0 ? -PLAYER_SPEED : -player->x;
  }

  if (app->keyboard[SDL_SCANCODE_RIGHT]) {
    player->dx = player->x + player->w + PLAYER_SPEED <= SCREEN_WIDTH
      ? PLAYER_SPEED
      : SCREEN_WIDTH - (player->x + player->w);
  }

  if (app->keyboard[SDL_SCANCODE_F] && player->reload == 0) {
    fire_bullet(stage, player);
  }

  player->x += player->dx;
  player->y += player->dy;
}

static void do_bullets(Stage *stage) {
  Entity *b, *prev;

  prev = &stage->bulletHead;

  for (b = stage->bulletHead.next; b != NULL; b = b->next) {
    b->x += b->dx;
    b->y += b->dy;

    if (b->x > SCREEN_WIDTH) {
      if (b == stage->bulletTail) {
        stage->bulletTail = prev;
      }

      prev->next = b->next;
      free(b);
      b = prev;
    }

    prev = b;
  }
}

static void logic(App *app, Stage *stage, Entity *player,
                  SDL_Texture *bullet_texture) {
  do_player(app, stage, player, bullet_texture);

  do_bullets(stage);
}

static void draw_player(App *app, Entity *player) {
  blit(app, player->texture, player->x, player->y);
}

static void draw_bullets(App *app, Stage *stage) {
  Entity *b;

  for (b = stage->bulletHead.next; b != NULL; b = b->next) {
    blit(app, b->texture, b->x, b->y);
  }
}

static void draw(App *app, Stage *stage, Entity *player) {
  draw_player(app, player);
  draw_bullets(app, stage);
}

Stage *init_stage(App *app, Entity *player) {
  app->delegate.logic = logic;
  app->delegate.draw = draw;

  Stage *stage = calloc(1, sizeof(Stage));
  stage->fighterTail = &stage->fighterHead;
  stage->bulletTail = &stage->bulletHead;

  // loading player
  stage->fighterTail->next = player;
  stage->fighterTail = player;
  player->x = 100;
  player->y = 100;
  player->texture = load_texture(app, "gfx/little_buddy.png");
  SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);

  // loading bullet gfx
  stage->bulletHead.texture = load_texture(app, "gfx/bullet.png");

  return stage;
}
