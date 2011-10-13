#include "simulator.h"

TuioCursor *tuio_cursor = NULL;
TuioServer *tuio_server = NULL;
TuioTime current_time;

void sim_init()
{
  tuio_server = new TuioServer();
  current_time = TuioTime::getSessionTime();
}
void sim_quit()
{
  delete tuio_server;
  delete tuio_cursor;
}

void sim_init_frame()
{
  if (tuio_server != NULL)
    {
      current_time = TuioTime::getSessionTime();
      tuio_server->initFrame(current_time);
    }
}
void sim_commit_frame()
{
  if (tuio_server != NULL)
    {
      tuio_server->stopUntouchedMovingCursors();
      tuio_server->commitFrame();
    }
}

void sim_mouse_pressed(float x, float y) 
{
  tuio_cursor = tuio_server->addTuioCursor(x,y);
}

void sim_mouse_dragged(float x, float y) 
{
  if (tuio_cursor==NULL) return;
  //	if (cursor->getTuioTime()==currentTime) return;
  tuio_server->updateTuioCursor(tuio_cursor,x,y);
}

void sim_mouse_released(float x, float y) {
  if (tuio_cursor==NULL) return;
  tuio_server->removeTuioCursor(tuio_cursor);
  tuio_cursor = NULL;
}

