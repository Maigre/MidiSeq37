#include "Sequencer.h"

Sequencer::Sequencer() {
  // Ticker
  ticker = new Ticker();
  setBPM(120);

  // Default parameters
  barSig = 4;
  barLoop = 4;
}

void Sequencer::setBPM(int newbpm) {
  bpm = newbpm;
  ticker->setTickTime(bpm, RESOLUTION);
}

void Sequencer::progress() {
  uint64_t ticks = ticker->getTicks();

  int tickLoop = RESOLUTION*barSig*barLoop;
  int loopCount = ticks / tickLoop;
  int tickCount = ticks % tickLoop;
  int barCount = tickCount / (barSig*RESOLUTION);
  tickCount = tickCount % (barSig*RESOLUTION);
  int beatCount = tickCount / RESOLUTION;
  tickCount = tickCount % RESOLUTION;

  cout << " Loop: " << loopCount;
  cout << " Bar: " << barCount+1 << " Beat: " << 1+beatCount;
  cout << " Ticks: " << tickCount;
  cout << endl;
}

void Sequencer::start() {
  ticker->startThread(true);
}

void Sequencer::stop() {
  ticker->stopThread();
}
