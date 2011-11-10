#include <touchlistener.h>

void TouchListener::addTuioObject(TUIO::TuioObject *tobj) {}
void TouchListener::updateTuioObject(TUIO::TuioObject *tobj) {}
void TouchListener::removeTuioObject(TUIO::TuioObject *tobj) {}
void TouchListener::addTuioCursor(TUIO::TuioCursor *tcur) 
{
  cursors.push_back(tcur);
}

void TouchListener::updateTuioCursor(TUIO::TuioCursor *tcur)
{
}

void TouchListener::removeTuioCursor(TUIO::TuioCursor *tcur) 
{
  cursors.remove(tcur);
}

void TouchListener::refresh(TUIO::TuioTime frameTime) 
{
  keys_release();
  for (std::list<TUIO::TuioCursor*>::iterator iter = cursors.begin(); iter != cursors.end(); iter++)
    {
      int key = get_key_at((*iter)->getX(), (*iter)->getY());
      keys_press(key);
    }
}
