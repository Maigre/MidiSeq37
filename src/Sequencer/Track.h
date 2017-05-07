#pragma once
#include "ofxMidi.h"
#include "MMidiEvent.h"
#include "Clock.h"
#include "Pattern.h"
#include "../Utils/Lockable.h"
#include <mutex>

class Track : public Lockable {

  public:
    Track(ofxMidiOut* output, uint chan);
    void progress();
    void onTick(uint64_t tick);
    Clock* clock();

    void selectPattern(uint index);
    bool isPattActive(uint index);
    bool isPattValid(uint index);

    MMidiNote* addNote(uint tick, uint note, uint duration);
    void copyNotes(uint startCopy, uint startPast, uint count);
    std::vector<MMidiNote*> getNotes(uint start, uint size);
    std::vector<MMidiNote*> getNotes(uint start, uint size, int noteval);

    void setChannel(uint ch);
    uint getChannel();


  private:
    void resize(uint64_t t);
    Pattern* activePattern();

    int channel;
    ofxMidiOut* midiOut;

    Clock* trackclock;

    std::vector<MMidiNote*> notesOFF;
    std::vector<Pattern*> patterns;
    Pattern* currentPattern;
    uint pattIndex;
};
