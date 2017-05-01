#include "Clock.h"
#include "../conf.h"


Clock::Clock() {
  tickMod = 0;

  loopBars = 2; // Bars in a loop
  barBeats = 4; // Beats in a bar

  loopTicks = RESOLUTION*loopBars*barBeats;
  barTicks = RESOLUTION*barBeats;

}

void Clock::setLoopSize(int barsInLoop) {
  lock();
  loopTicks = loopTicks*barsInLoop/loopBars;
  loopBars = barsInLoop;
  unlock();
}

void Clock::setBarSize(int beatsInBar) {
  lock();
  loopTicks = loopTicks*beatsInBar/barBeats;
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

int Clock::barsloop() {
  lock();
  int ls = loopBars;
  unlock();
  return ls;
}

int Clock::beatsloop() {
  lock();
  int ls = barBeats*loopBars;
  unlock();
  return ls;
}

int Clock::ticksloop() {
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

int Clock::beatfraction(char divider) {
  lock();
  int q = (tickMod*divider/RESOLUTION)+1;
  unlock();
  return q;
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
