#pragma once

#include "LPstore.h"
#include "Mode_home16.h"
#include "Mode_steps16.h"
#include "Mode_patts16.h"
#include "Mode_bank16.h"

class LPstate : public LPstore {

  public:
    LPstate(Sequencer* seq, uint width, uint height) : LPstore(seq, width, height) {

      // Load MODES
      modes[MODE_NONE] = new Mode_home16(this);
      modes[MODE_STEPS] = new Mode_steps16(this);
      modes[MODE_PATTS] = new Mode_patts16(this);
      modes[MODE_BANK] = new Mode_bank16(this);

    };

    bool setTrack(uint track, int patt) {
      bool ret = LPstore::setTrack(track, patt);
      mode()->onFocus();
      return ret;
    };

    void setMode(uint m) {
      LPstore::setMode(m);
      mode()->onFocus();
    }



  protected:


};
