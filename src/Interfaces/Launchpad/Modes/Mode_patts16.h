#include "Mode_base16.h"

#define BTN_VOLUME    1
#define BTN_COPY      7
#define BTN_PASTE     6


struct btnMatrix {
  uint x;
  uint y;
  bool on;
};

class Mode_patts16 : public Mode_base16 {
  public:
    Mode_patts16(LPstore* st) : Mode_base16(st) {
      clipboard = Json::nullValue;
      activeMatrix.on = false;
    };

    void inputMatrix(uint x, uint y, bool pushed) {

      // memorize matrix push
      if (pushed) {
        activeMatrix.on = true;
        activeMatrix.x = x;
        activeMatrix.y = y;
      }
      else {
        if (activeMatrix.x == x && activeMatrix.y == y) activeMatrix.on = false;
        return;
      }

      // patt pushed: Edit pattern
      if (store->buttons->active(ROW_RIGHT) == BTN_MODE_PATTS) {
        store->setTrack(x+1, y+1);
        activeMatrix.on = false;
        store->setMode(MODE_STEPS);
      }

      // Edit if already selected and playing
      // else if ((x+1) == store->getTrack() && (y+1) == store->getPatt()
      //           && (y+1) == store->track()->activePatternIndex())
      //   store->setMode(MODE_STEPS);

      // Select and play
      else if ( store->setTrack(x+1, y+1) ) store->track()->playPattern(y+1);

    };

    // TOP pushed
    void inputTop(uint n, bool pushed) {
      if (!pushed) return;

      // TOP: Volume if vol pressed
      if (store->buttons->active(ROW_RIGHT) == BTN_VOLUME)
        if (store->pattern() != NULL)
          store->pattern()->setVolume(n*8);

    };

    // LEFT pushed
    void inputLeft(uint n, bool pushed){
      if (!pushed) return;

      // SCENE PLAY
      for (int i=0; i<16; i++)
        if ( store->setTrack(i+1, n+1) ) store->track()->playPattern(n+1);

    };

    // RIGHT pushed
    void inputRight(uint n, bool pushed){
      if (!pushed) return;

      // Copy pattern
      if (n == BTN_COPY) {
        if (store->pattern() != NULL && store->pattern()->notempty())
          clipboard = store->pattern()->memdump();
      }

      // Paste pattern or Erase
      else if (n == BTN_PASTE) {
        if (store->pattern() != NULL)
          // COPY is pressed => Erase pattern
          if (store->buttons->active(ROW_RIGHT, BTN_COPY)) store->pattern()->clear();
          // Paste
          else if (clipboard != Json::nullValue) store->pattern()->memload(clipboard);
      }

      // Base menu
      else Mode_base16::inputRight(n, pushed);
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

      // TOP: Volume if vol pressed
      if (store->buttons->active(ROW_RIGHT) == BTN_VOLUME) {
        extraBtns[ROW_RIGHT][BTN_VOLUME] = COLOR_RED_LOW;
        if (store->pattern() != NULL)
          for (uint k = 0; k < 16; k++)
            if (k <= store->pattern()->getVolume()/8) extraBtns[ROW_TOP][k] = COLOR_RED_LOW;
      }

      // LEFT: Yellow on press
      if (store->buttons->active(ROW_LEFT) != BTN_NONE)
          extraBtns[ROW_LEFT][store->buttons->active(ROW_LEFT)] = COLOR_YELLOW;

    };

  Json::Value clipboard;
  btnMatrix activeMatrix;
};
