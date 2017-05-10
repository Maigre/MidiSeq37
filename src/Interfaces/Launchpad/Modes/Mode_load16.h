#include "Mode_base16.h"
#include "../Memory/MemFiles.h"

class Mode_load16 : public Mode_base16 {
  public:
    Mode_load16(LPstore* st) : Mode_base16(st) {};

    void onFocus() {
      Mode_base16::onFocus();
      bankList = MemFiles::List();
    }

    // TOP pushed
    void inputTop(uint n, bool pushed) {
      if (!pushed) return;

      // Bank Load
      if (n != store->sequencer->bank()) {
        store->sequencer->load(n);
        store->setMode(MODE_DEFAULT);
      }

    };

    // DRAW full matrix
    void refresh() {

      // Base draw
      Mode_base16::refresh();

      drawText("Load", 2);

      // TOP
      // Available banks
      for (uint k=0; (k<bankList.size() && k < store->width*8); k++) {
        if (k == store->sequencer->bank()) extraBtns[ROW_TOP][k] = semiblink(COLOR_YELLOW);
        else extraBtns[ROW_TOP][k] = COLOR_GREEN;
      }

    }

  private:
    vector<uint> bankList;
};
