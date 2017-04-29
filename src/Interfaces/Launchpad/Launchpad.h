#include "LPad.h"
#include "LPstate.h"

/*
LAUNCHPAD ARRAY class
*/

class Launchpad {
public:
  Launchpad(Sequencer* seq);
  void draw();

private:
  LPad* pads[4];
  LPstate* state;
  char size;
};
