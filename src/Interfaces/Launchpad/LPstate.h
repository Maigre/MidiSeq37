#pragma once

#include "Modes.h"
#include <list>

#include "../../Sequencer/Sequencer.h"
#include "../../Sequencer/Track.h"


class LPstate {

  public:
    LPstate(Sequencer* seq) {
      sequencer = seq;
      currentmode = MODE_DEFAULT;
      selectedtrack = 1;
      selectedpatt = selectedTrack()->activePatternIndex();
      height = 0;
      width = 0;
    };

    Track* selectedTrack() {
      return sequencer->track(selectedtrack);
    };

    Pattern* selectedPattern() {
      return selectedTrack()->pattern(selectedpatt);
    };

    void select(uint track, int patt) {
      selectedtrack = track;
      selectedpatt = patt;
    };

    uint trackSel() {
      return selectedtrack;
    }

    uint pattSel() {
      return selectedpatt;
    }

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

  private:
    uint        selectedtrack;
    int         selectedpatt;

    uint         pushedBtns[4][16];
    list<uint>   pushedStack[4];

};
