#include "MidiEvent.h"

//////////////////////////////

MidiEvent::MidiEvent() {
  valid = true;
}

void MidiEvent::remove() {
  valid = false;
}

bool MidiEvent::isValid() {
  return valid;
}

/////////////////////////////

MidiNote::MidiNote(int _note, int _velo, int _length)
  : MidiEvent(), note(_note), velocity(_velo), length(_length), out(NULL), channel(0) {}

void MidiNote::play(ofxMidiOut* _out, int _chan, int tick) {
  out = _out;
  channel = _chan;
  out->sendNoteOn(channel, note,  velocity);
}

void MidiNote::stop() {
  if (out == NULL) return;
  out->sendNoteOff(channel, note);
}



////////////////////////////
