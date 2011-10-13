#ifndef SCREEN_H
#define SCREEN_H

#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <keys.h>

extern SDL_Surface *window;

extern int screen_width;
extern int screen_height;
extern int screen_init();
extern void screen_render();
#endif
