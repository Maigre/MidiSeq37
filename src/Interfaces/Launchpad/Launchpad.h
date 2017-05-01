#include "LPad.h"
#include "LPstate.h"
#include "Mode_base.h"

/*
LAUNCHPAD ARRAY class
*/

class Launchpad {
public:
  Launchpad(Sequencer* seq);
  void draw();

private:
  LPad* pads[4];
  uint size;

  LPstate* state;
  Mode_base*  modes[16];
};
