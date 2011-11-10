#ifndef CLIENT_HH
#define CLIENT_HH

#include <TUIO/TuioListener.h>
#include <TUIO/TuioClient.h>

#include <screen.h>
#include <iostream>
#include <list>

class TouchListener : public TUIO::TuioListener
{
 private:
  std::list<TUIO::TuioCursor*> cursors;
 public:
  void addTuioObject(TUIO::TuioObject *tobj);
  void updateTuioObject(TUIO::TuioObject *tobj);
  void removeTuioObject(TUIO::TuioObject *tobj);
  
  void addTuioCursor(TUIO::TuioCursor *tcur);
  void updateTuioCursor(TUIO::TuioCursor *tcur);
  void removeTuioCursor(TUIO::TuioCursor *tcur);
  
  void refresh(TUIO::TuioTime frameTime);
};

#endif
