#pragma once
#include "ofxMidi.h"
#include "MMidiEvent.h"
#include "Clock.h"
#include "../Utils/Lockable.h"
#include <mutex>

class Track : public Lockable {

  public:
    Track(ofxMidiOut* output, uint chan);
    void progress();
    void onTick(uint64_t tick);
    Clock* clock();

    MMidiNote* addNote(uint tick, uint note, uint duration);
    std::vector<MMidiNote*> getNotes(uint start, uint size);
    std::vector<MMidiNote*> getNotes(uint start, uint size, int noteval);

    void setChannel(uint ch);
    uint getChannel();


  private:
    void resize(uint64_t t);

    int channel;
    ofxMidiOut* midiOut;

    Clock* tickclock;

    std::vector<std::vector<MMidiNote*>> notesON;
    std::vector<MMidiNote*> notesOFF;

};
