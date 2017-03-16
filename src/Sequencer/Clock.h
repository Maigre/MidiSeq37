#include "../Utils/Lockable.h"

class Clock : public Lockable {
  public:
    Clock(int barsInLoop, int beatsInBar);
    void setSignature(int barsInLoop, int beatsInBar);

    int set(uint64_t t);

    int loopsize();
    int ticks();

    int loop();
    int bar();
    int beat();
    int tick();
    int quarter();
    int eighth();

  private:
    uint64_t tickAll;
    int tickMod;
    int loopTicks;
    int barTicks;
    int barBeats;
};
