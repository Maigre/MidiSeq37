#include "ofThread.h"
#include "Semaphore.h"

typedef void (*OnTickClbck)(uint64_t);

class Ticker : public ofThread {

  public:
    Ticker();
    void setTickTime(int _tickTime);

    int getPerformances();
    float getRealBPM();

    void threadedFunction();

    uint64_t* ticks;
    Semaphore* newTick;

    int debug;


  private:


    uint64_t startTime;
    uint64_t lastTickTime;

    int tickTime;

    // Perfomances
    int missedTicks;
};
