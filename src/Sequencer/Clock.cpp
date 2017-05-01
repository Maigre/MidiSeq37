#include "Clock.h"
#include "../conf.h"


Clock::Clock() {
  tickMod = 0;

  loopBars = 1; // Bars in a loop
  barBeats = 4; // Beats in a bar

  loopTicks = RESOLUTION*loopBars*barBeats;
  barTicks = RESOLUTION*barBeats;

}

void Clock::setLoopSize(uint barsInLoop) {
  lock();
  loopTicks = loopTicks*barsInLoop/loopBars;
  loopBars = barsInLoop;
  unlock();
}

void Clock::setBarSize(uint beatsInBar) {
  lock();
  loopTicks = loopTicks*beatsInBar/barBeats;
  barBeats = beatsInBar;
  unlock();
}

uint Clock::set(uint64_t t) {
  lock();
  tickAll = t;
  tickMod = t % loopTicks;
  unlock();
  return tickMod;
}

uint Clock::barsloop() {
  lock();
  uint ls = loopBars;
  unlock();
  return ls;
}

uint Clock::beatsloop() {
  lock();
  uint ls = barBeats*loopBars;
  unlock();
  return ls;
}

uint Clock::ticksloop() {
  lock();
  uint ls = loopTicks;
  unlock();
  return ls;
}

uint Clock::ticks() {
  lock();
  uint t = tickMod;
  unlock();
  return t;
}

uint Clock::loop() {
  lock();
  uint l = (tickAll / loopTicks)+1;
  unlock();
  return l;
}

uint Clock::bar() {
  lock();
  uint b = (tickMod / barTicks)+1;
  unlock();
  return b;
}

uint Clock::beat() {
  lock();
  uint b = ((tickMod % barTicks) / RESOLUTION)+1;
  unlock();
  return b;
}

uint Clock::beatfraction(char divider) {
  lock();
  uint q = (tickMod*divider/RESOLUTION)+1;
  unlock();
  return q;
}

uint Clock::quarter() {
  lock();
  uint q = (tickMod*4/RESOLUTION)+1;
  unlock();
  return q;
}

uint Clock::eighth() {
  lock();
  uint e = (tickMod*8/RESOLUTION)+1;
  unlock();
  return e;
}

uint Clock::tick() {
  lock();
  uint t = (tickMod % barTicks) % RESOLUTION;
  unlock();
  return t;
}
