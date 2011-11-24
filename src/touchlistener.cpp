#include <touchlistener.h>

void TouchListener::addTuioObject(TUIO::TuioObject *tobj) {}
void TouchListener::updateTuioObject(TUIO::TuioObject *tobj) {}
void TouchListener::removeTuioObject(TUIO::TuioObject *tobj) {}
void TouchListener::addTuioCursor(TUIO::TuioCursor *tcur) 
{
  cursors.push_back(tcur);
  pot_t* pot = pots_get(tcur->getX(), tcur->getY());
  if (pot != NULL) {
    active_pots[tcur] = pot;
  }
}

void TouchListener::updateTuioCursor(TUIO::TuioCursor *tcur)
{
  std::list<TUIO::TuioPoint>::reverse_iterator iter = tcur->getPath().rend();
  iter++;
  iter++;
  TUIO::TuioPoint last_pos = *iter;
  float dist = tcur->getPosition().getY() - last_pos.getY();

  for (std::map<TUIO::TuioCursor*, pot_t*>::iterator iter = active_pots.begin();
       iter != active_pots.end(); ++iter) {
    pots_add(iter->second, -dist * 2);
  }
}

void TouchListener::removeTuioCursor(TUIO::TuioCursor *tcur) 
{
  active_pots.erase(tcur);
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
