#include "MMidiEvent.h"

//////////////////////////////

MMidiEvent::MMidiEvent() {
  valid = true;
  playing = false;
}

void MMidiEvent::remove() {
  valid = false;
}

bool MMidiEvent::isValid() {
  return valid;
}

bool MMidiEvent::isPlaying() {
  return playing;
}

/////////////////////////////

MMidiNote::MMidiNote(u_int _note, u_int _velo, u_int _length)
  : MMidiEvent(), note(_note), velocity(_velo), length(_length), out(NULL), channel(0) {}



void MMidiNote::play(ofxMidiOut* _out, u_int _chan, u_int tick) {
  out = _out;
  channel = _chan;
  if (out == NULL) return;
  out->sendNoteOn(channel, note,  velocity);
  playing = true;
}

void MMidiNote::stop() {
  if (out == NULL) return;
  out->sendNoteOff(channel, note);
  playing = false;
}



////////////////////////////
