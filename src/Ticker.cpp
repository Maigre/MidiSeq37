#include "Ticker.h"
#include "Track.h"
#include <chrono>
#include<thread>
using namespace std::chrono;


Ticker::Ticker() {
  tickTime = 10000; //default value
  debug = 0;
}


void Ticker::setTickTime(int _tickTime) {
  lock();
  tickTime = _tickTime;
  unlock();
}

int Ticker::getPerformances() {
  int perf;
  lock();
  if (*ticks > 0) perf = (*ticks - missedTicks)*100 / *ticks;
  else perf = 100;
  unlock();
  return perf;
}

float Ticker::getRealBPM() {
  lock();
  double ellapsed = (lastTickTime - startTime)/1000000.0;
  float realbpm = *ticks*60 / (ellapsed * RESOLUTION);
  unlock();
  return realbpm;
}


void Ticker::threadedFunction() {

  chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();
  startTime = duration_cast<microseconds>(now.time_since_epoch()).count();
  lastTickTime = 0;
  missedTicks = 0;
  *ticks = 0;

  while(isThreadRunning())
  {
    // CALCULATE NEW TICKS
    lock();
    chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();
    uint64_t currentTime = duration_cast<microseconds>(now.time_since_epoch()).count();
    if (lastTickTime > 0) {
      int microLapse = (currentTime - lastTickTime);
      lastTickTime = currentTime - (microLapse % tickTime);
      int incTick = (microLapse / tickTime);
      *ticks += incTick;
      if(incTick>1) missedTicks += incTick-1;
    }
    else lastTickTime = currentTime;
    now = now + chrono::microseconds((lastTickTime+tickTime)-currentTime);
    unlock();
    newTick->notify();

    //DEBUG
    if (debug == 1) {
      double ellapsed = (currentTime - startTime)/1000000.0;
      cout << " ELLAPSED: " << ellapsed;
      cout << " TICKS: " << *ticks;
    }


    // SLEEP
    std::this_thread::sleep_until(now);
  }
}
