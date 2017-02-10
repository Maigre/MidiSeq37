#include "ofThread.h"
#include "ofxMidi.h"
#include "Ticker.h"
#include "Track.h"
#include "Semaphore.h"

class Sequencer : public ofThread {

  public:
    Sequencer(int size);
    void setBPM(int _bpm);
    void progress();
    void start();
    void stop();

    void threadedFunction();

  private:
    int bpm;
    int reso;
    uint64_t lastTick;
    uint64_t ticks;
    Semaphore* newTick;

    Ticker* ticker;
    std::vector<Track*> tracks;
    ofxMidiOut midiOut;

};
