#include "MMidiEvent.h"
#include "Clock.h"
#include "../Utils/Lockable.h"

class Pattern : public Lockable {
  public:
    Pattern();

    bool notempty();
    Clock* clock();
    void resize();
    void resize(uint64_t t);

    MMidiNote* addNote(uint tick, uint note, uint duration);
    void copyNotes(uint startCopy, uint startPast, uint count);
    std::vector<MMidiNote*> getNotes(uint start, uint size);
    std::vector<MMidiNote*> getNotes(uint start, uint size, int noteval);

    void cleanNotes(uint tick);

  private:


    Clock* pattclock;
    std::vector<std::vector<MMidiNote*>> notesON;

};
