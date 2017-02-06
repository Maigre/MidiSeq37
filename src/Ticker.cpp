#include "Ticker.h"
#include <chrono>
#include<thread>
using namespace std::chrono;


Ticker::Ticker(void) {
  setTickTime(120, 480);
}


void Ticker::setTickTime(int _bpm, int _reso) {
  lock();
  bpm = _bpm;
  reso = _reso;
  tickTime = 60000000/(bpm*reso);
  unlock();
}


uint64_t Ticker::getTicks() {
  uint64_t tic;
  lock();
  tic = ticks;
  unlock();
  return tic;
}


void Ticker::threadedFunction() {

  chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();
  startTime = duration_cast<microseconds>(now.time_since_epoch()).count();
  lastTickTime = 0;
  ticks = 0;

  while(isThreadRunning())
  {
    // CALCULATE NEW TICKS
    lock();
    chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();
    uint64_t currentTime = duration_cast<microseconds>(now.time_since_epoch()).count();
    if (lastTickTime > 0) {
      int microLapse = (currentTime - lastTickTime);
      lastTickTime = currentTime - (microLapse % tickTime);
      ticks += microLapse / tickTime;
    }
    else lastTickTime = currentTime;
    now = now + chrono::microseconds(tickTime);
    unlock();

    //DEBUG
    // double ellapsed = (currentTime - startTime)/1000000.0;
    // double realbpm = ticks*60 / (ellapsed * reso);
    // cout << " ELLAPSED: " << ellapsed;
    // cout << " TICKS: " << ticks;
    // cout << " REAL BPM: " << realbpm << endl;

    // SLEEP
    std::this_thread::sleep_until(now);
  }
}
