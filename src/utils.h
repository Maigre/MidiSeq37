#pragma once
#include <mutex>

struct Position {
  int loopCount = tick / tickLoop;
  int tickCount = tick % tickLoop;
  int barCount = tickCount / beats(barSig);
  tickCount = tickCount % beats(barSig);
  int beatCount = tickCount / beat();
  tickCount = tickCount % beat();
}

int beats(int nBeat);
int beat();

class Position {
  public:
    Position();
    void set(uint64_t t);

    uint64_t getTick();
    void setSignature(int barsInLoop, int beatsInBar);

    int ticks();
    int loop();
    int bar();
    int beat();
    int tick();
    int quarter();
    int eighth();

  private:
    std::mutex tickLock;
    int ticks;
    uint64_t tickAll;
    int loopTicks;
    int barBeats;
}
