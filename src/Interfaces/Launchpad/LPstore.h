#pragma once
#include "LPbuttons.h"
#include "../../Sequencer/Sequencer.h"
#include "../../Sequencer/Track.h"

class Mode_abstract;

class LPstore {

  public:
    LPstore(Sequencer* seq, uint w, uint h) {
      buttons = new LPbuttons();
      sequencer = seq;
      width = w;
      height = h;

      selectedmode = MODE_NONE;
      previousmode = MODE_NONE;
      selectedtrack = 1;
      selectedpatt = (track() != NULL) ? track()->activePatternIndex() : 0;

    }

    Clock* baseclock() {
      return sequencer->track(0)->activePattern()->clock();
    };

    Track* track() {
      return sequencer->track(selectedtrack);
    };

    Pattern* pattern() {
      if (track() != NULL)
        return track()->pattern(selectedpatt);
      return NULL;
    };

    virtual bool setTrack(uint track, int patt) {
      if (sequencer->track(track) == NULL) return false;
      if (sequencer->track(track)->pattern(patt) == NULL) return false;
      selectedtrack = track;
      selectedpatt = patt;
      return true;
    };

    uint getTrack() {
      return selectedtrack;
    };

    uint getPatt() {
      return selectedpatt;
    };

    Mode_abstract* mode() {
      return modes[selectedmode];
    };

    void prevMode() {
      if (previousmode == MODE_NONE) return;
      setMode(previousmode);
      previousmode = MODE_NONE;
    }

    virtual void setMode(uint m) {
      previousmode = selectedmode;
      selectedmode = m;
    }

    uint getMode() {
      return selectedmode;
    };


    Sequencer*  sequencer;
    uint        width;
    uint        height;

    LPbuttons*  buttons;

  protected:
    Mode_abstract*  modes[16];

    uint        previousmode;
    uint        selectedmode;
    uint        selectedtrack;
    int         selectedpatt;


};
