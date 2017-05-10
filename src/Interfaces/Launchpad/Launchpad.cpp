#include "Modes.h"
#include "Launchpad.h"
#include "LPstate.h"

Launchpad::Launchpad(Sequencer* seq) {


  // Detect size
  uint offset = 0;
  for (char port=0; port<ofxMidiOut::getNumPorts(); port++)
    if (ofxMidiOut::getPortName(port).find("Launchpad") != string::npos) {
      //pads[offset] = new LPad(state, modes, port, offset);
      offset++;
      if (offset == 4) break;
    }

  // Load STATE
  state = new LPstate(seq, (offset > 1)?2:1, (offset > 2)?2:1);

  size = offset;

  // Create PADS
  offset = 0;
  for (char port=0; port<ofxMidiOut::getNumPorts(); port++)
    if (ofxMidiOut::getPortName(port).find("Launchpad") != string::npos) {
      pads[offset] = new LPad(state, port, offset);
      offset++;
      if (offset == 4) break;
    }
}

void Launchpad::draw() {


  state->mode()->refresh();

  for (uint k=0; k<size; k++)
    pads[k]->draw();

}
