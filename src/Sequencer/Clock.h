#include "../Utils/Lockable.h"

class Clock : public Lockable {
  public:

    // config
    Clock();
    void setLoopSize(int barsInLoop);
    void setBarSize(int beatsInBar);

    // update
    int set(uint64_t t);

    // info
    int barsloop();    // number of bar in a loop
    int beatsloop();   // number of beat in a loop
    int ticksloop();  // number of ticks in a loop
    int ticks();      // current tick in ticksloop

    // progression
    int loop();
    int bar();
    int beat();
    int beatfraction(char divider);
    int tick();
    int quarter();
    int eighth();

  private:
    uint64_t tickAll;
    int tickMod;
    int loopTicks;
    int loopBars;
    int barTicks;
    int barBeats;
};
