#pragma once
#include "ofxMidi.h"

class MMidiEvent {
  public:
    MMidiEvent();
    void remove();
    bool isValid();

  protected:
    bool valid;
};


class MMidiNote : public MMidiEvent {
  public:
    MMidiNote(u_int _note, u_int _velo, u_int _length);
    void play(ofxMidiOut* _out, u_int _chan, u_int tick);
    void stop();

    u_int note;
    u_int velocity;
    u_int length;
    u_int stopTick;

  private:
    ofxMidiOut* out;
    u_int channel;
};
