#pragma once

#include "Modes.h"
#include <list>

#include "../Sequencer/Sequencer.h"
#include "../Sequencer/Track.h"


class LPstate {

public:
  LPstate(Sequencer* seq) {
    sequencer = seq;
    currentmode = MODE_DEFAULT;
    tracksel = 1;
    height = 0;
    width = 0;
  };

  Track* activetrack() {
    return sequencer->track(tracksel);
  };

  void buttonRecord(char row, char btn, bool pushed) {
    pushedBtns[row][btn] = pushed;
    if (pushed) pushedStack[row].push_front(btn);
    else pushedStack[row].remove(btn);
  }

  char lastButton(char row) {
    if (pushedStack[row].empty()) return BTN_NONE;
    else return pushedStack[row].front();
  }

  Sequencer*  sequencer;
  uint        width;
  uint        height;

  char        currentmode;
  char        tracksel;

  char         pushedBtns[4][16];
  list<char>   pushedStack[4];

};
