#include "pots.h"

static float radius = 30;
static float padding = radius;
static std::list<pot_t> pots;

void pots_init(instrument_t* instrument)
{
  pot_t pot1 = { NULL };
  pots.push_back(pot1);
  pot_t pot2 = { NULL };
  pots.push_back(pot2);
  pot_t pot3 = { NULL };
  pots.push_back(pot3);
}

void pots_set(pot_t* pot, float amt)
{
  *(pot->value) = amt;
}

void pots_render()
{
  glLoadIdentity();

  float x = screen_width / 2 - pots.size() * (radius * 2 + padding);
  float y = screen_height / 4;
  
  for (std::list<pot_t>::iterator iter = pots.begin(); iter != pots.end(); iter++) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(-150 + 0 * 300, 0.0, 0, 1);
    glColor3f(0.5, 0.5, 0.2);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    for (float angle = 0; angle - 0.05 < 2 * M_PI; angle += 0.05) {
      glVertex2f(sin(angle) * radius, cos(angle) * radius);
    }
    glEnd();
    glColor3f(0.1, 0.1, 0);
    glBegin(GL_QUADS);
    glVertex2f(-radius / 10, 0);
    glVertex2f( radius / 10, 0);
    glVertex2f( radius / 10, -radius);
    glVertex2f(-radius / 10, -radius);
    glEnd();
    x += radius * 2 + padding;
    glPopMatrix();
  }
}
