#include "Pattern.h"
#include<iostream>
using namespace std;

Pattern::Pattern() {
  pattclock = new Clock();
  notesON.resize(pattclock->ticksloop());
};

bool Pattern::notempty() {
  bool notempty = false;
  for (uint k=0; k<notesON.size(); k++)
    if (notesON[k].size() > 0) {
      notempty = true;
      break;
    }
  return notempty;
};

Clock* Pattern::clock() {
  return pattclock;
}

void Pattern::resize() {
   resize(pattclock->ticksloop());
}

void Pattern::resize(uint64_t t) {
  if (t >= notesON.size())
    notesON.resize(t+2);
}

MMidiNote* Pattern::addNote(uint tick, uint note, uint duration) {
  resize(tick);
  MMidiNote* noteOn = new MMidiNote(note, 64, duration);
  lock();
  notesON[tick].push_back(noteOn);
  unlock();
  return noteOn;
}

void Pattern::copyNotes(uint startCopy, uint startPast, uint count) {
  for (uint tick=0; tick<count; tick++) {
    if (startCopy+tick >= notesON.size()) break;
    if (startPast+tick >= notesON.size()) break;
    std::vector<MMidiNote*> buffer;
    buffer = getNotes(startCopy+tick, 1);
    lock();
    notesON[startPast+tick].clear();
    unlock();
    for (uint k=0; k<buffer.size(); k++)
      addNote(startPast+tick, buffer[k]->note, buffer[k]->length);

  }
}

std::vector<MMidiNote*> Pattern::getNotes(uint start, uint size) {
  return getNotes(start, size, -1);
}

std::vector<MMidiNote*> Pattern::getNotes(uint start, uint size, int noteval) {
  std::vector<MMidiNote*> notes;

  uint endT = start + size-1;
  if (endT >= notesON.size())
    endT = notesON.size()-1;

  lock();

  // Stack requested notes
  for (uint t=start; t<=endT; t++)
    for (uint k=0; k<notesON[t].size(); k++)
      if (noteval < 0 || (uint)noteval == notesON[t][k]->note)
        if (notesON[t][k]->isValid())
          notes.push_back(notesON[t][k]);

  unlock();
  return notes;
}

// Clean dead notes
void Pattern::cleanNotes(uint t) {
  if (t >= notesON.size()) return;
  bool clean = false;
  bool scanfrom = 0;
  lock();
  while (!clean) {
    clean = true;
    for (uint k=scanfrom; k<notesON[t].size(); k++)
      if (!notesON[t][k]->isValid() && !notesON[t][k]->isPlaying()) {
        notesON[t].erase(notesON[t].begin() + k);
        clean = false;
        scanfrom = k;
        break;
      }
  }
  unlock();
}
