#include "Mode_base16.h"

#define BTN_STEPS_PAGE      0
#define BTN_STEPS_CHANNEL   5
#define BTN_STEPS_NOTEUP    6
#define BTN_STEPS_NOTEDOWN  7

#define BTN_COPY    7
#define BTN_PASTE   6

class Mode_save16 : public Mode_base16 {
  public:
    Mode_save16(LPstore* st) : Mode_base16(st) {};

    void onFocus() {
      Mode_base16::onFocus();
      doneSave = false;
    }

    // TOP pushed
    void inputTop(uint n, bool pushed) {
      if (!pushed) return;

      // Bank Save
      if (n == store->sequencer->bank()) {
        store->sequencer->save();
        doneSave = true;
      }

    };

    // DRAW full matrix
    void refresh() {

      // Base draw
      Mode_base16::refresh();

      drawText("Save", 2, 8, false);

      // TOP
      // Current bank
      uint n = store->sequencer->bank();
      if (n < 16) {
        if (doneSave) extraBtns[ROW_TOP][n] = COLOR_GREEN;
        else extraBtns[ROW_TOP][n] = semiblink(COLOR_RED);
      }

    }

  private:
    bool doneSave;
};
