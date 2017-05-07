#pragma once
#include "ofxMidi.h"
#include "MMidiEvent.h"
#include "Clock.h"
#include "Pattern.h"
#include <mutex>
#include "../Utils/Lockable.h"

class Track : public Lockable {

  public:
    Track(ofxMidiOut* output, uint chan);
    void progress();
    void onTick(uint64_t tick);

    Pattern* pattern(uint index);
    Pattern* activePattern();
    uint  activePatternIndex();
    bool  running();
    void  playPattern(uint index);

    void setChannel(uint ch);
    uint getChannel();


  private:

    int channel;
    ofxMidiOut* midiOut;

    std::vector<MMidiNote*> notesOFF;
    std::vector<Pattern*> patterns;
    uint runningPatt;
};
