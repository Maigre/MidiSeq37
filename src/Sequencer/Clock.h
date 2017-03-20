#include "../Utils/Lockable.h"

class Clock : public Lockable {
  public:
    Clock();
    void setLoopSize(int barsInLoop);
    void setBarSize(int beatsInBar);

    int set(uint64_t t);

    int loopsize();
    int ticks();

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
