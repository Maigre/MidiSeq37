#include "Mode_base16.h"
#include "../Memory/MemFiles.h"

class Mode_bank16 : public Mode_base16 {
  public:
    Mode_bank16(LPstore* st) : Mode_base16(st) {};

    void onFocus() {
      Mode_base16::onFocus();
      bankList = MemFiles::List();
      doneSave = false;
      doSave = false;
    }

    // TOP pushed
    void inputTop(uint n, bool pushed) {
      if (!pushed) return;

      // Bank Load
      if (!doSave && n != store->sequencer->bank()) {
        store->sequencer->load(n);
        store->setMode(MODE_DEFAULT);
      }

      // Bank Save
      else if (doSave && n == store->sequencer->bank()) {
        store->sequencer->save();
        doneSave = true;
      }

    };

    // RIGHT pushed
    void inputRight(uint n, bool pushed){

      // Secondary mode action
      if (n == BTN_MODE_BANK) {
        if (pushed) doSave = true;
        else if (doneSave) store->prevMode();
        else doSave = false;
      }

      // Base menu
      else Mode_base16::inputRight(n, pushed);
    };

    // DRAW full matrix
    void refresh() {

      // Base draw
      Mode_base16::refresh();

      // LOAD
      if (!doSave) {
        drawText("Load", 2);

        // TOP
        // Available banks
        for (uint k=0; (k<bankList.size() && k < store->width*8); k++) {
          if (k == store->sequencer->bank()) extraBtns[ROW_TOP][k] = semiblink(COLOR_YELLOW);
          else extraBtns[ROW_TOP][k] = COLOR_GREEN;
        }
      }

      // SAVE
      else {
        drawText("Save", 2, 8, false);

        // TOP
        // Current bank
        uint n = store->sequencer->bank();
        if (n < 16) {
          if (doneSave) extraBtns[ROW_TOP][n] = COLOR_GREEN;
          else extraBtns[ROW_TOP][n] = semiblink(COLOR_RED);
        }

        // RIGHT
        extraBtns[ROW_RIGHT][BTN_MODE_BANK] = COLOR_RED;
      }



    }

    bool doneSave;
    bool doSave;

  private:
    vector<uint> bankList;
};
