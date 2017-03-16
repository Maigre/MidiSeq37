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
    MMidiNote(int _note, int _velo, int _length);
    void play(ofxMidiOut* _out, int _chan, int tick);
    void stop();

    int note;
    int velocity;
    int length;
    int stopTick;

  private:
    ofxMidiOut* out;
    int channel;
};
