#include <touchlistener.h>

void TouchListener::addTuioObject(TUIO::TuioObject *tobj) {}
void TouchListener::updateTuioObject(TUIO::TuioObject *tobj) {}
void TouchListener::removeTuioObject(TUIO::TuioObject *tobj) {}

void TouchListener::addTuioCursor(TUIO::TuioCursor *tcur) 
{
  std::cout<<"Cursor added at "<<tcur->getX()<<" "<<tcur->getY()<<std::endl;
  int key = get_key_at(tcur->getX(), tcur->getY());
  if (key >= 0)
    std::cout<<"Key "<<key<<" pressed"<<std::endl;
}
void TouchListener::updateTuioCursor(TUIO::TuioCursor *tcur) {}
void TouchListener::removeTuioCursor(TUIO::TuioCursor *tcur) {}

void TouchListener::refresh(TUIO::TuioTime frameTime) {}
