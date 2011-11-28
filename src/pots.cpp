#include "pots.h"

static float radius = 30;
static float padding = radius * 2;
static std::list<pot_t> pots;

void pots_init(instrument_control_t* control)
{
  float x = screen_width / 2 - 3 * (radius * 2 + padding);
  float y = screen_height / 4;
  pot_t pot1 = { &(control->carrier_amplitude), x, y, "amp" };
  pots.push_back(pot1);
  x += radius * 2 + padding;
  pot_t pot2 = { &(control->filter_freq), x, y, "freq" };
  pots.push_back(pot2);
  x += radius * 2 + padding;
  pot_t pot3 = { &(control->filter_resonance), x, y, "res" };
  pots.push_back(pot3);
  x += radius * 2 + padding;
  pot_t pot4 = { &(control->filter_gain), x, y, "gain" };
  pots.push_back(pot4);
  x += radius * 2 + padding;
  pot_t pot5 = { &(control->modulator_amplitude), x, y, "amp" };
  pots.push_back(pot5);
}

void pots_set(pot_t* pot, float amt)
{
  *(pot->value) = amt;
}

void pots_add(pot_t* pot, float amt)
{
  *(pot->value) += amt;
  if (*(pot->value) + amt <= 0.0) *(pot->value) = 0.0;
  else if (*(pot->value) + amt > 1.0) *(pot->value) = 1.0;
  else *(pot->value) += amt;
}

void pots_render()
{

  float x, y;
  glLoadIdentity();  
  glDisable( GL_TEXTURE_2D );
  glDisable( GL_BLEND);
  for (std::list<pot_t>::iterator iter = pots.begin(); iter != pots.end(); iter++) {
    assert(iter->value >= 0);
    x = iter->x;
    y = iter->y;
    glPushMatrix();
    glTranslatef(x, y, 0);
    glTranslatef(0, radius + 10, 0);
    text_render(iter->name);
    glTranslatef(0, -radius - 10, 0);
    glRotatef(-150 + *(iter->value) * 300, 0.0, 0, 1);
    glColor3f(0.5, 0.5, 0.2);
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_BLEND);
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

    glPopMatrix();
  }
}


pot_t* pots_get(float x, float y)
{
  x = x * screen_width;
  y = y * screen_height;
  for (std::list<pot_t>::iterator iter = pots.begin(); iter != pots.end(); ++iter) {
    float dx = x - iter->x;
    float dy = y - iter->y;
    if (dx*dx + dy*dy < radius*radius)
      return &(*iter);
  }
  return NULL;
}
