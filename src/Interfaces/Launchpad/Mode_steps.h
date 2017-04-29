#include "Modes.h"

class Mode_steps : public LaunchMode {
  public:
    Mode_steps(Sequencer* seq, LpState* st) : LaunchMode(seq, st);

    void inputMatrix(char x, char y, bool pushed) {
      int note = (7-y)+state->note;
      int step = (x+state->offset*8)*RESOLUTION/state->zoom;

      if (pushed) {
        int length = RESOLUTION/state->zoom;

        Track* track = sequencer->track(state->track);
        std::vector<MMidiNote*> notes = track->getNotes(step, length, note);

        if (notes.size() == 0) sequencer->track(state->track)->addNote(step, note, length-1);
        else for (int k=0; k<notes.size(); k++) notes[k]->remove();
      }
    };

    void inputTop(char n) {};

    void inputLeft(char n){};

    void inputRight(char n){};

  private:


}
