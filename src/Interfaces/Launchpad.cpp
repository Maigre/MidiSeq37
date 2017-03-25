#include "Launchpad.h"
#include "Modes.h"
#include "../conf.h"
#include "../Sequencer/MMidiEvent.h"
#include <unistd.h>

Launchpad::Launchpad(Sequencer* seq, char n) {
  sequencer = seq;

  memset(matrixOUT, 0, sizeof(matrixOUT));
  clear();

  state.offset = n;
  state.mode  = MODE_STEPS;
  state.track = 1;
  state.page  = 1;
  state.zoom  = 4;
  state.note  = 43;

  bool found = false;
  unsigned int k;
  char offset = 0;
  for (k=0; k<ofxMidiOut::getNumPorts(); k++)
    if (ofxMidiOut::getPortName(k).find("Launchpad") != string::npos) {
      if (offset == state.offset) {
        padOut.openPort(k);
        padOut.sendControlChange(1, 0, 0);
        padIn.openPort(ofxMidiOut::getPortName(k));
        padIn.addListener(this);
        found = true;
        break;
      }
      else offset++;
    }

  if (found) cout << "Launchpad found on port " << k << ": " << ofxMidiOut::getPortName(k) << endl;
  else cout << "No Launchpad found" << endl;

}

void Launchpad::newMidiMessage(ofxMidiMessage& msg) {

  cout << "Received: " << msg.status << " " << msg.pitch << " " << msg.velocity << endl;

  if (msg.status == 144) {
    char xR, yR;
    char x = msg.pitch % 16;
    char y = msg.pitch / 16;

    // rotation
    if (state.offset%2 == 0) { xR = y; yR = (7-x);}
    else { xR = x; yR = y;}


    int note = (7-yR)+state.note;
    int step = (xR+state.offset*8)*RESOLUTION/state.zoom;

    if (state.mode == MODE_STEPS) {
      if (msg.velocity > 0) add_step(step, note);
    }
  }
}

void Launchpad::add_step(int step, int note) {
  int length = RESOLUTION/state.zoom;

  Track* track = sequencer->track(state.track);
  std::vector<MMidiNote*> notes = track->getNotes(step, length, note);

  if (notes.size() == 0) sequencer->track(state.track)->addNote(step, note, length-1);
  else for (int k=0; k<notes.size(); k++) notes[k]->remove();
}

char Launchpad::colorRG(char red, char green) {
  return 16*green+red+12;
}

void Launchpad::push() {
  bool update;
  char xR, yR;
  for (char x = 0; x < 8; x++)
    for (char y = 0; y < 8; y++)
      if (matrixOUT[x][y] != matrix[x][y]) {
        matrixOUT[x][y] = matrix[x][y];

        // rotation
        if (state.offset%2 == 0) { xR = (7-y); yR = x; }
        else { xR = x; yR = y; }

        // send midi cmd
        padOut.sendNoteOn(1, 16*yR+xR, matrixOUT[x][y]);
        //usleep(200);
      }
}

void Launchpad::clear() {
  // Reset matrix
  memset(matrix, COLOR_OFF, sizeof(matrix));
}

void Launchpad::draw() {
  clear();
  if (state.mode == MODE_STEPS) draw_steps();

  push();
}

void Launchpad::draw_steps() {

  Track* track = sequencer->track(state.track);
  int active_col = (track->clock()->beatfraction(state.zoom)-1);

  for (char x = 0; x < 8; x++) {
    // get notes for current grid
    std::vector<MMidiNote*> notes = track->getNotes((x+state.offset*8)*RESOLUTION/state.zoom, RESOLUTION/state.zoom);

    for (char y = 0; y < 8; y++) {
        // vertical red bar
        if ((x+state.offset*8) == active_col) matrix[x][y] = COLOR_RED;

        // notes green
        for (int k=0; k<notes.size(); k++)
          if (notes[k]->note == (7-y)+state.note)
            if (matrix[x][y] > COLOR_OFF) matrix[x][y] = COLOR_YELLOW;
            else matrix[x][y] = COLOR_GREEN;
    }
  }
}
