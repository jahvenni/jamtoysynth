#include <text.h>

static GLuint text_texture;

void text_init()
{
  SDL_Surface *surface = SDL_LoadBMP("src/font.bmp");
  glGenTextures(1, &text_texture);
  glBindTexture(GL_TEXTURE_2D, text_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
  SDL_FreeSurface(surface);
}

void text_render(char* str)
{
  glEnable( GL_TEXTURE_2D );
  glEnable( GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindTexture(GL_TEXTURE_2D, text_texture);

  glPushMatrix();
  int i = 0;
  while (str[i++] != '\0') {
    glTranslatef(-8, 0, 0);
  }
  glBegin(GL_QUADS);

  i = 0;
  while (str[i] != '\0') {
    int letter = *(str + i) - 97;
    if (letter < 0 || letter > 25) {
      i++;
      continue;
    }
    glTexCoord2f(1.0/26.0*letter, 0);
    glVertex2f(i * 16, 0);
    glTexCoord2f(1.0/26.0*(letter+1), 0);
    glVertex2f((i + 1) * 16, 0);
    glTexCoord2f(1.0/26.0*(letter+1), 1);
    glVertex2f((i + 1) * 16, 14);
    glTexCoord2f(1.0/26.0*letter, 1);
    glVertex2f(i * 16, 14);
    i++;
  }
  glEnd();
  glPopMatrix();
}
