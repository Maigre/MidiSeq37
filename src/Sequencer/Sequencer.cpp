#include "Sequencer.h"
#include "../Memory/MemFiles.h"

Sequencer::Sequencer(uint midiport) {

  //Midi Out
  midiOut.openPort(midiport);

  newTick = new Semaphore();
  lastTick = 0;

  // Init Track 0 (background clock)
  track(0);

  // Ticker
  ticker = new Ticker();
  ticker->ticks = &ticks;
  ticker->newTick = newTick;
  ticker->debug = 0;

  setBPM(137);

  currentBank = 0;
}

void Sequencer::load(uint bank) {
  memload( MemFiles::Read(bank) );
  currentBank = bank;
}

void Sequencer::save(uint bank) {
  MemFiles::Write(bank, memdump());
}

void Sequencer::save() {
  save(currentBank);
}

uint Sequencer::bank() {
  return currentBank;
}

void Sequencer::setBPM(int _bpm) {
  bpm = _bpm;
  ticker->setTickTime(60000000/(bpm*RESOLUTION));
}

void Sequencer::progress() {

  if (!ticker->isThreadRunning()) return;

  std::cout << " Perfs: " << ticker->getPerformances();
  std::cout << " Real BPM: "<< ticker->getRealBPM();
  std::cout << std::endl;

  //if (t > 10000) stop();

}

void Sequencer::start() {
  startThread(true);
}

void Sequencer::stop() {
  stopThread();
}

Track* Sequencer::track(uint8_t n) {
  lock();
  if (n >= tracks.size()) {
    uint oldsize = tracks.size();
      tracks.resize(n+1);
      for (uint k=oldsize; k<=n; k++)
        tracks[k] = new Track(&midiOut, k);
  }
  unlock();
  return tracks[n];
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
      //std::cout << "new tick: "<< t << std::endl;

      //check every tracks
      lock();
      for (uint8_t k = 0; k < tracks.size(); k++) tracks[k]->onTick(t);
      unlock();

    }


    lastTick = currentTick;
  }

  ticker->stopThread();
}

// export memory
Json::Value Sequencer::memdump() {
  memory["tracks"].clear();
  lock();
  for (uint k=0; k<tracks.size(); k++)
    memory["tracks"][k] = tracks[k]->memdump();
  unlock();
  return memory;
};

// import memory
void Sequencer::memload(Json::Value data) {
  MemObject::memload(data);

  lock();
  for (uint k=0; k<tracks.size(); k++)
    delete tracks[k];
  tracks.clear();
  unlock();

  for (uint k=0; k<data["tracks"].size(); k++)
    track(k)->memload(data["tracks"][k]);
}
