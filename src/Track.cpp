#include "Track.h"
#include<iostream>
using namespace std;

Track::Track(ofxMidiOut* output, int chan, int barPerLoop, int beatPerBar) {
  midiOut = output;
  channel = chan;
  barSig = beatPerBar;
  barLoop = barPerLoop;
  tickLoop = RESOLUTION*barSig*barLoop;

  notes.resize(tickLoop);
  cc.resize(tickLoop);
  pc.resize(tickLoop);
}

void Track::onTick(uint64_t tick) {

  int t = tick % tickLoop;

  for (uint16_t k=0; k<notes[t].size(); k++)
    midiOut->sendNoteOn(channel, notes[t][k]->note, notes[t][k]->velo);

  for (uint16_t k=0; k<cc[t].size(); k++)
    midiOut->sendControlChange(channel, cc[t][k]->control, cc[t][k]->value);

  for (uint16_t k=0; k<pc[t].size(); k++)
    midiOut->sendProgramChange(channel, pc[t][k]->value);

  // int loopCount = tick / tickLoop;
  // int tickCount = tick % tickLoop;
  // int barCount = tickCount / (barSig*RESOLUTION);
  // tickCount = tickCount % (barSig*RESOLUTION);
  // int beatCount = tickCount / RESOLUTION;
  // tickCount = tickCount % RESOLUTION;
  // cout << " Loop: " << loopCount;
  // cout << " Bar: " << barCount+1 << " Beat: " << 1+beatCount;
  // cout << " Ticks: " << tickCount;
  // cout << endl;
}
