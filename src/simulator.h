#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <TUIO/TuioServer.h>
#include <TUIO/TuioCursor.h>
#include <list>
#include <stdio.h>


extern TUIO::TuioCursor *tuio_cursor;
extern TUIO::TuioServer *tuio_server;
extern TUIO::TuioTime current_time;

extern void sim_init();
extern void sim_quit();
extern void sim_mouse_pressed(float, float);
extern void sim_mouse_dragged(float, float);
extern void sim_mouse_released(float, float);
extern void sim_init_frame();
extern void sim_commit_frame();

#endif
