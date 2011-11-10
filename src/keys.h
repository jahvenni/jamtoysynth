#ifndef KEYS_H
#define KEYS_H

#include <screen.h>
#include <synth.h>
#include <audio_output.h>

extern void keys_init(synth_t*);
extern void keys_render();
extern int get_key_at(float,float);
extern void keys_press(unsigned int);
extern void keys_release();
#endif
