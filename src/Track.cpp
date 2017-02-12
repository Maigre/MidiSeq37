#include "Track.h"
#include "utils.h"
#include<iostream>
using namespace std;

Track::Track(ofxMidiOut* output, int chan, int barPerLoop, int beatPerBar) {
  midiOut = output;
  channel = chan;
  barSig = beatPerBar;
  barLoop = barPerLoop;
  tickLoop = beats(barSig)*barLoop;

  position.setSignature(barLoop, barSig);

  notesON.resize(tickLoop);
  notesOFF.resize(128);
  for(int k=0; k<128; k++) notesOFF[k] = NULL;

}

Position Track::getPosition() {
  Position _pos;
  tickLock.lock();
  _pos = position;
  tickLock.unlock();
  return _pos;
}

void Track::progress() {
  cout << " Loop: " << position.loop();
  cout << " Bar: " << position.bar() << " Beat: " << position.beat();
  cout << " Ticks: " << position.tick();
  cout << " AbsTicks: " << position.ticks();
  cout << endl;
}

void Track::onTick(uint64_t tick) {

  bool debug = true;

  // Position
  int t = position.set(tick);

  // Notes ON
  notesLock.lock();
  for (auto note = begin(notesON[t]); note != end(notesON[t]); /**/) {
    if (!(*note)->isValid()) note = notesON[t].erase(note);
    else {
      (*note)->play(midiOut, channel, t);     // NoteON
      (*note)->stopTick = (t + (*note)->length) % tickLoop;
      notesOFF[(*note)->note] = (*note);      // Program NoteOFF
      if (debug) {
        cout << "Played note ON ";
        progress(tick);
      }
      ++note;
    }
  }

  // Notes OFF
  for(int k=0; k<128; k++) {
    if (notesOFF[k] != NULL && notesOFF[k]->stopTick == t) {
      notesOFF[k]->stop();
      notesOFF[k] = NULL;
      if (debug) {
        cout << "Played note OFF ";
        progress(tick);
      }
    }
  }
  notesLock.unlock();




  // for (uint16_t k=0; k<cc[t].size(); k++)
  //   midiOut->sendControlChange(channel, cc[t][k]->control, cc[t][k]->value);
  //
  // for (uint16_t k=0; k<pc[t].size(); k++)
  //   midiOut->sendProgramChange(channel, pc[t][k]->value);
}

MidiNote* Track::addNote(int tick, int note, int duration) {
  if (tick >= tickLoop) return NULL;
  MidiNote* noteOn = new MidiNote(note, 64, duration);
  notesLock.lock();
  notesON[tick].push_back(noteOn);
  notesLock.unlock();
  return noteOn;
}

std::vector<MidiNote*> Track::getNotes(int start, int size) {
  std::vector<MidiNote*> notes;
  int end = start + size-1;
  if (end >= tickLoop) end = tickLoop-1;
  notesLock.lock();
  for (int t=start; t<end; t++)
    for (uint16_t k=0; k<notesON[t].size(); k++)
      notes.push_back(notesON[t][k]);
  notesLock.unlock();
  return notes;
}
