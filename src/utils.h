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
    void setTick(uint64_t t);
    int loop();
    int bar();
    int beat();
    int tick();
    int quarter();
    int eighth();

  private:
    std::mutex tickLock;
    uint64_t tick;
}
