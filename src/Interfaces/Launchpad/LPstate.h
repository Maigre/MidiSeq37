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

  void buttonRecord(uint row, uint btn, uint pushed) {
    pushedBtns[row][btn] = pushed;
    if (pushed) pushedStack[row].push_front(btn);
    else pushedStack[row].remove(btn);
  }

  uint lastButton(uint row) {
    if (pushedStack[row].empty()) return BTN_NONE;
    else return pushedStack[row].front();
  }

  Sequencer*  sequencer;
  uint        width;
  uint        height;

  uint        currentmode;
  uint        tracksel;

  uint         pushedBtns[4][16];
  list<uint>   pushedStack[4];

};
