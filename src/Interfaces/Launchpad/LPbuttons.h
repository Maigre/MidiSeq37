#pragma once
#include "Modes.h"
#include <list>




class LPbuttons {

  public:
    LPbuttons() {
      memset(pushedBtns, 0, sizeof(pushedBtns));
    }

    void record(uint row, uint btn, uint pushed) {
      pushedBtns[row][btn] = pushed;
      if (pushed) pushedStack[row].push_front(btn);
      else pushedStack[row].remove(btn);
    };

    uint active(uint row) {
      if (pushedStack[row].empty()) return BTN_NONE;
      else return pushedStack[row].front();
    };

    uint active(uint row, uint btn) {
      return pushedBtns[row][btn];
    }

  private:
    uint              pushedBtns[4][16];
    std::list<uint>   pushedStack[4];

};
