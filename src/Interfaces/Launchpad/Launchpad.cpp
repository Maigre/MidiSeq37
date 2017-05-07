#include "Modes.h"
#include "Launchpad.h"
#include "Mode_steps16.h"
#include "Mode_patts16.h"

Launchpad::Launchpad(Sequencer* seq) {
  // Load STATE
  state = new LPstate(seq);

  // Detect size
  uint offset = 0;
  for (char port=0; port<ofxMidiOut::getNumPorts(); port++)
    if (ofxMidiOut::getPortName(port).find("Launchpad") != string::npos) {
      //pads[offset] = new LPad(state, modes, port, offset);
      offset++;
      if (offset == 4) break;
    }

  size = offset;
  state->width = (offset > 1)?2:1;
  state->height = (offset > 2)?2:1;

  // Load MODES
  modes[MODE_STEPS] = new Mode_steps16(state);
  modes[MODE_PATTS] = new Mode_patts16(state);

  // Create PADS
  offset = 0;
  for (char port=0; port<ofxMidiOut::getNumPorts(); port++)
    if (ofxMidiOut::getPortName(port).find("Launchpad") != string::npos) {
      pads[offset] = new LPad(state, modes, port, offset);
      offset++;
      if (offset == 4) break;
    }
}

void Launchpad::draw() {
  modes[state->currentmode]->refresh();

  for (uint k=0; k<size; k++)
    pads[k]->draw();

}
