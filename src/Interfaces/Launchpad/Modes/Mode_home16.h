#include "Mode_abstract.h"

class Mode_home16 : public Mode_abstract {
  public:
    Mode_home16(LPstore* st) : Mode_abstract(st) {};

    // DRAW full matrix
    void refresh() {

      Mode_abstract::refresh();

      if (!drawText("MiniSeq37", 2, 32, false))
        store->setMode(MODE_PATTS);

    }
};
