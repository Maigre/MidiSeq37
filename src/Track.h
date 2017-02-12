#pragma once
#include "ofxMidi.h"
#include "MidiEvent.h"
#include "utils.h"
#include <mutex>

class Track {

  public:
    Track(ofxMidiOut* output, int chan, int barPerLoop, int beatPerBar);
    void progress(uint64_t tick);
    void onTick(uint64_t tick);
    Position getPosition();

    MidiNote* addNote(int tick, int note, int duration);
    std::vector<MidiNote*> getNotes(int start, int size);

  private:
    int barSig;
		int barLoop;
    int tickLoop;
    int channel;
    ofxMidiOut* midiOut;
    std::mutex notesLock;
    Position position;

    std::vector<std::vector<MidiNote*>> notesON;
    std::vector<MidiNote*> notesOFF;

};
