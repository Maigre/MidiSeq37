#include "Mode_base16.h"

class Mode_patts16 : public Mode_base16 {
  public:
    Mode_patts16(LPstate* st) : Mode_base16(st) {

    };

    void inputMatrix(uint x, uint y, bool pushed) {
      if (!pushed) return;

      state->tracksel = x+1;
      state->pattsel = y;

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
      for (uint x = 0; x < state->width*8; x++)
        for (uint y = 0; y < state->height*8; y++) {

            if (state->sequencer->track(x+1)->isPattValid(y))
              if (state->sequencer->track(x+1)->isPattActive(y)) matrix[x][y] = COLOR_YELLOW;
              else matrix[x][y] = COLOR_GREEN;
            else if (state->sequencer->track(x+1)->isPattActive(y)) matrix[x][y] = COLOR_RED_LOW;

            if ((x+1) == state->tracksel && y == state->pattsel) {
              if (matrix[x][y] == COLOR_OFF) matrix[x][y] = COLOR_RED_LOW;
              matrix[x][y] = blink(matrix[x][y]);
            }

        }

    };

};
