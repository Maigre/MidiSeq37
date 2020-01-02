#include "Modes.h"
#include "Launchpad.h"
#include "LPstate.h"

Launchpad::Launchpad(Sequencer* seq) {


  // Detect size
  ofxMidiOut midiOut;
  uint offset = 0;
  for (char port=0; port<midiOut.getNumOutPorts(); port++)
    if (midiOut.getOutPortName(port).find("Launchpad") != std::string::npos) {
      //pads[offset] = new LPad(state, modes, port, offset);
      offset++;
      if (offset == 4) break;
    }

  // Load STATE
  state = new LPstate(seq, (offset > 1)?2:1, (offset > 2)?2:1);

  size = offset;

  // Create PADS
  offset = 0;
  std::cout << "Looking for LAUNCHPADs";
  for (char port=0; port<midiOut.getNumOutPorts(); port++) {
    std::cout << std::endl << "Scanning " <<  midiOut.getOutPortName(port);
    if (midiOut.getOutPortName(port).find("Launchpad") != std::string::npos) {
      std::cout << " -> Found !" << std::endl;
      pads[offset] = new LPad(state, port, offset);
      offset++;
      if (offset == 4) break;
    }
  }
  if (offset == 0) std::cout << std::endl << "NO Launchpad found :( ";
  std::cout << std::endl << std::endl;

}

void Launchpad::draw() {


  state->mode()->refresh();

  for (uint k=0; k<size; k++)
    pads[k]->draw();

}
