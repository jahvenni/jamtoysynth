#include <screen.h>

SDL_Surface *window;
int screen_width = 1024;
int screen_height = 600;

int screen_init()
{

  window = SDL_SetVideoMode(screen_width, screen_height, 32, SDL_OPENGL | SDL_DOUBLEBUF);
  if ( window == NULL ) {
    printf("Could not open window.\n");
    return -1;
  }
  


  glViewport(0, 0, screen_width, screen_height);
  glMatrixMode(GL_PROJECTION);	
  glLoadIdentity();
  glOrtho(0, (float)screen_width,  (float)screen_height, 0, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  SDL_WM_SetCaption("Jamtoysynth", NULL);

  text_init();
  return 0;
}

void screen_render()
{
  if (window == NULL)
    return;
  glClearColor(0.6f, 0.6f, 1.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);  
  glLoadIdentity();
  keys_render();
  pots_render();

  SDL_GL_SwapBuffers();
}

