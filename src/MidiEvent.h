#include "ofxMidi.h"

class MidiEvent {
  public:
    MidiEvent();
    void remove();
    bool isValid();

  protected:
    bool valid;
};


class MidiNote : public MidiEvent {
  public:
    MidiNote(int _note, int _velo, int _length);
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
