#pragma once
#include "utils.h"
#include "conf.h"

int beats(int nBeat) {
  return nBeat*RESOLUTION;
}

int beat() {
  return RESOLUTION;
}


Position::Position() {
  tick = 0;
}

void Position::set(uint64_t t) {
    tickLock.lock();
    tickAll = t;
    ticks = t % loopTicks;
    tickLock.unlock();
}

void Position::setSignature(int barsInLoop, int beatsInBar) {
  loopTicks = beats(beatsInBar*barsInLoop);
  barBeats = beatsInBar;
}

int Position::ticks() {
  tickLock.lock();
  int t = ticks;
  tickLock.unlock();
  return t;
}

int Position::bar() {
  return (ticks() / beats(barBeats))+1;
}

int Position::beat() {
  return ((ticks() % beats(barBeats)) / beat())+1;
}

int Position::tick() {
  return (ticks() % beats(barBeats)) % beat();
}

int Position::quarter() {
  return (tick()*4/beat())+1;
}

int Position::eighth() {
  return (tick()*8/beat())+1;
}
