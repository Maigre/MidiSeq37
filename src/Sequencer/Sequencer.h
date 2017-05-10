#pragma once
#include "ofThread.h"
#include "ofxMidi.h"
#include "Ticker.h"
#include "Track.h"
#include "../Utils/Semaphore.h"
#include "../conf.h"


class Sequencer : public ofThread, public MemObject {

  public:
    Sequencer(uint midiport);
    void setBPM(int _bpm);
    void progress();
    void start();
    void stop();

    void load(uint bank);
    void save(uint bank);
    void save();
    uint bank();

    Track* track(uint8_t n);

    void threadedFunction();

    Json::Value memdump();
    void memload(Json::Value data);

  private:
    int bpm;
    int reso;
    uint64_t lastTick;
    uint64_t ticks;
    Semaphore* newTick;

    Ticker* ticker;
    std::vector<Track*> tracks;
    ofxMidiOut midiOut;

    uint currentBank;
};
