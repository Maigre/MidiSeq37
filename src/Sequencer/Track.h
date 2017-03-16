#pragma once
#include "ofxMidi.h"
#include "MMidiEvent.h"
#include "Clock.h"
#include "../Utils/Lockable.h"
#include <mutex>

class Track : public Lockable {

  public:
    Track(ofxMidiOut* output, int chan);
    void progress();
    void onTick(uint64_t tick);
    Clock* clock();

    MMidiNote* addNote(int tick, int note, int duration);
    std::vector<MMidiNote*> getNotes(int start, int size);

  private:
    int channel;
    ofxMidiOut* midiOut;

    Clock* tickclock;

    std::vector<std::vector<MMidiNote*>> notesON;
    std::vector<MMidiNote*> notesOFF;

};
