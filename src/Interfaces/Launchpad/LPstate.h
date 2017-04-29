#pragma once

#include "Modes.h"
#include "../Sequencer/Sequencer.h"
#include "../Sequencer/Track.h"

class LPstate {

public:
  LPstate(Sequencer* seq) {
    sequencer = seq;
    mode = MODE_DEFAULT;
    tracksel = 1;
  };

  Track* activetrack() {
    return sequencer->track(tracksel);
  };

  Sequencer*  sequencer;
  char        mode;
  char        tracksel;

};
