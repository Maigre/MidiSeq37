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
  : MMidiEvent(), note(_note), velocity(_velo), length(_length), out(NULL), channel(0) {
    memory["note"] = note;
    memory["velocity"] = velocity;
    memory["length"] = length;
  }

void MMidiNote::play(ofxMidiOut* _out, u_int _chan) {
  play(_out, _chan, 1.0);
}

void MMidiNote::play(ofxMidiOut* _out, u_int _chan, float modVolume) {
  out = _out;
  channel = _chan;
  if (out == NULL) return;
  out->sendNoteOn(channel, note,  int(velocity*modVolume));
  playing = true;
}

void MMidiNote::stop() {
  if (out == NULL) return;
  out->sendNoteOff(channel, note);
  playing = false;
}

////////////////////////////

MMidiProgram::MMidiProgram(u_int _bank, u_int _program)
  : MMidiEvent(), bank(_bank), program(_program) {
    memory["bank"] = bank;
    memory["prog"] = program;
  }

void MMidiProgram::play(ofxMidiOut* _out, u_int _chan) {
  if (_out == NULL) return;
  _out->sendControlChange(_chan, 0, bank/8);
  _out->sendControlChange(_chan, 32, bank%8);
  _out->sendProgramChange(_chan, program);
}

////////////////////////////
