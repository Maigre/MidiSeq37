#include "MMidiEvent.h"

//////////////////////////////

MMidiEvent::MMidiEvent() {
  valid = true;
}

void MMidiEvent::remove() {
  valid = false;
}

bool MMidiEvent::isValid() {
  return valid;
}

/////////////////////////////

MMidiNote::MMidiNote(int _note, int _velo, int _length)
  : MMidiEvent(), note(_note), velocity(_velo), length(_length), out(NULL), channel(0) {}

void MMidiNote::play(ofxMidiOut* _out, int _chan, int tick) {
  out = _out;
  channel = _chan;
  out->sendNoteOn(channel, note,  velocity);
}

void MMidiNote::stop() {
  if (out == NULL) return;
  out->sendNoteOff(channel, note);
}



////////////////////////////
