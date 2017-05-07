#include "Track.h"
#include<iostream>
using namespace std;

Track::Track(ofxMidiOut* output, uint chan) {
  midiOut = output;
  channel = chan;
  runningPatt = 0;

  notesOFF.resize(128);
  for(uint k=0; k<128; k++) notesOFF[k] = NULL;
}

Pattern* Track::pattern(uint index) {
  if (index > patterns.size() || index == 0) return NULL;
  return patterns[index-1];
}

Pattern* Track::activePattern() {
  return pattern(runningPatt);
}

uint Track::activePatternIndex() {
  return runningPatt;
}

bool Track::running() {
  return activePattern() != NULL;
}

void Track::playPattern(uint index) {
  if (index > patterns.size()) {
    uint oldsize = patterns.size();
    patterns.resize(index);
    for (uint k=oldsize; k<index; k++) patterns[k] = new Pattern();
  }
  runningPatt = index;
}

void Track::progress() {
  cout << " Loop: " << activePattern()->clock()->loop();
  cout << " Bar: " << activePattern()->clock()->bar();
  cout << " Beat: " << activePattern()->clock()->beat();
  cout << " Ticks: " << activePattern()->clock()->tick();
  cout << " AbsTicks: " << activePattern()->clock()->ticks();
  cout << endl;
}

void Track::onTick(uint64_t tick) {
  if (!running()) return;

  // Clock
  uint t = activePattern()->clock()->set(tick);

  // Clean Dead notes
  activePattern()->cleanNotes(t);

  // Notes ON
  activePattern()->resize();
  std::vector<MMidiNote*> notes = activePattern()->getNotes(t,1);
  for (uint k=0; k<notes.size(); k++) {
    notes[k]->play(midiOut, channel, t);     // NoteON
    notes[k]->stopTick = (t + notes[k]->length) % activePattern()->clock()->ticksloop();  /// TODO : Pb si changement de pattern plus court
    notesOFF[notes[k]->note] = notes[k];      // Program NoteOFF
  }

  // Notes OFF
  for(uint k=0; k<128; k++) {
    if (notesOFF[k] != NULL && notesOFF[k]->stopTick == t) {
      notesOFF[k]->stop();
      notesOFF[k] = NULL;
    }
  }

  // for (uint16_t k=0; k<cc[t].size(); k++)
  //   midiOut->sendControlChange(channel, cc[t][k]->control, cc[t][k]->value);
  //
  // for (uint16_t k=0; k<pc[t].size(); k++)
  //   midiOut->sendProgramChange(channel, pc[t][k]->value);
}

uint Track::getChannel() {
  lock();
  uint ch = channel;
  unlock();
  return ch;
}

void Track::setChannel(uint ch) {
  lock();
  channel = ch;
  unlock();
}
