#pragma once
#include "Mode_abstract.h"


#define BTN_MODE_PATTS  0
#define BTN_MODE_BANK   5

class Mode_base16 : public Mode_abstract {
  public:
    Mode_base16(LPstore* st) : Mode_abstract(st) {};

    // RIGHT pushed
    virtual void inputRight(uint n, bool pushed){

      // PATTERN
      if (pushed && n == BTN_MODE_PATTS)
        store->setMode(MODE_PATTS);

      // BANK
      if (n == BTN_MODE_BANK)
        if (pushed && store->getMode() == MODE_LOAD) store->setMode(MODE_SAVE);
        else store->setMode(MODE_LOAD);

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
      extraBtns[ROW_RIGHT][BTN_MODE_BANK] =
      (store->getMode() == MODE_LOAD) ? semiblink(COLOR_GREEN) :
        (store->getMode() == MODE_SAVE) ? COLOR_RED : COLOR_OFF;

    };



};
