#include "MMidiEvent.h"
#include "Clock.h"

class Pattern {
  public:
    Pattern() {
      pattclock = new Clock();
      notesON.resize(pattclock->ticksloop());
      notempty = false;
    };

    bool check() {
      notempty = false;
      for (uint k=0; k<notesON.size(); k++)
        if (notesON[k].size() > 0) {
          notempty = true;
          break;
        }
      return notempty;
    };

    bool isActive() {
      return notempty;
    };

    Clock* pattclock;
    std::vector<std::vector<MMidiNote*>> notesON;

  private:
    bool notempty;

};
