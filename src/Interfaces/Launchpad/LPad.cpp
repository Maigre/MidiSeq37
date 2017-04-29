#include "LPad.h"
#include "Modes.h"
#include "../conf.h"
#include "../Sequencer/MMidiEvent.h"
#include <unistd.h>

LPad::LPad(LPstate* s, char outport, int n) {

  memset(matrix, 0, sizeof(matrix));
  memset(matrixOUT, 0, sizeof(matrixOUT));

  state = s;
  offset = n;

  padOut.openPort(outport);
  padOut.sendControlChange(1, 0, 0);
  padIn.openPort(ofxMidiOut::getPortName(outport));
  padIn.addListener(this);

  cout << "ADDED PAD " << offset << " : " << ofxMidiOut::getPortName(outport) << endl;

}

void LPad::newMidiMessage(ofxMidiMessage& msg) {

  cout << "Received: " << msg.status << " " << msg.pitch << " " << msg.velocity << endl;

  // 8x8 matrix
  if (msg.status == 144) {
    char xR, yR;
    char x = msg.pitch % 16;
    char y = msg.pitch / 16;

    // rotation
    if (offset%2 == 0) { xR = y; yR = (7-x);}
    else { xR = x; yR = y;}

    // MODE_STEPS
    if (state->mode == MODE_STEPS) {

      Track* track = state->activetrack();

      int note = (7-yR)+track->state.basenote;
      int step = (xR+offset*8)*RESOLUTION/track->state.zoom;

      if (msg.velocity > 0) {
        int length = RESOLUTION/track->state.zoom;

        std::vector<MMidiNote*> notes = track->getNotes(step, length, note);

        if (notes.size() == 0) track->addNote(step, note, length-1);
        else for (unsigned int k=0; k<notes.size(); k++) notes[k]->remove();
      }
    }

    // MODE_MAIN_BARS
    else if (state->mode == MODE_MAIN_BARS) {

    }

    // MODE_MAIN_MIX
    else if (state->mode == MODE_MAIN_MIX) {

    }

    // MODE_MAIN_PATT
    else if (state->mode == MODE_MAIN_PATT) {

    }

  }
}

char LPad::colorRG(char red, char green) {
  return 16*green+red+12;
}

void LPad::draw() {
  // clear matrix
  memset(matrix, COLOR_OFF, sizeof(matrix));

  // MODE_STEPS
  if (state->mode == MODE_STEPS) {
    Track* track = state->activetrack();
    int active_col = (track->clock()->beatfraction(track->state.zoom)-1);

    for (unsigned char x = 0; x < 8; x++) {
      // get notes for current grid
      std::vector<MMidiNote*> notes = track->getNotes((x+offset*8)*RESOLUTION/track->state.zoom, RESOLUTION/track->state.zoom);

      for (unsigned char y = 0; y < 8; y++) {
          // vertical red bar
          if ((x+offset*8) == active_col) matrix[x][y] = COLOR_RED;

          // notes green
          for (unsigned int k=0; k<notes.size(); k++)
            if (notes[k]->note == (7-y)+track->state.basenote) {
              if (matrix[x][y] > COLOR_OFF) matrix[x][y] = COLOR_YELLOW;
              else matrix[x][y] = COLOR_GREEN;
            }
      }
    }
  }


  // Push Midi commands
  char xR, yR;
  for (unsigned char x = 0; x < 8; x++)
    for (unsigned char y = 0; y < 8; y++)
      if (matrixOUT[x][y] != matrix[x][y]) {
        matrixOUT[x][y] = matrix[x][y];

        // rotation
        if (offset%2 == 0) { xR = (7-y); yR = x; }
        else { xR = x; yR = y; }

        // send midi cmd
        padOut.sendNoteOn(1, 16*yR+xR, matrixOUT[x][y]);
      }
}
