#include "Clock.h"
#include "../conf.h"


Clock::Clock(int barsInLoop, int beatsInBar) {
  tickMod = 0;
  setSignature(barsInLoop, beatsInBar);
}

void Clock::setSignature(int barsInLoop, int beatsInBar) {
  lock();
  loopTicks = RESOLUTION*beatsInBar*barsInLoop;
  barTicks = (RESOLUTION*barBeats);
  barBeats = beatsInBar;
  unlock();
}

int Clock::set(uint64_t t) {
  lock();
  tickAll = t;
  tickMod = t % loopTicks;
  unlock();
  return tickMod;
}

int Clock::loopsize() {
  lock();
  int ls = loopTicks;
  unlock();
  return ls;
}

int Clock::ticks() {
  lock();
  int t = tickMod;
  unlock();
  return t;
}

int Clock::loop() {
  lock();
  int l = (tickAll / loopTicks)+1;
  unlock();
  return l;
}

int Clock::bar() {
  lock();
  int b = (tickMod / barTicks)+1;
  unlock();
  return b;
}

int Clock::beat() {
  lock();
  int b = ((tickMod % barTicks) / RESOLUTION)+1;
  unlock();
  return b;
}

int Clock::quarter() {
  lock();
  int q = (tickMod*4/RESOLUTION)+1;
  unlock();
  return q;
}

int Clock::eighth() {
  lock();
  int e = (tickMod*8/RESOLUTION)+1;
  unlock();
  return e;
}

int Clock::tick() {
  lock();
  int t = (tickMod % barTicks) % RESOLUTION;
  unlock();
  return t;
}
