#ifndef POT_H
#define POT_H

#include <screen.h>
#include <list>
#include <cmath>
#include <instrument.h>
#include <assert.h>

typedef struct pot_t {
  float *value;
  float x, y;
  char *name;
} pot_t;

//Create new pot
extern void pots_init(instrument_control_t* control);
extern void pots_set(pot_t *pot, float amt);
extern void pots_add(pot_t *pot, float amt);
extern void pots_render();
extern pot_t* pots_get(float x, float y);
#endif
