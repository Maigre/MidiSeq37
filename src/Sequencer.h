
#include "Ticker.h"

const int RESOLUTION = 480;

class Sequencer {

  public:
    Sequencer();
    void setBPM(int newbpm);
    void progress();
    void start();
    void stop();

  private:
    int bpm;
    Ticker* ticker;
    
    //Tracker* tracker; //contains multiple Track* tracks[16]
    //each with barSignature & barLoop: after a new tick, calculate new inner tickCount
    //compare with next event tickCount and proceed or return
    //Tracker locks back when all tracks have return, and will be unlocked by Ticker

    // Move to tracks
		int barSig;
		int barLoop;

};

/*Ticker unlock multitracks (thread) trigger,
multitracks trigger calls all tracks to proceed with new tick count
then lock back.*/
