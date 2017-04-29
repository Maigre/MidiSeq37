#include "Launchpad.h"

Launchpad::Launchpad(Sequencer* seq) {
  state = new LPstate(seq);

  unsigned char offset = 0;
  for (char port=0; port<ofxMidiOut::getNumPorts(); port++)
    if (ofxMidiOut::getPortName(port).find("Launchpad") != string::npos) {
      pads[offset] = new LPad(state, port, offset);
      offset++;
      if (offset == 4) break;
    }

  size = offset;
}

void Launchpad::draw() {
  for (unsigned char k=0; k<size; k++) pads[k]->draw();
}
