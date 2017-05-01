#include "LPad.h"
#include "Modes.h"
#include "../conf.h"
#include "../Sequencer/MMidiEvent.h"
#include <unistd.h>
#include <string>

LPad::LPad(LPstate* s, Mode_base** m, char outport, int n) {

  memset(matrixOUT,     COLOR_OFF, sizeof(matrixOUT));
  memset(extraBtnsOUT,  COLOR_OFF, sizeof(extraBtnsOUT));

  state = s;
  modes = m;
  offset = n;

  padOut.openPort(outport);
  padOut.sendControlChange(1, 0, 0);
  padIn.openPort(ofxMidiOut::getPortName(outport));
  padIn.addListener(this);

  cout << "ADDED PAD " << offset << " : " << ofxMidiOut::getPortName(outport) << endl;

}

void LPad::newMidiMessage(ofxMidiMessage& msg) {

  //cout << "Received: " << msg.status << " " << msg.pitch << " " << msg.velocity << " " << msg.control << " " << msg.value << endl;

  // 8x8 matrix & right col
  if (msg.status == 144 )
  {
    // coords
    char x = msg.pitch % 16;
    char y = msg.pitch / 16;

    // pushed
    bool pushed = msg.velocity > 0;

    // 8x8 matrix
    if (x < 8) {
      char xR, yR;

      // rotation
      if (offset%2 == 0) { xR = y; yR = (7-x);}
      else { xR = x; yR = y;}

      // offset
      if (offset%2 == 1) { xR += 8; }
      yR += (offset/2)*8;

      modes[state->currentmode]->inputMatrix(xR, yR, pushed);
    }

    // right col
    else if (x == 8) {

      // transform
      char row, btn;
      if (offset == 0)      {row = ROW_TOP;     btn = y;}
      else if (offset == 1) {row = ROW_RIGHT;   btn = y;}
      else if (offset == 2) {row = ROW_LEFT;    btn = y+8;}
      else if (offset == 3) {row = ROW_BOTTOM;  btn = 15-y;}

      // trigger
      state->buttonRecord(row, btn, pushed);
      modes[state->currentmode]->inputCommand(row, btn, pushed);
    }
  }

  // top row
  else if (msg.status == 176) {

    //btn
    char x = msg.control - 104;
    bool pushed = msg.value > 0;

    // translate
    char row, btn;
    if (offset == 0)      {row = ROW_LEFT;    btn = 7-x;}
    else if (offset == 1) {row = ROW_TOP;     btn = x+8;}
    else if (offset == 2) {row = ROW_BOTTOM;  btn = 7-x;}
    else if (offset == 3) {row = ROW_RIGHT;   btn = x+8;}

    // trigger
    state->buttonRecord(row, btn, pushed);
    modes[state->currentmode]->inputCommand(row, btn, pushed);
  }
}

char LPad::colorRG(char red, char green) {
  return 16*green+red+12;
}

void LPad::draw() {

  char** matrix = modes[state->currentmode]->getMatrix(offset);
  char** extraBtns = modes[state->currentmode]->getCommands(offset);

  // Push matrix
  char xR, yR;
  for (uint x = 0; x < 8; x++){
    bool p = false;
    for (uint y = 0; y < 8; y++)
      //if (matrixOUT[x][y] != matrix[x][y]) {
      if (true) {
        matrixOUT[x][y] = matrix[x][y];

        // rotation
        if (offset == 0) { xR = (7-y); yR = x; }
        else if (offset == 1) { xR = x; yR = y; }
        else if (offset == 2) { xR = x; yR = y; }
        else if (offset == 3) { xR = x; yR = y; }

        // send midi cmd
        padOut.sendNoteOn(1, 16*yR+xR, matrixOUT[x][y]);
        //usleep(1000);
      }
  }

  // Push extra buttons HORIZONTAL
  for (uint b = 0; b < 8; b++)
    //if (extraBtnsOUT[0][b] != extraBtns[0][b]) {
    if (true) {
      extraBtnsOUT[0][b] = extraBtns[0][b];

      // send midi cmd
      if (offset == 0)      padOut.sendNoteOn(1, 16*b+8, extraBtnsOUT[0][b]);
      else if (offset == 1) padOut.sendControlChange(1, 104+b, extraBtnsOUT[0][b]);
      else if (offset == 2) padOut.sendControlChange(1, 111-b, extraBtnsOUT[0][b]);
      else if (offset == 3) padOut.sendNoteOn(1, 16*(7-b)+8, extraBtnsOUT[0][b]);
      //usleep(1000);

      if (offset == 0 && b == 6) cout << to_string(extraBtnsOUT[0][b]) << endl;

    }

  // Push extra buttons VERTICAL
  for (uint b = 0; b < 8; b++)
    //if (extraBtnsOUT[1][b] != extraBtns[1][b]) {
    if (true) {
      extraBtnsOUT[1][b] = extraBtns[1][b];

      // send midi cmd
      if (offset == 0)      padOut.sendControlChange(1, 111-b, extraBtnsOUT[1][b]);
      else if (offset == 1) padOut.sendNoteOn(1, 16*b+8, extraBtnsOUT[1][b]);
      else if (offset == 2) padOut.sendNoteOn(1, 16*(7-b)+8, extraBtnsOUT[1][b]);
      else if (offset == 3) padOut.sendControlChange(1, 104+b, extraBtnsOUT[1][b]);
      //usleep(1000);
    }

}
