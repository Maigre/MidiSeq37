#include "Sequencer.h"

Sequencer::Sequencer(int size) {

  //Midi Out
  midiOut.listPorts();
  midiOut.openPort(0);

  newTick = new Semaphore();
  lastTick = 0;

  // Tracker
  tracks.resize(size+1);
  for (int k=0; k<=size; k++)
    tracks[k] = new Track(&midiOut, k, 4, 4);

  // Ticker
  ticker = new Ticker();
  ticker->ticks = &ticks;
  ticker->newTick = newTick;
  ticker->debug = 0;

  setBPM(137);

}

void Sequencer::setBPM(int _bpm) {
  bpm = _bpm;
  ticker->setTickTime(60000000/(bpm*RESOLUTION));
}

void Sequencer::progress() {

  if (!ticker->isThreadRunning()) return;

  ticker->lock();
  int t = ticks;
  ticker->unlock();

  // cout << " Perfs: " << ticker->getPerformances();
  // cout << " Real BPM: "<< ticker->getRealBPM();
  // cout << endl;

  if (t > 10000) stop();

}

void Sequencer::start() {
  startThread(true);
}

void Sequencer::stop() {
  stopThread();
}

void Sequencer::threadedFunction() {

  ticker->startThread(true);
  uint64_t currentTick = 0;

  // Sequencer is running
  while(isThreadRunning())
  {
    newTick->wait();
    ticker->lock();
    currentTick = ticks;
    ticker->unlock();

    for (uint64_t t = lastTick+1; t <= currentTick; t++) {
      //cout << "new tick: "<< t << endl;

      //check every tracks
      for (uint8_t k = 0; k < tracks.size(); k++) tracks[k]->onTick(t);

    }


    lastTick = currentTick;
  }

  ticker->stopThread();
}
