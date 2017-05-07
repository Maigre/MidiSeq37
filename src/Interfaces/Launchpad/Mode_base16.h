#pragma once
#include "Mode_abstract.h"


#define BTN_MODE_PATTS  0

class Mode_base16 : public Mode_abstract {
  public:
    Mode_base16(LPstate* s) : Mode_abstract(s) {
    };

    // RIGHT pushed
    virtual void inputRight(uint n, bool pushed){
      if (!pushed) return;

      if (n == BTN_MODE_PATTS)
        if (state->currentmode == MODE_PATTS)
          state->currentmode = MODE_STEPS;
        else state->currentmode = MODE_PATTS;

    };

    // DRAW
    virtual void refresh() {
      // Clear
      Mode_abstract::refresh();

      // RIGHT
      // Yellow on press
      if (state->lastButton(ROW_RIGHT) != BTN_NONE)
          extraBtns[ROW_RIGHT][state->lastButton(ROW_RIGHT)] = COLOR_YELLOW;

      // Mode Pattern
      extraBtns[ROW_RIGHT][BTN_MODE_PATTS] =
        (state->currentmode == MODE_PATTS) ? COLOR_GREEN : COLOR_OFF;


    };

};
