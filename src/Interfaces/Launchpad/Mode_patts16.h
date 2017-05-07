#include "Mode_base16.h"

class Mode_patts16 : public Mode_base16 {
  public:
    Mode_patts16(LPstate* st) : Mode_base16(st) {

    };

    void inputMatrix(uint x, uint y, bool pushed) {
      if (!pushed) return;

      // Edit pattern
      if (state->lastButton(ROW_RIGHT) == BTN_MODE_PATTS) {
        state->select(x+1, y+1);
        state->currentmode = MODE_STEPS;
      }

      // Edit if already selected
      else if ((x+1) == state->trackSel() && (y+1) == state->pattSel())
        state->currentmode = MODE_STEPS;

      // Select and play
      else {
        state->select(x+1, y+1);
        state->selectedTrack()->playPattern(y+1);
      }

    };

    // TOP pushed
    void inputTop(uint n, bool pushed) {
      if (!pushed) return;

    };

    // LEFT pushed
    void inputLeft(uint n, bool pushed){
      if (!pushed) return;

    };


    // DRAW full matrix
    void refresh() {

      // Base draw
      Mode_base16::refresh();

      // draw patterns
      for (uint x = 0; x < state->width*8; x++) {
        Track* track = state->sequencer->track(x+1);
        if (track != NULL)
          for (uint y = 0; y < state->height*8; y++) {
              if (track->pattern(y+1) != NULL && track->pattern(y+1)->notempty())
                if (track->activePatternIndex() == y+1) matrix[x][y] = COLOR_YELLOW;
                else matrix[x][y] = COLOR_GREEN;

              // Blink selected Track/Patt
              if ((x+1) == state->trackSel() && (y+1) == state->pattSel()) {
                if (matrix[x][y] == COLOR_OFF) matrix[x][y] = COLOR_RED_LOW;
                matrix[x][y] = semiblink(matrix[x][y]);
              }

          }
      }

    };

};
