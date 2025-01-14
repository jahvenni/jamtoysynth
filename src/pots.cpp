#include "pots.h"

static float radius = 0.04;
static float padding = radius * 0.8;

typedef struct potgroup_t {
  std::list<pot_t> pots;
  float x, y;
  char* name;
} potgroup_t;

static std::list<potgroup_t> groups;

void pots_init(instrument_control_t* control)
{
  {
    std::list<pot_t> pots;
    pot_t pot1 = { &(control->carrier_amplitude), 0, 3 * radius, (char*)"amp" };
    pots.push_back(pot1);
    potgroup_t group = { pots, 0.1, 0.1, (char*)"general" };
    groups.push_back(group);
  }
  
  {
    std::list<pot_t> pots;
    pot_t pot1 = { &(control->filter_freq), -radius * 2 - padding, 3 * radius, (char*)"freq" };
    pot_t pot2 = { &(control->filter_resonance), 0, 3 * radius, (char*)"res" };
    pot_t pot3 = { &(control->filter_gain), radius * 2 + padding, 3 * radius, (char*)"gain" };
    pots.push_back(pot1);
    pots.push_back(pot2);
    pots.push_back(pot3);
    potgroup_t group = { pots, 0.5, 0.1, (char*)"filter" };
    groups.push_back(group);
  }

  {
    std::list<pot_t> pots;
    pot_t pot1 = { &(control->modulator_amplitude), -radius - padding / 2, 3 * radius, (char*)"amp" };
    pot_t pot2 = { &(control->modulator_freq), radius + padding / 2, 3 * radius, (char*)"freq" };
    pots.push_back(pot1);
    pots.push_back(pot2);
    potgroup_t group = { pots, 0.5, 0.4, (char*)"modulator" };
    groups.push_back(group);
  }
  
}

void pots_set(pot_t* pot, float amt)
{
  *(pot->value) = amt;
}

void pots_add(pot_t* pot, float amt)
{
  if (*(pot->value) + amt <= 0.0) *(pot->value) = 0.0;
  else if (*(pot->value) + amt > 1.0) *(pot->value) = 1.0;
  else *(pot->value) += amt;
}

void pots_render()
{
  float r = radius * screen_width;
  glLoadIdentity();  
  glDisable( GL_TEXTURE_2D );
  glDisable( GL_BLEND);
  for (std::list<potgroup_t>::iterator group_iter = groups.begin(); group_iter != groups.end(); group_iter++) {
    potgroup_t *group = &(*group_iter);
    glPushMatrix();
    glTranslatef((int)(group->x * screen_width), (int)(group->y * screen_height), 0);
    text_render(group->name);
    for (std::list<pot_t>::iterator iter = group->pots.begin(); iter != group->pots.end(); iter++) {
      glPushMatrix();
      glTranslatef((int)(iter->x * screen_width), (int)(iter->y * screen_height), 0);
      glTranslatef(0, r + 10, 0);
      text_render(iter->name);
      glTranslatef(0, -r - 10, 0);
      glRotatef(-150 + *(iter->value) * 300, 0.0, 0, 1);
      glColor3f(0.5, 0.5, 0.2);
      glDisable( GL_TEXTURE_2D );
      glDisable( GL_BLEND);
      glBegin(GL_TRIANGLE_FAN);
      glVertex2f(0, 0);
      for (float angle = 0; angle - 0.05 < 2 * M_PI; angle += 0.05) {
	glVertex2f(sin(angle) * r, cos(angle) * r);
      }
      glEnd();
      glColor3f(0.1, 0.1, 0);
      glBegin(GL_QUADS);
      glVertex2f(-r / 10, 0);
      glVertex2f( r / 10, 0);
      glVertex2f( r / 10, -r);
      glVertex2f(-r / 10, -r);
      glEnd();

      glPopMatrix();
    }
    glPopMatrix();
  }
}


pot_t* pots_get(float x, float y)
{
  for (std::list<potgroup_t>::iterator group_iter = groups.begin(); group_iter != groups.end(); group_iter++) {
    potgroup_t *group = &(*group_iter);;
    std::list<pot_t> *pots = &(group->pots);
    for (std::list<pot_t>::iterator iter = pots->begin(); iter != pots->end(); ++iter) {
      //The y distance has to be scaled to make the world a square
      float dx = (x - (group->x + iter->x));
      float dy = (y - (group->y + iter->y)) * (screen_height / (float)screen_width);
      if (dx*dx + dy*dy < radius*radius) {
	return &(*iter);
      }
    }
  }
  return NULL;
}
