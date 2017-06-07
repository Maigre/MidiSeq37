#pragma once
#include "../Utils/Lockable.h"
#include "../Memory/MemObject.h"

class Clock : public Lockable, public MemObject {
  public:

    // config
    Clock();
    void clear();
    void memload(Json::Value data);
    void setLoopSize(uint barsInLoop);
    void setBarSize(uint beatsInBar);

    // update
    uint set(uint64_t t);

    // info
    uint barsloop();    // number of bar in a loop
    uint beatsloop();   // number of beat in a loop
    uint ticksloop();  // number of ticks in a loop
    uint ticks();      // current tick in ticksloop

    // progression
    uint loop();
    uint bar();
    uint beat();
    uint beatfraction(char divider);
    uint tick();
    uint quarter();
    uint eighth();

  private:
    uint64_t tickAll;
    uint tickMod;
    uint loopTicks;
    uint loopBars;
    uint barTicks;
    uint barBeats;
};
