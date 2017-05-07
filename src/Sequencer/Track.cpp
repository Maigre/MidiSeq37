#include "Track.h"
#include<iostream>
using namespace std;

Track::Track(ofxMidiOut* output, uint chan) {
  midiOut = output;
  channel = chan;

  selectPattern(0);
  //activePattern()->check();

  notesOFF.resize(128);
  for(uint k=0; k<128; k++) notesOFF[k] = NULL;
}

Clock* Track::clock() {
  return trackclock;
}

Pattern* Track::activePattern() {
  if (pattIndex >= patterns.size()) return NULL;
  return patterns[pattIndex];
}

bool Track::isPattActive(uint index) {
  return index == pattIndex;
}

bool Track::isPattValid(uint index) {
  if (index >= patterns.size()) return false;
  patterns[index]->check();
  return patterns[index]->isActive();
}

void Track::selectPattern(uint index) {
  if (activePattern() != NULL) activePattern()->check();
  if (index >= patterns.size()) {
    uint oldsize = patterns.size();
    patterns.resize(index+1);
    for (uint k=oldsize; k<(index+1); k++) patterns[k] = new Pattern();
  }
  pattIndex = index;
  trackclock = activePattern()->pattclock;
}

void Track::progress() {
  cout << " Loop: " << trackclock->loop();
  cout << " Bar: " << trackclock->bar();
  cout << " Beat: " << trackclock->beat();
  cout << " Ticks: " << trackclock->tick();
  cout << " AbsTicks: " << trackclock->ticks();
  cout << endl;
}

void Track::resize(uint64_t t) {
  if (t >= activePattern()->notesON.size()) {
    uint oldsize = activePattern()->notesON.size();
    activePattern()->notesON.resize(t+2);
    //for(uint k=oldsize; k<=t+1; k++) notesOFF[k] = NULL;
  }
}

void Track::onTick(uint64_t tick) {

  bool debug = true;

  // Clock
  uint t = trackclock->set(tick);

  // Notes ON
  lock();
  activePattern()->notesON.resize(trackclock->ticksloop());
  //cout << t << " / " << trackclock->ticksloop() << endl;
  for (auto note = begin(activePattern()->notesON[t]); note != end(activePattern()->notesON[t]); /**/) {
    if (!(*note)->isValid()) note = activePattern()->notesON[t].erase(note);
    else {
      (*note)->play(midiOut, channel, t);     // NoteON
      (*note)->stopTick = (t + (*note)->length) % trackclock->ticksloop();
      notesOFF[(*note)->note] = (*note);      // Program NoteOFF
      if (debug) {
        //cout << "Played note ON ";
        //progress();
      }
      ++note;
    }
  }

  // Notes OFF
  for(uint k=0; k<128; k++) {
    if (notesOFF[k] != NULL && notesOFF[k]->stopTick == t) {
      notesOFF[k]->stop();
      notesOFF[k] = NULL;
      if (debug) {
        //cout << "Played note OFF ";
        //progress();
      }
    }
  }
  unlock();

  // for (uint16_t k=0; k<cc[t].size(); k++)
  //   midiOut->sendControlChange(channel, cc[t][k]->control, cc[t][k]->value);
  //
  // for (uint16_t k=0; k<pc[t].size(); k++)
  //   midiOut->sendProgramChange(channel, pc[t][k]->value);
}

MMidiNote* Track::addNote(uint tick, uint note, uint duration) {
  resize(tick);
  MMidiNote* noteOn = new MMidiNote(note, 64, duration);
  lock();
  activePattern()->notesON[tick].push_back(noteOn);
  unlock();
  return noteOn;
}

void Track::copyNotes(uint startCopy, uint startPast, uint count) {
  for (uint tick=0; tick<count; tick++) {
    if (startCopy+tick >= activePattern()->notesON.size()) break;
    if (startPast+tick >= activePattern()->notesON.size()) break;
    std::vector<MMidiNote*> buffer;
    buffer = getNotes(startCopy+tick, 1);
    lock();
    activePattern()->notesON[startPast+tick].clear();
    unlock();
    for (uint k=0; k<buffer.size(); k++)
      addNote(startPast+tick, buffer[k]->note, buffer[k]->length);

  }
}

std::vector<MMidiNote*> Track::getNotes(uint start, uint size) {
  return getNotes(start, size, -1);
}

std::vector<MMidiNote*> Track::getNotes(uint start, uint size, int noteval) {
  std::vector<MMidiNote*> notes;
  uint end = start + size-1;
  if (end >= activePattern()->notesON.size())
    end = activePattern()->notesON.size()-1;
  lock();
  for (uint t=start; t<=end; t++)
    for (uint k=0; k<activePattern()->notesON[t].size(); k++)
      if (noteval < 0 || (uint)noteval == activePattern()->notesON[t][k]->note)
        if (activePattern()->notesON[t][k]->isValid())
          notes.push_back(activePattern()->notesON[t][k]);
  unlock();
  return notes;
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
