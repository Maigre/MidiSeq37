#pragma once
#include "Mode_abstract.h"


#define BTN_MODE_PATTS  0
#define BTN_MODE_BANK   5

class Mode_base16 : public Mode_abstract {
  public:
    Mode_base16(LPstore* st) : Mode_abstract(st) {};

    // RIGHT pushed
    virtual void inputRight(uint n, bool pushed){

      // PATTERN / STEPS
      if (pushed && n == BTN_MODE_PATTS)
        if (store->getMode() != MODE_PATTS) store->setMode(MODE_PATTS);
        //else store->setMode(MODE_STEPS);

      // BANK
      if (n == BTN_MODE_BANK) store->setMode(MODE_BANK);

    };

    // DRAW
    virtual void refresh() {
      // Clear
      Mode_abstract::refresh();

      // RIGHT
      // Yellow on press
      if (store->buttons->active(ROW_RIGHT) != BTN_NONE)
          extraBtns[ROW_RIGHT][store->buttons->active(ROW_RIGHT)] = COLOR_YELLOW;

      // Mode Pattern / Steps
      extraBtns[ROW_RIGHT][BTN_MODE_PATTS] =
        (store->getMode() == MODE_PATTS) ? COLOR_GREEN :
          (store->getMode() == MODE_STEPS) ? COLOR_YELLOW : COLOR_OFF;

      // Mode Bank
      if (store->getMode() == MODE_BANK)
        extraBtns[ROW_RIGHT][BTN_MODE_BANK] = semiblink(COLOR_GREEN);

    };



};
