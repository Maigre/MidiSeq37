#pragma once
#include "ofxMidi.h"

const int RESOLUTION = 960;

struct Track_note {
  int note;
  int velo;
};

struct Track_cc {
  int control;
  int value;
};

struct Track_pc {
  int value;
};


class Track {

  public:
    Track(ofxMidiOut* output, int chan, int barPerLoop, int beatPerBar);
    void onTick(uint64_t tick);

  private:
    int barSig;
		int barLoop;
    int tickLoop;
    int channel;
    ofxMidiOut* midiOut;

    std::vector<std::vector<Track_note*>> notes;
    std::vector<std::vector<Track_cc*>> cc;
    std::vector<std::vector<Track_pc*>> pc;


};
