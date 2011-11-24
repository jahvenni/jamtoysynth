#ifndef POT_H
#define POT_H

#include <screen.h>
#include <list>
#include <cmath>

typedef struct pot_t {
  float *value;
} pot_t;

//Create new pot
extern void pots_init();
extern void pots_set(pot_t *pot, float amt);
extern void pots_render();
#endif
