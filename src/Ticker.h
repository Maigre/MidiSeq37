#include "ofThread.h"

class Ticker : public ofThread {

  public:
    Ticker();
    void setTickTime(int _bpm, int _reso);
    uint64_t getTicks();
    void threadedFunction();

  private:
    uint64_t ticks;
    uint64_t startTime;
    uint64_t lastTickTime;

    int bpm;
    int reso;
    int tickTime;

};
