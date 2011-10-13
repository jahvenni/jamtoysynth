#include <touchlistener.hh>

void TouchListener::addTUIO::Object(TUIO::Object *tobj) {}
void TouchListener::updateTUIO::Object(TUIO::Object *tobj) {}
void TouchListener::removeTUIO::Object(TUIO::Object *tobj) {}

void TouchListener::addTUIO::Cursor(TUIO::Cursor *tcur) 
{
  std::cout<<"Cursor added at "<<tcur->getX()<<" "<<tcur->getY()<<std::endl;
  int key = get_key_at(tcur->getX(), tcur->getY());
  std::cout<<"Key "<<key<<" pressed"<<std::endl;
}
void TouchListener::updateTUIO::Cursor(TUIO::Cursor *tcur) {}
void TouchListener::removeTUIO::Cursor(TUIO::Cursor *tcur) {}

void TouchListener::refresh(TUIO::Time frameTime) {}
