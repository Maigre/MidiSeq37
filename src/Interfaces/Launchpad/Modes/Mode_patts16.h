#include "Mode_base16.h"

class Mode_patts16 : public Mode_base16 {
  public:
    Mode_patts16(LPstore* st) : Mode_base16(st) {};

    void inputMatrix(uint x, uint y, bool pushed) {
      if (!pushed) return;

      // Edit pattern
      if (store->buttons->active(ROW_RIGHT) == BTN_MODE_PATTS) {
        store->setTrack(x+1, y+1);
        store->setMode(MODE_STEPS);
      }

      // Edit if already selected and playing
      else if ((x+1) == store->getTrack() && (y+1) == store->getPatt()
                && (y+1) == store->track()->activePatternIndex())
        store->setMode(MODE_STEPS);

      // Select and play
      else if ( store->setTrack(x+1, y+1) ) store->track()->playPattern(y+1);

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
      for (uint x = 0; x < store->width*8; x++) {
        Track* track = store->sequencer->track(x+1);
        if (track != NULL)
          for (uint y = 0; y < store->height*8; y++) {

              if (track->pattern(y+1) != NULL && track->pattern(y+1)->notempty()) {
                if (track->activePatternIndex() == y+1) matrix[x][y] = COLOR_YELLOW;
                else matrix[x][y] = COLOR_GREEN;
              }

              // Blink selected Track/Patt
              if ((x+1) == store->getTrack() && (y+1) == store->getPatt()) {
                if (matrix[x][y] == COLOR_OFF) matrix[x][y] = COLOR_RED_LOW;
                matrix[x][y] = semiblink(matrix[x][y]);
              }

          }
      }

    };

};
