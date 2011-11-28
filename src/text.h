#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>
#include <SDL.h>
#include <GL/gl.h>

#ifdef __cplusplus
extern "C" {
#endif
  
  void text_init();
  void text_render(char* text);


#ifdef __cplusplus
}
#endif

#endif
