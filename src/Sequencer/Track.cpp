#include "Track.h"
#include<iostream>
using namespace std;

Track::Track(ofxMidiOut* output, uint chan) {
  midiOut = output;
  channel = chan;

  tickclock = new Clock();

  notesON.resize(tickclock->ticksloop());
  notesOFF.resize(128);
  for(uint k=0; k<128; k++) notesOFF[k] = NULL;
}

Clock* Track::clock() {
  return tickclock;
}

void Track::progress() {
  cout << " Loop: " << tickclock->loop();
  cout << " Bar: " << tickclock->bar();
  cout << " Beat: " << tickclock->beat();
  cout << " Ticks: " << tickclock->tick();
  cout << " AbsTicks: " << tickclock->ticks();
  cout << endl;
}

void Track::resize(uint64_t t) {
  if (t >= notesON.size()) {
    uint oldsize = notesON.size();
    notesON.resize(t+2);
    for(uint k=oldsize; k<=t+1; k++) notesOFF[k] = NULL;
  }
}

void Track::onTick(uint64_t tick) {

  bool debug = true;

  // Clock
  uint t = tickclock->set(tick);

  // Notes ON
  lock();
  resize(tickclock->ticksloop());
  for (auto note = begin(notesON[t]); note != end(notesON[t]); /**/) {
    if (!(*note)->isValid()) note = notesON[t].erase(note);
    else {
      (*note)->play(midiOut, channel, t);     // NoteON
      (*note)->stopTick = (t + (*note)->length) % tickclock->ticksloop();
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
  notesON[tick].push_back(noteOn);
  unlock();
  return noteOn;
}

std::vector<MMidiNote*> Track::getNotes(uint start, uint size) {
  return getNotes(start, size, -1);
}

std::vector<MMidiNote*> Track::getNotes(uint start, uint size, int noteval) {
  std::vector<MMidiNote*> notes;
  uint end = start + size-1;
  if (end >= notesON.size()) end = notesON.size()-1;
  lock();
  for (uint t=start; t<end; t++)
    for (uint16_t k=0; k<notesON[t].size(); k++)
      if (noteval < 0 || (uint)noteval == notesON[t][k]->note)
        if (notesON[t][k]->isValid())
          notes.push_back(notesON[t][k]);
  unlock();
  return notes;
}
