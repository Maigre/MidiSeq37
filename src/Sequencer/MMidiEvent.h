#pragma once
#include "ofxMidi.h"
#include "../Memory/MemObject.h"

class MMidiEvent : public MemObject {
  public:
    MMidiEvent();
    void remove();
    bool isValid();
    bool isPlaying();

  protected:
    bool valid;
    bool playing;
};


class MMidiNote : public MMidiEvent {
  public:
    MMidiNote(u_int _note, u_int _velo, u_int _length);
    void play(ofxMidiOut* _out, u_int _chan);
    void stop();

    u_int note;
    u_int velocity;
    u_int length;
    u_int stopTick;

  private:
    ofxMidiOut* out;
    u_int channel;
};

class MMidiProgram : public MMidiEvent {
  public:
    MMidiProgram(u_int _bank, u_int _program);
    void play(ofxMidiOut* _out, u_int _chan);

    u_int bank;
    u_int program;
};
